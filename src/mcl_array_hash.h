#ifndef _MCL_ARRAY_HASH_H_
#define _MCL_ARRAY_HASH_H_

/*
  mcl_array_hash 是一个数组哈希表，通过指向数组的指针保存冲突的条目

  使用场景：
     - 适合条目(entry)大小比较小的情景（20字节以内，这样才能更高效的利用CPU cache以及TLB）
     - 适合条目数量比较大（百万级别或以上）的情景（数量小当然也适合）
     - 需要提前预知条目规模以及最大值，并根据实际情况调整负载因子(load factor)

  特点：
     - 桶(bucket)是一个指针，指向一个可扩展的数组
     - 通过降低负载因子，可将冲突率降低到一个较满意的级别
     - 不支持删除条目
     - 条目需要动态分配（需要内存池管理）
     - 预计最多有两次cache miss
*/

#define DEFAULT_LOAD_FACTOR (0.2)

typedef struct _mcl_array_hash_entries {
    uint32_t cnt_max;
    uint32_t cnt_used;
    uint32_t size;

    uint8_t  data[0];
} mcl_array_hash_entries_t;

typedef struct {
    // immutable
    uint32_t         entry_size;
    uint32_t         cnt_max;
    uint32_t         cnt_bucket;

    // mutable
    uint32_t         cnt_used;
    uint32_t         cnt_conflict;
    uint32_t         cnt_resize;
    uint32_t         cnt_bucket_max_size;

    // buckets
    uint8_t         *buckets[0];
} mcl_array_hash_t;

typedef struct {
    mcl_array_hash_t *table;
    uint32_t          bucket_idx;
    uint32_t          entry_idx;
} mcl_array_hash_iter_t;

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, float load_factor);

uint32_t mcl_array_hash_create(void *start, uint32_t buf_size, uint32_t entry_size, uint32_t max_cnt, float load_factor);

void* mcl_array_hash_find(mcl_array_hash_t *table, void *key, cb_hash hash, cb_equal equal);

void* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *data, cb_hash hash);

inline void mcl_array_hash_reset(mcl_array_hash_t *table);

void* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter);

void* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter);

#endif
