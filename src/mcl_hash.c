#include "mcl_common.h"
#include "mcl_htable.h"

static inline uint32_t __pool_factor(uint32_t max)
{
    uint32_t i = 0;
    uint32_t value = 0;
    do
    {
        value = (uint32_t)sqrt(max);
        for (i = 2; i <= value; i++) {
            if (max % i == 0) {
                break;
            }
        }
        if (i > value) {
            return max;
        }
    }while (--max);
    return 0;
}

static inline mcl_htable_item_t* __get_node(mcl_htable_t *table, uint32_t idx)
{
    return (mcl_htable_item_t *)(table->nodes + (sizeof(mcl_htable_item_t) + table->data_size) * idx);
}

uint32_t mcl_htable_compute_size(uint32_t max_cnt, uint32_t data_size)
{
    uint32_t cnt_hash  = __pool_factor((size_t)((max_cnt + 1) * 5));
    uint32_t size = sizeof(mcl_htable_t) + cnt_hash * sizeof(uint32_t) + (max_cnt + 1) * (sizeof(mcl_htable_item_t) + data_size);
    return size;
}

void mcl_htable_reset(mcl_htable_t *table)
{
    int size = table->cnt_hash * sizeof(uint32_t) + (table->total + 1) * (sizeof(mcl_htable_item_t) + table->data_size);

    table->idle_idx            = 1; // first item should not be use
    table->cnt_used            = 0;
    table->cnt_idle            = table->total;
    //table->cnt_conflict        = 0;
    memset(table->htable, 0, size);
}

uint32_t mcl_htable_create(void *start, uint32_t mcl_size, uint32_t data_max_cnt, 
                           uint32_t data_size, uint32_t timeout)
{
    mcl_htable_t *table     = (mcl_htable_t *)start;
    uint32_t      cnt_hash  = __pool_factor((size_t)((data_max_cnt + 1) * 5));
    uint32_t      _mcl_size = mcl_htable_compute_size(data_max_cnt, data_size);

    if (mcl_size < _mcl_size) {
        return -1;
    }

    if (data_size != table->data_size || cnt_hash != table->cnt_hash)
    {
        table->data_size           = data_size;
        table->total               = data_max_cnt;
        table->cnt_hash            = cnt_hash;
        table->timeout             = timeout;
        table->nodes               = (char *)start + sizeof(mcl_htable_t) + cnt_hash * sizeof(uint32_t);

        mcl_htable_reset(table);
    }

    return 0;
}

mcl_htable_item_t* mcl_htable_find(mcl_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal)
{
    mcl_htable_item_t *item  = NULL;
    register uint32_t  idx   = 0;
    register uint32_t  times = 0;

    if (unlikely(!table || !key || !data)) {
        return NULL;
    }

    idx = table->htable[hash(key) % table->cnt_hash];
    
    do {
        if (idx == 0) {
            break;
        }
        item = __get_node(table, idx);
        if (item->status == STATUS_USING && equal(item->data, data)) {
            return item;
        }
        idx = item->next;
        ++times;
        if (unlikely(idx >= table->total || times >= table->total)) {
            break;
        }
    } while (1);

    return NULL;
}

static inline uint32_t __pool_idle(mcl_htable_t *table)
{
    uint32_t           times     = 0;
    mcl_htable_item_t *item      = NULL;

    do {
        if (unlikely(table->idle_idx > table->total)) {
            table->idle_idx = 1;
        }
        item = __get_node(table, table->idle_idx);
        if (likely(item->status == STATUS_NOT_USED)) {
            item->status = STATUS_USING;
            return table->idle_idx++;
        } else {
            table->idle_idx++;
        }
    } while (likely(++times < table->total));

    return 0;
}

static inline mcl_htable_item_t* pool_idle(mcl_htable_t *table, uint32_t hash_value)
{
    uint32_t           idx       = hash_value;
    uint32_t           idle_idx  = 0;
    mcl_htable_item_t *item      = NULL;
    mcl_htable_item_t *item_new  = NULL;

    // no idle node
    if (0 == (idle_idx = __pool_idle(table))) {
        return NULL;
    }

    // list is empty
    if (likely(table->htable[idx]) == 0) {
        item = __get_node(table, idle_idx);
        item->prev = 0;
        item->next = 0;

        CVMX_SYNCW;
        table->htable[idx] = idle_idx;
        return item;
    }

    idx = table->htable[idx];         // "idx of htable" and "idx of nodes"
    item = __get_node(table, idx);
    while (item->next) {
        idx  = item->next;
        item = __get_node(table, idx);
    }

    item_new = __get_node(table, idle_idx);
    item_new->next = 0;
    item_new->prev = idx;
    //table->cnt_conflict++;

    CVMX_SYNCW;
    item->next = idle_idx;
    return item_new;
}

mcl_htable_item_t* mcl_htable_insert(mcl_htable_t *table, void *key, void *data, cb_hash hash)
{
    mcl_htable_item_t *item = NULL;

    if (!table || table->total == 0 || !key || !data) {
        return NULL;
    }

    item = pool_idle(table, hash(key) % table->cnt_hash);
    if (unlikely(NULL == item)) {
        return NULL;
    }
    ++table->cnt_used;
    --table->cnt_idle;
    
    memcpy(item->data, data, table->data_size);
    return item;
}

int mcl_htable_del(mcl_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal)
{
    mcl_htable_item_t *item  = NULL;

    if (NULL == (item = mcl_htable_find(table, key, data, hash, equal))) {
        return -1;
    }

    item->status == STATUS_NOT_USED;
    CVMX_SYNCW;
    return 0;
}

void mcl_htable_show(mcl_htable_t *table, void cb(void *))
{
    uint32_t           i;
    mcl_htable_item_t *item = NULL;

    printf("shared hash table addr: %p\n", table);
    //printf("total: %u, used: %u, idle: %u, conflict: %u\n",
    //       table->total, table->cnt_used, table->cnt_idle, table->cnt_conflict);
    printf("total: %u, used: %u, idle: %u\n",
           table->total, table->cnt_used, table->cnt_idle);
    printf("data size: %u, cnt_hash: %u, idle_idx: %u\n", 
           table->data_size, table->cnt_hash,  table->idle_idx);
    printf("\n");

    for (i = 1; i < table->idle_idx; i++) {
        item = __get_node(table, i);
        if (item->status == STATUS_USING) {
            printf("idx: %d, ", i);
            cb(item->data);
        }
    } 
}

mcl_htable_item_t* mcl_htable_get_first(mcl_htable_t *table, mcl_htable_iter_t *iter)
{
    mcl_htable_item_t *item = NULL;

    if (table == NULL || iter == NULL) {
        return NULL;
    }

    iter->table = table;
    iter->idx   = 1;

    for (; i < table->total; (iter->idx)++) {
        item = __get_node(table, iter->idx);
        if (item->status == STATUS_USING) {
            ++(iter->idx);
            return item;
        }
    }
    
    return NULL;
}

mcl_htable_item_t* mcl_htable_get_next(mcl_htable_iter_t *iter)
{
    mcl_htable_item_t *item = NULL;

    if (iter == NULL) {
        return NULL;
    }

    for (; i < iter->table->total; (iter->idx)++) {
        item = __get_node(table, iter->idx);
        if (item->status == STATUS_USING) {
            ++(iter->idx);
            return item;
        }
    }
    
    return NULL;
}
