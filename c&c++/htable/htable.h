#ifndef _HTABLE_H_
#define _HTABLE_H_

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define HTABLE_FACTOR 7

typedef struct _htable_item {
    struct _htable_item* next;
    struct _htable_item* prev;
    struct _htable_item* next1;       // for timeout check
    struct _htable_item* prev1;       // for timeout check
    time_t               last_read;   // last read time
    uint32_t             idx;         // hash table idx
    char                 payload[0];  // payload
} htable_item_t;

typedef struct {
    uint32_t       payload_size;     // payload size
    
    uint32_t       cnt_used_max;     // max of used item count
    uint32_t       cnt_idle_max;     // max idle item count
    uint32_t       cnt_hash;         // hash domain size
    uint32_t       cnt_used;         // used item count
    uint32_t       cnt_idle;         // idle item count
    uint32_t       cnt_conflict;     // conflict item count
    
    uint32_t       cnt_exceed_err;   // cnt_used exceed cnt_used_max 
    uint32_t       cnt_alloc;        // alloc count

    uint32_t       cnt_timeout;      // how many seconds timeout, 0 mean not check timeout
    time_t         now;              // current time
    
    uint32_t (*hash)(const void*);   // hash function
    int (*equal)(const void*, const void*);  // item equal function

    htable_item_t *used_head;        // for timeout check, insert from this
    htable_item_t *used_tail;        // for timeout check, remove from this
    
    htable_item_t *freed;            // freed list (singly linked list)
    htable_item_t *heads[0];         // hash table array
} htable_t;

htable_t* htable_create(uint32_t payload_size, uint32_t cnt_used_max, 
                        uint32_t cnt_idle_init, uint32_t cnt_idle_max, 
                        uint32_t cnt_timeout, uint32_t (*hash)(const void*),
                        int (*equal)(const void*, const void*));

void htable_destory(htable_t *table);

htable_item_t* htable_find(htable_t *table, void *key, void *payload);

htable_item_t* htable_insert(htable_t *table, void *key, void *payload);

int htable_remove(htable_t *table, htable_item_t *item);

void htable_remove_timeout(htable_t *table);

void htable_update_now(htable_t *table, time_t now);

void htable_print_stat(htable_t *table);

#ifdef __cplusplus
};
#endif

#endif

