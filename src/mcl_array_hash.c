#include "our_common.h"
#include "mcl_array_hash.h"

#define LOAD_FACTOR (0.2)

static inline uint32_t __pool_factor(uint32_t max)
{
    uint32_t i = 0;
    uint32_t value = 0;

    do {
        value = (uint32_t)sqrt(max);
        for (i = 2; i <= value; i++) {
            if (max % i == 0) {
                break;
            }
        }
        if (i > value) {
            return max;
        }
    } while (--max);

    return 0;
}

static inline mcl_array_hash_entry_t* __get_node(mcl_array_hash_t *table, uint32_t idx)
{
    return (mcl_array_hash_entry_t *)(table->nodes + (sizeof(mcl_array_hash_entry_t) + table->entry_size) * idx);
}

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor);
{
    uint32_t cnt_bucket  = __pool_factor((uint32_t)((float)max_cnt / load_factor));
    return sizeof(mcl_array_hash_t) + cnt_bucket * sizeof(uint32_t);
}

uint32_t mcl_array_hash_create(void *start, uint32_t buf_size, uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor)
{
    mcl_array_hash_t *table   = (mcl_array_hash_t *)start;
    uint32_t      cnt_bucket  = __pool_factor((uint32_t)((float)cnt_max / load_factor));
    uint32_t      size        = mcl_array_hash_compute_size(cnt_max, entry_size, load_factor);

    if (size < buf_size) {
        return -1;
    }

    if (entry_size != table->entry_size || cnt_max != table->cnt_max  || cnt_bucket != table->cnt_bucket) {
        table->entry_size          = entry_size;
        table->cnt_max             = cnt_max;
        table->cnt_bucket          = cnt_bucket;

        mcl_array_hash_reset(table);
    }

    return 0;
}

inline void mcl_array_hash_reset(mcl_array_hash_t *table)
{
    table->cnt_used            = 0;
    table->cnt_idle            = table->cnt_max;
    table->cnt_conflict        = 0;
    table->cnt_resize          = 0;
}

mcl_array_hash_entry_t* mcl_array_hash_find(mcl_array_hash_t *table, void *key, void *data, cb_hash hash, cb_equal equal)
{
    mcl_array_hash_entry_t *item  = NULL;
    register uint32_t       idx   = 0;

    if (unlikely(!table || !key || !data)) {
        return NULL;
    }

    idx = table->htable[hash(key) % table->cnt_bucket];
    
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
        if (unlikely(idx >= table->cnt_max || times >= table->cnt_max)) {
            break;
        }
    } while (1);

    return NULL;
}

static inline uint32_t __pool_idle(mcl_array_hash_t *table)
{
    uint32_t           times     = 0;
    mcl_array_hash_entry_t *item      = NULL;

    do {
        if (unlikely(table->idle_idx > table->cnt_max)) {
            table->idle_idx = 1;
        }
        item = __get_node(table, table->idle_idx);
        if (likely(item->status == STATUS_NOT_USED)) {
            item->status = STATUS_USING;
            return table->idle_idx++;
        } else {
            table->idle_idx++;
        }
    } while (likely(++times < table->cnt_max));

    return 0;
}

static inline mcl_array_hash_entry_t* pool_idle(mcl_array_hash_t *table, uint32_t hash_value)
{
    uint32_t           idx       = hash_value;
    uint32_t           idle_idx  = 0;
    mcl_array_hash_entry_t *item      = NULL;
    mcl_array_hash_entry_t *item_new  = NULL;

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

mcl_array_hash_entry_t* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *data, cb_hash hash)
{
    mcl_array_hash_entry_t *item = NULL;

    if (!table || table->cnt_max == 0 || !key || !data) {
        return NULL;
    }

    item = pool_idle(table, hash(key) % table->cnt_bucket);
    if (unlikely(NULL == item)) {
        return NULL;
    }
    ++table->cnt_used;
    --table->cnt_idle;
    
    memcpy(item->data, data, table->entry_size);
    return item;
}

int mcl_array_hash_del(mcl_array_hash_t *table, void *key, void *data, cb_hash hash, cb_equal equal)
{
    mcl_array_hash_entry_t *item  = NULL;

    if (NULL == (item = mcl_array_hash_find(table, key, data, hash, equal))) {
        return -1;
    }

    item->status == STATUS_NOT_USED;
    CVMX_SYNCW;
    return 0;
}

void mcl_array_hash_show(mcl_array_hash_t *table, void cb(void *))
{
    uint32_t           i;
    mcl_array_hash_entry_t *item = NULL;

    printf("shared hash table addr: %p\n", table);
    //printf("cnt_max: %u, used: %u, idle: %u, conflict: %u\n",
    //       table->cnt_max, table->cnt_used, table->cnt_idle, table->cnt_conflict);
    printf("cnt_max: %u, used: %u, idle: %u\n",
           table->cnt_max, table->cnt_used, table->cnt_idle);
    printf("data size: %u, cnt_bucket: %u, idle_idx: %u\n", 
           table->entry_size, table->cnt_bucket,  table->idle_idx);
    printf("\n");

    for (i = 1; i < table->idle_idx; i++) {
        item = __get_node(table, i);
        if (item->status == STATUS_USING) {
            printf("idx: %d, ", i);
            cb(item->data);
        }
    } 
}

mcl_array_hash_entry_t* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter)
{
    mcl_array_hash_entry_t *item = NULL;

    if (table == NULL || iter == NULL) {
        return NULL;
    }

    iter->table = table;
    iter->idx   = 1;

    for (; i < table->cnt_max; (iter->idx)++) {
        item = __get_node(table, iter->idx);
        if (item->status == STATUS_USING) {
            ++(iter->idx);
            return item;
        }
    }
    
    return NULL;
}

mcl_array_hash_entry_t* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter)
{
    mcl_array_hash_entry_t *item = NULL;

    if (iter == NULL) {
        return NULL;
    }

    for (; i < iter->table->cnt_max; (iter->idx)++) {
        item = __get_node(table, iter->idx);
        if (item->status == STATUS_USING) {
            ++(iter->idx);
            return item;
        }
    }
    
    return NULL;
}
