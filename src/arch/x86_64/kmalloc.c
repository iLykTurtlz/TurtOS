#include "kmalloc.h"
#include "phys_memory.h"
#include "virt_memory.h"


//for tests only
#include "string.h"
#include "kprint.h"

#define NB_POOLS 6
#define REFILL_NUMBER 100
// 32, 64, 128, 512, 1024, and 2048

struct KmallocPool
{
    int max_size;
    int avail;
    struct FreeList *head;
};

struct FreeList
{
    struct FreeList *next;
};

struct KmallocExtra
{
    struct KmallocPool *pool;
    size_t size;
};

static const size_t block_sizes[NB_POOLS] = {32, 64, 128, 512, 1024, 2048};
static struct KmallocPool pools[NB_POOLS];

void fill_pool(struct KmallocPool *pool, size_t min_nb_blocks)
{
    int min_size = min_nb_blocks * (pool->max_size + sizeof(struct KmallocExtra));
    int actual_size;
    if (min_size % PAGE_FRAME_SIZE != 0)
    {
        actual_size = min_size - (min_size % PAGE_FRAME_SIZE) + PAGE_FRAME_SIZE;
    }
    else
    {
        actual_size = min_size;
    }
    void *pages = MMU_alloc_pages(actual_size / PAGE_FRAME_SIZE); // should I store this somewhere?
    if (pages == KERNEL_NULL)
    {
        // no kprint yet.
        __asm__("hlt");
    }
    int nb_blocks = actual_size / (pool->max_size + sizeof(struct KmallocExtra));
    uint8_t *curr = pages;
    for (int i = 0; i < nb_blocks; i++)
    {
        struct FreeList *node = (struct FreeList *)curr;
        node->next = pool->head;
        pool->head = node;
        curr += (pool->max_size + sizeof(struct KmallocExtra));
    }
    pool->avail += nb_blocks;
}

void kmalloc_init(void)
{
    // CAN ADJUST THIS
    const size_t min_nb_blocks[NB_POOLS] = {100, 100, 100, 100, 100, 100};
    for (size_t i = 0; i < NB_POOLS; i++)
    {
        pools[i].max_size = block_sizes[i];
        pools[i].avail = 0;
        pools[i].head = KERNEL_NULL;
        fill_pool(&pools[i], min_nb_blocks[i]);
    }
}

void kfree(void *addr)
{
    struct KmallocExtra *header = addr;
    header--;
    if (header->pool != KERNEL_NULL)
    {
        struct KmallocPool *pool = header->pool;
        struct FreeList *node = (struct FreeList *)header;
        node->next = pool->head;
        pool->head = node;
        pool->avail++;
    }
    else
    {
        size_t actual_size = header->size + sizeof(struct KmallocExtra);
        int nb_pages = actual_size / PAGE_FRAME_SIZE;
        if (actual_size % PAGE_FRAME_SIZE != 0)
        {
            nb_pages++;
        }
        MMU_free_pages(header, nb_pages);
    }
}

void *kmalloc(size_t size)
{
    struct KmallocPool *pool = KERNEL_NULL;
    for (int i = 0; i < NB_POOLS; i++)
    {
        if (pools[i].max_size >= size)
        {
            pool = &pools[i];
            break;
        }
    }
    if (pool == KERNEL_NULL)
    {
        size_t actual_size = size + sizeof(struct KmallocExtra);
        size_t nb_pages = actual_size / PAGE_FRAME_SIZE;
        if (actual_size % PAGE_FRAME_SIZE != 0)
        {
            nb_pages++;
        }
        void *pages = MMU_alloc_pages(nb_pages);
        struct KmallocExtra *res = pages;
        res->pool = KERNEL_NULL; // no pool
        res->size = size;
        return res + 1;
    }
    else
    {
        if (pool->avail == 0)
        {
            fill_pool(pool, REFILL_NUMBER);
        }
        struct FreeList *block = pool->head;
        pool->head = block->next;
        pool->avail--;
        struct KmallocExtra *res = (struct KmallocExtra *)block;
        res->pool = pool;
        res->size = size;
        return res + 1;
    }
    return KERNEL_NULL;
}

