#ifndef _OUR_ARRAY_H_
#define _OUR_ARRAY_H_

#include <stdint.h>

#pragma pack(8)

typedef struct {
    volatile uint32_t status;
                 char data[0];
} our_array_item_t;

typedef struct {
    uint32_t         data_size;
    uint32_t         total;
    //int              (*equal)(const void*, const void*);

    uint32_t         idle_idx;
    uint32_t         cnt_used;
    uint32_t         cnt_idle;

    char             items[0];
} our_array_t;

#pragma pack()

uint32_t our_array_compute_size(uint32_t max_cnt, uint32_t data_size);

void our_array_reset(our_array_t *array);

our_array_t* our_array_create(void *mem, uint32_t mem_size, uint32_t max_cnt, uint32_t data_size);

void* our_array_find(our_array_t *array, void *data, cb_equal eq);

void* our_array_insert(our_array_t *array, void *data);

void* our_array_del(our_array_t *array, void *data);

//typedef void (*our_array_show_cb)(void *item);
void our_array_show(our_array_t *array, void cb(void *));

void our_array_travel(our_array_t *array, void cb(void *, void *, void *), void *arg1, void *arg2);

#endif
