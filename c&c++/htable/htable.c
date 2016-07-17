#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"

inline int likely(int expr)
{
#ifdef __GNUC__
    return __builtin_expect(expr, 1);
#else
    return expr;
#endif
}

inline int unlikely(int expr)
{
#ifdef __GNUC__
    return __builtin_expect(expr, 0);
#else
    return expr;
#endif
}

static inline uint32_t __htable_factor(uint32_t max)
{
    uint32_t i = 0;
    uint32_t value = 0;

    do
    {
        value = (uint32_t)sqrt(max);
        for (i = 2; i <= value; i++) {
            if (max % i == 0) {
                break;
            }
        }
        if (i > value) {
            return max;
        }
    }while (--max);

    return 0;
}

void htable_destory(htable_t *table)
{
    htable_item_t *item;
    htable_item_t *_item;
    
    if (NULL == table) {
        return;
    }

    for (item = table->freed; item != NULL;) {
        _item = item;
        item = item->next;
        free(_item);
    }
    
    for (item = table->used_head; item != NULL;) {
        _item = item;
        item = item->next;
        free(_item);
    }

    free(table);
}

htable_t* htable_create(uint32_t payload_size, uint32_t cnt_used_max, 
                        uint32_t cnt_idle_init, uint32_t cnt_idle_max, 
                        uint32_t cnt_timeout, uint32_t (*hash)(const void*),
                        int (*equal)(const void*, const void*))
{
    uint32_t       cnt_hash = __htable_factor(cnt_used_max * HTABLE_FACTOR);
    htable_t      *table   = NULL;
    htable_item_t *item     = NULL;
    int            i        = 0;

    // alloc hash table struct 
    if (NULL == (table = (htable_t *)calloc(1, sizeof(htable_t) + cnt_hash * sizeof(htable_item_t *)))) {
        return NULL;
    }
    
    // init freed list
    for (i = 0; i < cnt_idle_init; i++) {
        if (NULL == (item = (htable_item_t *)calloc(1, sizeof(htable_item_t) + payload_size))) {
            goto err;
        }
        if (table->freed != NULL) {
            table->freed->prev = item;
        }
        item->next = table->freed;
        item->prev = NULL;
        table->freed = item;
    }

    table->payload_size = payload_size;
    table->cnt_used_max = cnt_used_max;
    table->cnt_idle_max = cnt_idle_max;
    table->cnt_hash     = cnt_hash;
    table->cnt_used     = 0;
    table->cnt_idle     = cnt_idle_init;
    table->cnt_conflict = 0;
    table->hash         = hash;
    table->equal        = equal;
    table->cnt_timeout  = cnt_timeout;

    return table;

  err:
    htable_destory(table);
    return NULL;
}

htable_item_t* htable_find(htable_t *table, void *key, void *payload)
{
    htable_item_t *item = NULL;
    uint32_t       idx  = 0;

    if (unlikely(table == NULL || key == NULL || payload == NULL)) {
        return NULL;
    }

    idx = table->hash(key) % table->cnt_hash;
    for (item = table->heads[idx]; item != NULL; item = item->next)
    {
        if (likely(table->equal(item->payload, payload))) 
        {
            // update used list 
            if (likely(item->prev1 != NULL)) {
                if (likely(item->next1 != NULL)) {
                    item->next1->prev1 = item->prev1;
                } else {
                    table->used_tail = item->prev1;
                }
                item->prev1->next1 = item->next1;
                item->next1 = table->used_head;
                item->prev1 = NULL;
                table->used_head->prev1 = item;
                table->used_head = item;
            }
            item->last_read = table->now;

            return item;
        }
    }

    return NULL;
}

htable_item_t* htable_insert(htable_t *table, void *key, void *payload)
{
    htable_item_t *item = NULL;
    uint32_t       idx  = 0;

    if (unlikely(table == NULL || key == NULL || payload == NULL)) {
        return NULL;
    }

    // used count limit check
    if (table->cnt_used >= table->cnt_used_max) {
        table->cnt_exceed_err++;
        return NULL;
    }

    // get one free item
    if (table->freed == NULL) {
        if (NULL == (item = (htable_item_t *)calloc(1, sizeof(htable_item_t) + table->payload_size))) {
            return NULL;
        }
        table->cnt_alloc++;
    } else {
        item = table->freed;
        table->freed = table->freed->next;
        table->cnt_idle--;
    }

    // set used list for timeout checking
    item->last_read = table->now;
    item->next1 = table->used_head;
    item->prev1 = NULL;
    if (table->used_head != NULL) {
        table->used_head->prev1 = item;
    } else {
        table->used_tail = item;
    }
    table->used_head = item;

    // set hash table list
    idx = table->hash(key) % table->cnt_hash;
    item->idx  = idx;
    item->next = table->heads[idx];
    item->prev = NULL;
    if (table->heads[idx] != NULL) {
        table->heads[idx]->prev = item;
        table->cnt_conflict++;
    }
    table->heads[idx] = item;

    table->cnt_used++;
    memcpy(item->payload, payload, table->payload_size);

    return item;
}

static inline void _htable_remove(htable_t *table, htable_item_t *item)
{
    // update hash table
    if (item->next != NULL || item->prev != NULL) {
        table->cnt_conflict--;
    }
    if (item->next != NULL) {
        item->next->prev = item->prev;
    }
    if (item->prev != NULL) {
        item->prev->next = item->next;
    } else {
        table->heads[item->idx] = item->next;
    }
        
    // update used list for timeout checking
    if (item->next1 != NULL) {
        item->next1->prev1 = item->prev1;
    } else {
        table->used_tail = item->prev1;
    }
    if (item->prev1 != NULL) {
        item->prev1->next1 = item->next1;
    } else {
        table->used_head = item->next1;
    }

    // release item or insert item to freed list 
    table->cnt_used--;
    if (table->cnt_idle >= table->cnt_idle_max) {
        free(item);
    } else {
        item->next = table->freed;
        table->freed = item;
        table->cnt_idle++;
    }
}


int htable_remove(htable_t *table, htable_item_t *item)
{
    if (unlikely(table == NULL || item == NULL)) {
        return -1;
    }
    
    _htable_remove(table, item);
    return 0;
}

void htable_remove_timeout(htable_t *table)
{
    htable_item_t *item;

    if (table == NULL || table->cnt_timeout == 0) {
        return;
    }

    while (NULL != (item = table->used_tail))
    {
        // remain items are not timeout
        if (table->now < item->last_read + table->cnt_timeout) {
            break;
        }

        _htable_remove(table, item);
    }
}

void htable_update_now(htable_t *table, time_t now)
{
    table->now = now;
}

void htable_print_stat(htable_t *table)
{
    printf("htable stat. used: %u, idle: %u, conflict: %u, exceed_err: %u, alloc: %u\n", 
           table->cnt_used, table->cnt_idle, table->cnt_conflict, table->cnt_exceed_err, table->cnt_alloc);
    table->cnt_alloc = 0;
    table->cnt_exceed_err = 0;
}
