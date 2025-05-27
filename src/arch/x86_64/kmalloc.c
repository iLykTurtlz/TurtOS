#include "kmalloc.h"


struct KmallocPool {
   int max_size;
   int avail;
   struct FreeList *head;
};

struct FreeList {
   struct FreeList *next;
};

struct KmallocExtra {
   struct KmallocPool *pool;
   size_t size;
};