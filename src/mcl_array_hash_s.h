#ifndef _MCL_ARRAY_HASH_S_H_
#define _MCL_ARRAY_HASH_S_H_

/*
  mcl_array_hash_s 是一个数组哈希表，通过数组保存冲突的条目。s表示small

  与mcl_array_hash类似，但是：
  - 桶不是不是指针或者下标，而是数组本身
  - 适合条目数量比较小（百万级别以下）的情景
  - 比mcl_array_hash少一次cache miss
  - 每个桶可容纳的最大条目是固定的，无法扩容
*/

#pragma pack(8)

typedef struct _mcl_array_hash_entries {
    uint32_t cnt_used;

    char     data[0];
} mcl_array_hash_entries_t;

typedef struct {
    // immutable
    uint32_t         entry_size;
    uint32_t         cnt_max;
    uint32_t         cnt_bucket;
    uint32_t         bucket_max_entry;

    // mutable
    uint32_t         cnt_used;
    uint32_t         cnt_idle;
    uint32_t         cnt_conflict;
    uint32_t         cnt_resize;

    // buckets
    uint8_t          buckets[0];
} mcl_array_hash_t;

typedef struct {
    mcl_array_hash_t *table;
    uint32_t         *idx;
} mcl_array_hash_iter_t;

#pragma pack()

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor);

uint32_t mcl_array_hash_create(void *start, uint32_t buf_size, uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor);

void* mcl_array_hash_find(mcl_array_hash_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

void* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *data, cb_hash hash);

inline void mcl_array_hash_reset(mcl_array_hash_t *table);

void mcl_array_hash_show(mcl_array_hash_t *table, void cb(void *));

void* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter);

void* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter);

#endif
