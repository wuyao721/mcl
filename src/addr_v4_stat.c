#include "mcl.h"

typedef struct addr_v4_stat {
    uint32_t addr;
    uint32_t cnt;
} addr_v4_stat_t;

uint32_t addr_v4_stat_hash(const void* key)
{
    return ((addr_v4_stat_t *)key)->addr;
}

int addr_v4_stat_equal(const void *key1, const void *key2)
{
    return ((addr_v4_stat_t *)key1)->addr == ((addr_v4_stat_t *)key2)->addr;
}

int test_mcl_array_hash()
{
    uint32_t size  = 0;
    void    *start = NULL;
    addr_v4_stat_t *p = NULL;
    mcl_array_hash_iter_t iter;
    addr_v4_stat_t stat = {1, 0};

    size  = mcl_array_hash_compute_size(sizeof(addr_v4_stat_t), 3 * 1000 * 1000, 0.2);
    start = calloc(1, size);

    if (start == NULL) {
        return -1;
    }

    if (0 != mcl_array_hash_create(start, size, sizeof(addr_v4_stat_t), 3 * 1000 * 1000, 0.2)) {
        return -1;
    }

    if (NULL == (p = mcl_array_hash_insert((mcl_array_hash_t *)start, &stat, &stat, addr_v4_stat_hash))) {
        ;// TODO
    } else {
        p->cnt++;
    }

    if (NULL == (p = mcl_array_hash_insert((mcl_array_hash_t *)start, &stat, &stat, addr_v4_stat_hash))) {
        ;// TODO
    } else {
        p->cnt++;
    }

    p = mcl_array_hash_get_first((mcl_array_hash_t *)start, &iter);
    while (p != NULL) {
        printf("addr: %u, cnt: %u\n", p->addr, p->cnt);
        p = mcl_array_hash_get_next(&iter);
    }

    if (NULL != (p = mcl_array_hash_find((mcl_array_hash_t *)start, &stat, addr_v4_stat_hash, addr_v4_stat_equal))) {
        printf("found. addr: %u, cnt: %u\n", p->addr, p->cnt);
    } else {
        printf("not found.\n");
    }

    return 0;
}

int main()
{
    if (test_mcl_array_hash()) {;}
    return 0;
}
