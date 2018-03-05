#ifndef _OUR_RING_H_
#define _OUR_RING_H_

#pragma pack(8)
typedef struct our_ring_data
{
    uint32_t    len;
    uint8_t     data[0];
} our_ring_data_t;

typedef struct our_ring
{
             uint32_t total_cnt;
             uint32_t item_size;
    volatile uint32_t used_cnt;
    volatile uint64_t rd_cnt;
    volatile uint64_t drop_cnt;
    volatile uint32_t rd_idx;
    volatile uint32_t wr_idx;
    
    uint8_t           queue[0];
} our_ring_t;
#pragma pack()

uint32_t our_ring_compute_size(uint32_t max_cnt, uint32_t item_size);
our_ring_t* our_ring_create(void *cache_start, uint32_t cache_size, uint32_t max_cnt, uint32_t item_size);
int our_ring_put(void *handle, char* data, int len);
char *our_ring_read(void *handle, int *len);
int our_ring_read_discard(void *handle);

#endif