void pool_display(size_t pool_idx, int n)
{
    struct KmallocPool pool = pools[pool_idx];
    struct FreeList *curr = pool.head;
    for (int i=0; i<n && i<pool.avail; i++) {
        if (curr == KERNEL_NULL) {
            kprintf("ERROR: pool %ld contains fewer blocks that pool.avail\n", pool_idx);
            break;
        }
        kprintf("%p\n", curr);
        curr = curr->next;
    }
}

void *pool_peek(size_t pool_idx) {
    struct KmallocPool pool = pools[pool_idx];
    return (void *)pool.head;
}

void test_kmalloc(void) {
    const int nb_allocs = 200; // enough to require re-filling the pool
    void *allocs[nb_allocs]; 
    for (int i=0; i<NB_POOLS; i++) {
        size_t alloc_size = block_sizes[i];
        for (int j=0; j<nb_allocs; j++) {
            // allocate the full size of the block or maybe a little smaller
            if (j & 1)
                allocs[j] = kmalloc(alloc_size - 1);
            else
                allocs[j] = kmalloc(alloc_size);

            //make sure it came from the right pool and has the right size
            struct KmallocExtra *header = allocs[j];
            header--;
            if (header->pool != &pools[i]) {
                kprintf("alloc of size <= %ld should have come from pool @%p, but came from pool @%p\n", alloc_size, &pools[i], header->pool);
            }
            else if ((j & 1) && header->size != alloc_size - 1) {
                kprintf("alloc of size %ld registered as alloc of size %ld\n", alloc_size - 1, header->size);
            }
            else if (!(j & 1) && header->size != alloc_size) {
                kprintf("alloc of size %ld registered as alloc of size %ld\n", alloc_size, header->size);
            }

            // readable?  writeable?
            char *msg = "Yo!";
            char *field = allocs[j];
            memcpy(field, msg, strlen(msg) + 1);
            if (strcmp(msg, field)) {
                kprintf("Problem reading and writing to %p, an alloc of size %ld\n", allocs[j], alloc_size);
                kprintf("\tfield=%s\n", field);
            }

            // write the whole thing and check it
            uint32_t magic_num = 0x12345678;
            uint32_t *block = allocs[j];
            for (int k=0; k<alloc_size / sizeof(uint32_t); k++) {
                block[k] = magic_num;
            }
            for (int k=0; k<alloc_size / sizeof(uint32_t); k++) {
                if (block[k] != magic_num) {
                    kprintf("Unexpected uint32_t value at idx %d in block nb %d of size %ld",k, j, alloc_size);
                }
            }
        }
        // free them all and confirm they ended up at the top of the pool
        for (int j=0; j<nb_allocs; j++) {
            kfree(allocs[j]);
            if (pool_peek(i) != (void *)((size_t)allocs[j] - sizeof(struct KmallocExtra))) {
                kprintf("Problem freeing from pool idx %d: address free'd=%p, but top of pool=%p\n", i, allocs[j], pool_peek(i));
            }
        }
    }
    // confirm pool capacity:
    for (int i=0; i<NB_POOLS; i++) {
        kprintf("Pool %d has max_size %d and avail %d\n", i, pools[i].max_size, pools[i].avail);
    }

    //big alloc
    const size_t big_block_size = 4096 * 3 + 64; //four consecutive blocks needed
    void *big_block = kmalloc(big_block_size);

    // write the whole thing and check it
    uint32_t magic_num = 0x12345678;
    uint32_t *field = big_block;
    for (int k=0; k<big_block_size / sizeof(uint32_t); k++) {
        field[k] = magic_num;
    }
    for (int k=0; k<big_block_size / sizeof(uint32_t); k++) {
        if (field[k] != magic_num) {
            kprintf("Unexpected uint32_t value at idx %d in big block of size %ld",k, big_block_size);
        }
    }
    kfree(big_block);
    union vaddr address;
    address.value = (uint64_t)big_block;
    void *phys_address = vaddr_to_addr(address);
    kprintf("Big block at virt@%p - phys@%p\n", (void *)address.value, phys_address);
    freelist_peek(4);
    
    kprintf("END of kmalloc test\n");
}
