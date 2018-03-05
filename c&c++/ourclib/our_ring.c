#include "our_common.h"
#include "our_ring.h"

uint32_t our_ring_compute_size(uint32_t max_cnt, uint32_t item_size)
{
    return sizeof(our_ring_t) + item_size * max_cnt;
}

our_ring_t* our_ring_create(void *cache_start, uint32_t cache_size, uint32_t max_cnt, uint32_t item_size)
{
    our_ring_t *ring      = start;

    if (cache_start == NULL              || 
        cache_size <= sizeof(our_ring_t) || 
        max_cnt == 0                     ||
        item_size == 0) {
        return NULL;
    }

    if (cache_size < our_ring_compute_size(max_cnt, item_size)) {
        return NULL;
    }
    
    if (ring->cache_size == cache_size &&
        ring->total_cnt  == max_cnt  &&
        ring->item_size  == item_size) {
        return (our_ring_t *)cache_start;
    }

    ring->cache_size = cache_size;
    ring->max_cnt    = max_cnt;
    ring->item_size  = item_size;
    ring->used_cnt   = 0;
    ring->rd_cnt     = 0;
    ring->drop_cnt   = 0;
    ring->rd_idx     = 0;
    ring->wr_idx     = 0;
    memset(ring->queue, 0, item_size * max_cnt);
    
    return (our_ring_t *)cache_start;
}

int our_ring_put(void *handle, char* data, int len)
{
    our_ring_t      *ring = handle;
    our_ring_data_t *item = NULL;

    if (handle == NULL || data == NULL || len <= 0 || len > (int)ring->item_size) {
        return -1;
    }

    if ((ring->wr_idx + 1 == ring->rd_idx) ||
        ((ring->wr_idx == ring->total_cnt - 1) && (ring->rd_idx == 0)))
    {
        ring->drop_cnt++;
        return -1;
    }

    item = (our_ring_data_t *)(ring->queue + sizeof(our_ring_data_t) * ring->item_size * ring->rd_idx);
    memcpy(item->data, data, len);
    item->len = len;

    CVMX_SYNCW;  /* Make sure message is visible before wr_idx update */
    ring->wr_idx = (ring->wr_idx + 1) % ring->total_cnt;

    return 0;
}

char *our_ring_read(void *handle, int *len)
{
    our_ring_t      *ring = handle;
    our_ring_data_t *item = NULL;

    if (handle == NULL || len == NULL) {
        return NULL;
    }

    if (ring->rd_idx == ring->wr_idx) {
        return NULL;
    }

    item = (our_ring_data_t *)(ring->queue + sizeof(our_ring_data_t) * ring->item_size * ring->rd_idx);
    
    ring->rd_cnt++;
    *len = item->len;
    return (const char *)item->data;
}

int our_ring_read_discard(void *handle)
{
    our_ring_t *ring = handle;

    if (handle == NULL) {
        return -1;
    }

    ring->rd_idx = (ring->rd_idx + 1) % ring->total_cnt;

    return 0;
}
