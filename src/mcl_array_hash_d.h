#ifndef _MCL_ARRAY_HASH_H_
#define _MCL_ARRAY_HASH_H_

/*
  mcl_array_hash_r 是一个数组哈希表，通过指向数组的指针保存冲突的条目。d表示del

  与mcl_array_hash类似，但是支持删除条目。不支持超时删除。

*/

#define LOAD_FACTOR (1/5)

#pragma pack(8)

typedef struct _mcl_array_hash_entry {
    uint32_t cnt_max;
    uint32_t cnt_used;

    char     data[0];
} mcl_array_hash_entry_t;

typedef struct {
    // immutable
    uint32_t         entry_size;
    uint32_t         cnt_max;
    uint32_t         cnt_bucket;

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

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, uint32_t cnt_bucket);

uint32_t mcl_array_hash_create(void *start, uint32_t our_size, uint32_t data_max_cnt, 
                           uint32_t entry_size, uint32_t timeout);

mcl_array_hash_entry_t* mcl_array_hash_find(mcl_array_hash_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

mcl_array_hash_entry_t* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *data, cb_hash hash);

void mcl_array_hash_reset(mcl_array_hash_t *table);

void mcl_array_hash_show(mcl_array_hash_t *table, void cb(void *));

mcl_array_hash_entry_t* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter);

mcl_array_hash_entry_t* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter);

#endif
