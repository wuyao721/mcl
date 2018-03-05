#include "our_common.h"
#include "our_array.h"

static inline our_array_item_t* __get_item(our_array_t *array, uint32_t idx)
{
    return (our_array_item_t *)(array->items + (sizeof(our_array_item_t) + array->data_size) * idx);
}

uint32_t our_array_compute_size(uint32_t max_cnt, uint32_t data_size)
{
    return sizeof(our_array_t) + max_cnt * (data_size + sizeof(our_array_item_t));
}

void our_array_reset(our_array_t *array)
{
    int size = array->total * (array->data_size + sizeof(our_array_item_t));

    array->idle_idx = 0;
    array->cnt_used = 0;
    array->cnt_idle = array->total;
    memset(array->items, 0, size);
}

our_array_t* our_array_create(void *mem, uint32_t mem_size, uint32_t max_cnt, uint32_t data_size)
{
    our_array_t   *array = (our_array_t *)mem;
    uint32_t       size  = 0;

    size = our_array_compute_size(max_cnt, data_size);

    if (mem == NULL     || 
        mem_size < size ||
        max_cnt == 0    ||
        data_size == 0) {
        return NULL;
    }

    if (data_size != array->data_size || max_cnt != array->total) {
        array->data_size = data_size;
        array->total     = max_cnt;

        our_array_reset(array);
    }

    return (our_array_t *)mem;
}

void* our_array_find(our_array_t *array, void *data, cb_equal eq)
{
    register uint32_t  idx  = 0;
    our_array_item_t  *item = NULL;

    if (unlikely(!array || !data || !eq)) {
        return NULL;
    }

    for (idx = 0; idx < array->cnt_used; idx++) {
        item = array->items + idx * (array->data_size + sizeof(our_array_item_t));
        if (eq(item->data, data)) {
            return item->data;
        }
    }

    return NULL;
}

void* our_array_insert(our_array_t *array, void *data)
{
    our_array_item_t *item = NULL;

    if (!array || array->total == 0 || !data) {
        return NULL;
    }

    if (array->cnt_used >= array->total) {
        return NULL;
    }

    item = __get_item(array, array->idle_idx);
    memcpy(item, data, array->data_size);

    ++array->cnt_used;
    --array->cnt_idle;
    return item;
}

void our_array_show(our_array_t *array, void cb(void *))
{
    uint32_t i;
    void *item = NULL;

    printf("shared array addr: %p\n", array);
    printf("total: %u, used: %u, idle: %u, data size: %u\n", 
           array->total, array->cnt_used, array->cnt_idle, array->data_size);
    printf("\n");
    for (i = 0; i < array->cnt_used; i++) {
        item = array->items + i * array->data_size;
        printf("idx: %d, ", i);
        cb(item);
    } 
}

void our_array_travel(our_array_t *array, void cb(void *, void *, void *), void *arg1, void *arg2)
{
    uint32_t i;
    void *item = NULL;

    for (i = 0; i < array->cnt_used; i++) {
        item = array->items + i * array->data_size;
        cb(item, arg1, arg2);
    } 
}
