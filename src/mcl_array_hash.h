#ifndef _MCL_ARRAY_HASH_H_
#define _MCL_ARRAY_HASH_H_

/*
  mcl_array_hash ��һ�������ϣ��ͨ��ָ�������ָ�뱣���ͻ����Ŀ

  ʹ�ó�����
     - �ʺ���Ŀ(entry)��С�Ƚ�С���龰��20�ֽ����ڣ��������ܸ���Ч������CPU cache�Լ�TLB��
     - �ʺ���Ŀ�����Ƚϴ󣨰��򼶱�����ϣ����龰������С��ȻҲ�ʺϣ�
     - ��Ҫ��ǰԤ֪��Ŀ��ģ�Լ����ֵ��������ʵ�����������������(load factor)

  �ص㣺
     - Ͱ(bucket)��һ��ָ�룬ָ��һ������չ������
     - ͨ�����͸������ӣ��ɽ���ͻ�ʽ��͵�һ��������ļ���
     - ��֧��ɾ����Ŀ
     - ��Ŀ��Ҫ��̬���䣨��Ҫ�ڴ�ع���
     - Ԥ�����������cache miss
*/

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
    uint8_t         *buckets[0];
} mcl_array_hash_t;

typedef struct {
    mcl_array_hash_t *table;
    uint32_t         *idx;
} mcl_array_hash_iter_t;

#pragma pack()

uint32_t mcl_array_hash_compute_size(uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor);

uint32_t mcl_array_hash_create(void *start, uint32_t buf_size, uint32_t entry_size, uint32_t max_cnt, uint32_t load_factor);

mcl_array_hash_entry_t* mcl_array_hash_find(mcl_array_hash_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

mcl_array_hash_entry_t* mcl_array_hash_insert(mcl_array_hash_t *table, void *key, void *data, cb_hash hash);

inline void mcl_array_hash_reset(mcl_array_hash_t *table);

void mcl_array_hash_show(mcl_array_hash_t *table, void cb(void *));

mcl_array_hash_entry_t* mcl_array_hash_get_first(mcl_array_hash_t *table, mcl_array_hash_iter_t *iter);

mcl_array_hash_entry_t* mcl_array_hash_get_next(mcl_array_hash_iter_t *iter);

#endif
