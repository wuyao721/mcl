// gcc main.c htable.c -lm -g -Wall -O2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "htable.h"

uint32_t ip_key(const void* key)
{
    return *(uint32_t *)key;
}

int ip_cmp(const void* value1, const void* value2)
{
    return *(uint32_t *)value1 == *(uint32_t *)value2;
}

int main()
{
    htable_t *table = NULL;
    int       i     = 0;
    int       j     = 0;
    uint32_t  ip    = 0;
    
    if (NULL == (table = htable_create(4, 3000 * 1000, 1000, 22000, 60, ip_key, ip_cmp))) {
        return -1;
    }

    srand(time(NULL));

    while (1)
    {
        htable_update_now(table, time(NULL));
        for (i = 0; i < 10000; i++) {
            ip = (uint32_t)rand();
            if (NULL == htable_find(table, &ip, &ip)) {
                htable_insert(table, &ip, &ip);
            }
        }
        htable_remove_timeout(table);
        if ((j++ % 20) == 0) {
            htable_print_stat(table);
        }
            
        sleep(1);
    }

    htable_destory(table);
    return 0;
}
