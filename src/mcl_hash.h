#ifndef _MCL_HTABLE_H_
#define _MCL_HTABLE_H_

#pragma pack(8)
typedef struct _mcl_htable_item {
    volatile uint32_t prev;
    volatile uint32_t next;
    volatile uint32_t status;
//             uint32_t res;

    char data[0];
} mcl_htable_item_t;

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
} mcl_htable_t;

typedef struct {
    mcl_htable_t *table;
    uint32_t     *idx;
} mcl_htable_iter_t;

#pragma pack()

uint32_t mcl_htable_compute_size(uint32_t max_cnt, uint32_t data_size);

uint32_t mcl_htable_create(void *start, uint32_t mcl_size, uint32_t data_max_cnt, 
                           uint32_t data_size, uint32_t timeout);

mcl_htable_item_t* mcl_htable_find(mcl_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

mcl_htable_item_t* mcl_htable_insert(mcl_htable_t *table, void *key, void *data, cb_hash hash);

int mcl_htable_del(mcl_htable_t *table, void *key, void *data, cb_hash hash, cb_equal equal);

void mcl_htable_show(mcl_htable_t *table, void cb(void *));

void mcl_htable_reset(mcl_htable_t *table);

mcl_htable_item_t* mcl_htable_get_first(mcl_htable_t *table, mcl_htable_iter_t *iter);

mcl_htable_item_t* mcl_htable_get_next(mcl_htable_iter_t *iter);

#endif
