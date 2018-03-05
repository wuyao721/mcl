#ifndef _OUR_HTABLE_H_
#define _OUR_HTABLE_H_

#pragma pack(8)
typedef struct _our_htable_item {
    volatile uint32_t prev;
    volatile uint32_t next;
    volatile uint32_t status;
//             uint32_t res;

    char data[0];
} our_htable_item_t;

typedef struct {
    uint32_t         data_size;
    uint32_t         total;
    uint32_t         cnt_hash;
    uint32_t         timeout;
    uint32_t         type;
    uint32_t         (*hash)(const void*);
    int              (*equal)(const void*, const void*);

    uint32_t         idle_idx;
    uint32_t         cnt_used;
    uint32_t         cnt_idle;
    //uint32_t         cnt_conflict;

    char            *nodes;
    uint32_t         htable[0];
} our_htable_t;

typedef struct {
    our_htable_t *table;
    uint32_t     *idx;
} our_htable_iter_t;

#pragma pack()

uint32_t our_htable_compute_size(uint32_t max_cnt, uint32_t data_size);

uint32_t our_htable_create(void *start, uint32_t our_size, uint32_t data_max_cnt, 
                           uint32_t data_size, uint32_t timeout);

our_htable_item_t* our_htable_find(our_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

our_htable_item_t* our_htable_insert(our_htable_t *table, void *key, void *data, cb_hash hash);

int our_htable_del(our_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

void our_htable_show(our_htable_t *table, void cb(void *));

void our_htable_reset(our_htable_t *table);

our_htable_item_t* our_htable_get_first(our_htable_t *table, our_htable_iter_t *iter);

our_htable_item_t* our_htable_get_next(our_htable_iter_t *iter);

#endif
