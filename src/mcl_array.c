#include "mcl_common.h"
#include "mcl_array.h"

static inline mcl_array_item_t* __get_item(mcl_array_t *array, uint32_t idx)
{
    return (mcl_array_item_t *)(array->items + (sizeof(mcl_array_item_t) + array->data_size) * idx);
}

uint32_t mcl_array_compute_size(uint32_t max_cnt, uint32_t data_size)
{
    return sizeof(mcl_array_t) + max_cnt * (data_size + sizeof(mcl_array_item_t));
}

void mcl_array_reset(mcl_array_t *array)
{
    int size = array->total * (array->data_size + sizeof(mcl_array_item_t));

    array->idle_idx = 0;
    array->cnt_used = 0;
    array->cnt_idle = array->total;
    memset(array->items, 0, size);
}

mcl_array_t* mcl_array_create(void *mem, uint32_t mem_size, uint32_t max_cnt, uint32_t data_size)
{
    mcl_array_t   *array = (mcl_array_t *)mem;
    uint32_t       size  = 0;

    size = mcl_array_compute_size(max_cnt, data_size);

    if (mem == NULL     || 
        mem_size < size ||
        max_cnt == 0    ||
        data_size == 0) {
        return NULL;
    }

    if (data_size != array->data_size || max_cnt != array->total) {
        array->data_size = data_size;
        array->total     = max_cnt;

        mcl_array_reset(array);
    }

    return (mcl_array_t *)mem;
}

void* mcl_array_find(mcl_array_t *array, void *data, cb_equal eq)
{
    register uint32_t  idx  = 0;
    mcl_array_item_t  *item = NULL;

    if (unlikely(!array || !data || !eq)) {
        return NULL;
    }

    for (idx = 0; idx < array->cnt_used; idx++) {
        item = array->items + idx * (array->data_size + sizeof(mcl_array_item_t));
        if (eq(item->data, data)) {
            return item->data;
        }
    }

    return NULL;
}

void* mcl_array_insert(mcl_array_t *array, void *data)
{
    mcl_array_item_t *item = NULL;

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

void mcl_array_show(mcl_array_t *array, void cb(void *))
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

void mcl_array_travel(mcl_array_t *array, void cb(void *, void *, void *), void *arg1, void *arg2)
{
    uint32_t i;
    void *item = NULL;

    for (i = 0; i < array->cnt_used; i++) {
        item = array->items + i * array->data_size;
        cb(item, arg1, arg2);
    } 
}
