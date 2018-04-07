#include "mcl_common.h"
#include "mcl_array_hash.h"

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

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, float load_factor)
{
    uint32_t cnt_bucket  = __pool_factor((uint32_t)((float)max_cnt / load_factor));
    return sizeof(mcl_array_hash_t) + cnt_bucket * sizeof(uint8_t *);
}

uint32_t mcl_array_hash_create(void *start, uint32_t buf_size, uint32_t entry_size, uint32_t max_cnt, float load_factor)
{
    mcl_array_hash_t *table   = (mcl_array_hash_t *)start;
    uint32_t      cnt_bucket  = __pool_factor((uint32_t)((float)max_cnt / load_factor));
    uint32_t      size        = mcl_array_hash_compute_size(entry_size, max_cnt, load_factor);

    if (size < buf_size) {
        return -1;
    }
    
    if ((CACHE_LINE_SIZE - sizeof(mcl_array_hash_entries_t)) < entry_size) {
        return -1;
    }

    if (entry_size != table->entry_size || max_cnt != table->cnt_max  || cnt_bucket != table->cnt_bucket) {
        table->entry_size          = entry_size;
        table->cnt_max             = max_cnt;
        table->cnt_bucket          = cnt_bucket;

        mcl_array_hash_reset(table);
    }

    return 0;
}

inline void mcl_array_hash_reset(mcl_array_hash_t *table)
{
    table->cnt_used             = 0;
    table->cnt_conflict         = 0;
    table->cnt_resize           = 0;
    table->cnt_bucket_max_size  = CACHE_LINE_SIZE;
}

inline void* mcl_array_hash_entry(mcl_array_hash_t *table, mcl_array_hash_entries_t *entries, int entry_idx)
{
    return ((void *)entries) + sizeof(mcl_array_hash_entries_t) + entry_idx * table->entry_size;
}

void* mcl_array_hash_find(mcl_array_hash_t *table, void *key, cb_hash hash, cb_equal equal)
{
    mcl_array_hash_entries_t *entries  = NULL;
    void                     *entry    = NULL;
    int                       i        = 0;

    if (unlikely(!table || !key)) {
        return NULL;
    }

    if (NULL == (entries = (mcl_array_hash_entries_t *)table->buckets[hash(key) % table->cnt_bucket])) {
        return NULL;
    }
    
    for (i = 0; i < entries->cnt_used; i++) {
        entry = mcl_array_hash_entry(table, entries, i);
        if (likely(equal(entry, key))) {
            return entry;
        }
    }

    return NULL;
}

void* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *value, cb_hash hash)
{
    mcl_array_hash_entries_t *entries = NULL;
    void                     *entry = NULL;

    if (!table || !key || !value) {
        return NULL;
    }

    if (table->cnt_used >= table->cnt_max) {
        return NULL;
    }

    if (NULL == table->buckets[hash(key) % table->cnt_bucket]) {
        if (NULL == (entries = calloc(1, CACHE_LINE_SIZE))) {
            return NULL;
        }
        entries->size = CACHE_LINE_SIZE;
        entries->cnt_max = (CACHE_LINE_SIZE - sizeof(mcl_array_hash_entries_t)) / table->entry_size;
        entries->cnt_used = 1;
        entry = entries->data;
        table->buckets[hash(key) % table->cnt_bucket] = (uint8_t *)entries;
    } else {
        entries = (mcl_array_hash_entries_t *)table->buckets[hash(key) % table->cnt_bucket];
        if (entries->cnt_max == entries->cnt_used) {
            if (NULL == (entries = (mcl_array_hash_entries_t *)realloc(entries, entries->size * 2))) { // not multi-thread safe
                return NULL;
            }
            table->cnt_conflict++;
            if (entries->size * 2 > table->cnt_bucket_max_size) {
                table->cnt_bucket_max_size = entries->size;
            }
            entries->size *= 2;
            entries->cnt_max = (entries->size - sizeof(mcl_array_hash_entries_t)) / table->entry_size;
            table->buckets[hash(key) % table->cnt_bucket] = (uint8_t *)entries;
        }
        entry = entries->data + entries->cnt_used * table->entry_size;
        entries->cnt_used++;
        table->cnt_conflict++;
    }

    ++table->cnt_used;
    
    memcpy(entry, value, table->entry_size);
    return entry;
}

static void* __mcl_array_hash_get_one(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter)
{
    mcl_array_hash_entries_t *entries = NULL;
    void                     *entry   = NULL;
    
    for (; iter->bucket_idx < table->cnt_bucket; (iter->bucket_idx)++) {
        if (NULL != (entries = (mcl_array_hash_entries_t *)table->buckets[iter->bucket_idx])) {
            for (; iter->entry_idx < entries->cnt_used; (iter->entry_idx)++) {
                entry = entries->data + table->entry_size * iter->entry_idx;
                (iter->entry_idx)++;
                return entry;
            }
        }
    }
    
    return NULL;
}

void* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter)
{
    if (table == NULL || iter == NULL) {
        return NULL;
    }
    
    iter->table       = table;
    iter->bucket_idx  = 0;
    iter->entry_idx   = 0;
    
    return __mcl_array_hash_get_one(table, iter);
}

void* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter)
{
    if (iter == NULL) {
        return NULL;
    }
    
    return __mcl_array_hash_get_one(iter->table, iter);
}
