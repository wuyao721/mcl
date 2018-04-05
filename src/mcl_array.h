#ifndef _MCL_ARRAY_H_
#define _MCL_ARRAY_H_

#include <stdint.h>

#pragma pack(8)

typedef struct {
    volatile uint32_t status;
                 char data[0];
} mcl_array_item_t;

typedef struct {
    uint32_t         data_size;
    uint32_t         total;
    //int              (*equal)(const void*, const void*);

    uint32_t         idle_idx;
    uint32_t         cnt_used;
    uint32_t         cnt_idle;

    char             items[0];
} mcl_array_t;

#pragma pack()

uint32_t mcl_array_compute_size(uint32_t max_cnt, uint32_t data_size);

void mcl_array_reset(mcl_array_t *array);

mcl_array_t* mcl_array_create(void *mem, uint32_t mem_size, uint32_t max_cnt, uint32_t data_size);

void* mcl_array_find(mcl_array_t *array, void *data, cb_equal eq);

void* mcl_array_insert(mcl_array_t *array, void *data);

void* mcl_array_del(mcl_array_t *array, void *data);

//typedef void (*mcl_array_show_cb)(void *item);
void mcl_array_show(mcl_array_t *array, void cb(void *));

void mcl_array_travel(mcl_array_t *array, void cb(void *, void *, void *), void *arg1, void *arg2);

#endif
