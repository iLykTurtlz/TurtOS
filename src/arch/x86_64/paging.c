#include "paging.h"
#include "mmu.h"
#include "string.h"
#include "kprint.h"
#include "irq.h"

#define PML4_SIZE 8


#define PHYS_ADDR_MAX 0x7fe0000
#define NODE_CAPACITY ((sizeof(struct page_frame))/(sizeof(uint64_t)))   

#define ONE_TO_ONE_MAPPING_BASE_ADDR 0x0000000000
#define KERNEL_HEAP_BASE_ADDR 0x8000000000
#define KERNEL_STACKS_BASE_ADDR 0x78000000000
#define USER_SPACE_BASE_ADDR 0x80000000000

#define ONE_TO_ONE_MAPPING_L4_IDX 0
#define KERNEL_HEAP_L4_IDX 1
#define KERNEL_STACKS_L4_IDX 15
#define USER_SPACE_L4_IDX 16




struct cr3_val {
    uint64_t reserved0 : 3;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t reserved1: 7;
    uint64_t L4_addr: 40;
    uint64_t reserved2: 12;
}__attribute__((packed));

struct generic_entry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t reserved1 : 3;
    uint64_t AVL : 3;
    uint64_t addr : 40;
    uint64_t reserved2 : 12;
}__attribute__((packed));


struct L4_entry {
    uint64_t P : 1; //0 if not using
    uint64_t RW : 1; //1 writeable
    uint64_t US : 1;  //0
    uint64_t PWT : 1; //
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t IGN : 1;
    uint64_t MBZ : 2;
    uint64_t AVL : 3;
    uint64_t L3_addr : 40;
    uint64_t available : 11;
    uint64_t NX : 1; 
}__attribute__((packed));

struct L3_entry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t IGN : 1;
    uint64_t zero : 1;
    uint64_t MBZ : 1;
    uint64_t AVL : 3;
    uint64_t L2_addr : 40;
    uint64_t available : 11;
    uint64_t NX : 1;
}__attribute__((packed));

struct L2_entry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t IGN0 : 1;
    uint64_t zero : 1;
    uint64_t IGN1 : 1;
    uint64_t AVL : 3;
    uint64_t L1_addr : 40;
    uint64_t available : 11;
    uint64_t NX : 1;
}__attribute__((packed));

// L2 needs a slightly different page for the 2M huge page identity map
struct L2_huge_entry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t D : 1;
    uint64_t one : 1;
    uint64_t G : 1;
    uint64_t AVL : 3;
    uint64_t PAT : 1;
    uint64_t MBZ : 8;
    uint64_t phys_addr : 31;
    uint64_t available : 11;
    uint64_t NX : 1;
};

struct L1_entry {
    uint64_t P : 1;
    uint64_t RW : 1;
    uint64_t US : 1;
    uint64_t PWT : 1;
    uint64_t PCD : 1;
    uint64_t A : 1;
    uint64_t D : 1;
    uint64_t PAT : 1;
    uint64_t G : 1;
    uint64_t AVL : 3;
    uint64_t phys_addr : 40;
    uint64_t available : 11;
    uint64_t NX : 1;
}__attribute__((packed));

static struct {
    struct L4_entry *root;
    union vaddr kernel_heap;
    union vaddr kernel_stacks;
    union vaddr user_space;
} next_alloc;




uint64_t test_page_table(uint64_t vaddr) {
    struct virtual_address *addr = (struct virtual_address *)&vaddr;
    struct L4_entry *root;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(root));
    uint64_t l3_addr = (root[addr->L4_offset].L3_addr << 12);
    struct L3_entry *pdp = (struct L3_entry *)l3_addr;
    uint64_t l2_addr = (pdp[addr->L3_offset].L2_addr << 12);
    struct L2_entry *pd = (struct L2_entry *)l2_addr;
    uint64_t l1_addr = (pd[addr->L2_offset].L1_addr << 12);
    struct L1_entry *pt = (struct L1_entry *)l1_addr;
    uint64_t pf_addr = pt[addr->L1_offset].phys_addr << 12;
    uint64_t res = pf_addr + addr->phys_offset;
    return res;
}





// int walk(void *vaddr, struct generic_entry *entry, int level, int num, int allocate_virt, int allocate_phys, int rw, int us) {
//     // union vaddr res;
//     // memcpy(&res, '\0', sizeof(union vaddr));
//     // struct L3_entry *l3_start = (struct L3_entry *)(next_alloc.root[next_alloc.kernel_heap.L4_offset].L3_addr << 12);
//     // struct L3_entry *l3_current = l3_start + next_alloc.kernel_heap.L3_offset;
//     // struct L2_entry *l2_start = (struct L2_entry *)((l3_current->L2_addr) << 12);
//     // struct L2_entry *l2_current = l2_start + next_alloc.kernel_heap.L2_offset;
//     // struct L1_entry *l1_start = (struct L1_entry *)((l2_current->L1_addr) << 12);
//     // struct L1_entry *l1_current = l1_start + next_alloc.kernel_heap.L1_offset;

//     int top_level = level;
//     union vaddr current;
//     current.value = (uint64_t)vaddr;
//     struct generic_entry *path[5];
//     path[level] = entry;
//     while (level > 0) {
//         uint16_t offset = 0;
//         switch (level) {
//             case 0:
//                 current.addr.phys_offset;
//                 break;
//             case 1:
//                 offset = current.addr.L1_offset;
//                 break;
//             case 2:
//                 offset = current.addr.L2_offset;
//                 break;
//             case 3:
//                 offset = current.addr.L3_offset;
//                 break;
//             case 4:
//                 offset = current.addr.L4_offset;
//                 break;
//             default:
//                 kprintf("ERROR: walk level = %d\n", level);
//                 __asm__ volatile("hlt");
//         }
//         if (path[level][offset].P == 0) {
//             if (allocate_virt) {
//                 struct generic_entry *new_node = MMU_pf_alloc();
//                 path[level][offset].addr = (uint64_t)new_node >> 12;
//                 path[level][offset].RW = rw;
//                 path[level][offset].US = us;
//                 path[level][offset].P = 1;
                
//             } else {
//                 // no way down
//                 return 0;
//             }
//         }
//         path[level-1] = (struct generic_entry *)(path[level][offset].addr << 12);
//         level--;
//     }
//     if (allocate_phys && path[0][current.addr.phys_offset].P == 0 && path[0][current.addr.phys_offset].AVL == 1) {
//         struct page_frame *pf = MMU_pf_alloc();
//         path[0][current.addr.phys_offset].addr = (uint64_t)pf >> 12;
//         path[0][current.addr.phys_offset].RW = rw;
//         path[0][current.addr.phys_offset].US = us;
//         path[0][current.addr.phys_offset].P = 1;
//         return 1;
//     }
//     else if (allocate_phys && path[0][current.addr.phys_offset].P == 0 && path[0][current.addr.phys_offset].AVL == 0) {
//         // cannot fill page frame
//         return 0;
//     }

//     int found=0;



// }

uint16_t level_offset(union vaddr current, int level) {
    uint16_t offset = 0;
    switch (level) {
        case 0:
            offset = current.addr.phys_offset;
            break;
        case 1:
            offset = current.addr.L1_offset;
            break;
        case 2:
            offset = current.addr.L2_offset;
            break;
        case 3:
            offset = current.addr.L3_offset;
            break;
        case 4:
            offset = current.addr.L4_offset;
            break;
        default:
            kprintf("ERROR: walk level = %d\n", level);
            __asm__ volatile("hlt");
    }
    return offset;
}


struct L1_entry *walk(void *vaddr, struct generic_entry *entry, int level, int allocate, int rw, int us) {
    // int top_level = level;
    union vaddr current;
    current.value = (uint64_t)vaddr;
    struct generic_entry *path[5];
    path[level] = entry;
    while (level > 1) {
        uint16_t offset = level_offset(current, level);
        if (path[level][offset].P == 0) {
            if (allocate) {
                struct generic_entry *new_node = MMU_pf_alloc();
                if (new_node == (void *)MMU_NULL) {
                    //nothing left to allocate
                    kprintf("NO MEMORY LEFT!\n");
                    return (void *)KERNEL_NULL;
                }
                memset(new_node, '\0', sizeof(struct generic_entry));
                path[level][offset].addr = (uint64_t)new_node >> 12;
                path[level][offset].RW = rw;
                path[level][offset].US = us;
                path[level][offset].P = 1;
            } else {
                // no way down
                return (void *)KERNEL_NULL; //NULL
            }
        }
        path[level-1] = (struct generic_entry *)((uint64_t)(path[level][offset].addr << 12));
        level--;
    }
    return (struct L1_entry *)&path[1][current.addr.L1_offset];
    // if (allocate_phys && path[0][current.addr.phys_offset].P == 0 && path[0][current.addr.phys_offset].AVL == 1) {
    //     struct page_frame *pf = MMU_pf_alloc();
    //     path[0][current.addr.phys_offset].addr = (uint64_t)pf >> 12;
    //     path[0][current.addr.phys_offset].RW = rw;
    //     path[0][current.addr.phys_offset].US = us;
    //     path[0][current.addr.phys_offset].P = 1;
    //     return 1;
    // }
    // else if (allocate_phys && path[0][current.addr.phys_offset].P == 0 && path[0][current.addr.phys_offset].AVL == 0) {
    //     // cannot fill page frame
    //     return 0;
    // }

}


void handle_page_fault(uint8_t irq, uint32_t error, void *arg) {
    void *vaddr = next_alloc.root;
    asm volatile ("mov %%cr2, %0" : "=r"(vaddr));
    // struct cr3_val val;
    // __asm__ volatile ("mov %%cr3, %0" : "=r"(val));
    // struct L4_entry *root = (struct L4_entry *)((uint64_t)(val.L4_addr << 12));
    int stop = 0;
    if (error & 1) {
        kprintf("Page protection violation caused by ");
        stop = 1;
    } 
    // else {
    //     kprintf("Non present page caused by ");
    // }
    // if (error & 2) {
    //     kprintf("a write at %lx in ", vaddr);
    // } else {
    //     kprintf("a read at %lx in ", vaddr);
    // }
    // if (error & 4) {
    //     kprintf("mode U\n");
    // } else {
    //     kprintf("mode S\n");
    // }
    if (stop) {
        __asm__ volatile("hlt");
    }
    // // TODO: check present bits on the way down
    // struct virtual_address *addr = (struct virtual_address *)&vaddr;
    // uint64_t l3_addr = (root[addr->L4_offset].L3_addr << 12);
    // struct L3_entry *pdp = (struct L3_entry *)l3_addr;
    // uint64_t l2_addr = (pdp[addr->L3_offset].L2_addr << 12);
    // struct L2_entry *pd = (struct L2_entry *)l2_addr;
    // uint64_t l1_addr = (pd[addr->L2_offset].L1_addr << 12);
    // struct L1_entry *pt = (struct L1_entry *)l1_addr;
    // struct L1_entry *leaf = &pt[addr->L1_offset];
    struct L1_entry *leaf = walk((void *)vaddr, (struct generic_entry *)next_alloc.root, 4, 0, 0, 0);
    if (leaf == (struct L1_entry *)KERNEL_NULL) {
        kprintf("No page allocated at %p\n", vaddr);
        __asm__ volatile("hlt");
    }
    if (leaf->P == 0 && leaf->AVL == 1) {
        void *ptr = MMU_pf_alloc();
        if (ptr == (void *)MMU_NULL) {
            kprintf("Page frame alloc failed in the page fault handler\n");
            __asm__ volatile("hlt");
        }
        leaf->phys_addr = (uint64_t)ptr >> 12;
        leaf->RW = 1;
        leaf->P = 1;
        leaf->AVL = 0;
    }
    else if (leaf->P == 1) {
        kprintf("Present bit: should never happen\n");
        __asm__ volatile("hlt");
    }
    else if (leaf->AVL != 1) {
        kprintf("AVL == %d\n", leaf->AVL);
        __asm__ volatile("hlt");
    }
    else {
        kprintf("Something else went wrong\n");
        __asm__ volatile("hlt");
    }
}


void MMU_init(void) {
    // TODO make this local
    struct L4_entry *root = MMU_pf_alloc();
    memset(root, '\0', sizeof(struct page_frame));

    // TODO allocate 2MB huge pages instead
    // complete one-to-one mapping subtree
    struct L3_entry *one_to_one = MMU_pf_alloc();
    memset(one_to_one, '\0', sizeof(struct page_frame));
    
    root[ONE_TO_ONE_MAPPING_L4_IDX].US = 0; //superuser
    root[ONE_TO_ONE_MAPPING_L4_IDX].P = 1; //present
    root[ONE_TO_ONE_MAPPING_L4_IDX].RW = 1;
    root[ONE_TO_ONE_MAPPING_L4_IDX].L3_addr = (uint64_t)one_to_one >> 12;

    uint64_t pf_addr = 0;
    // 2M page version
    for (size_t L3_idx=0; L3_idx<NODE_CAPACITY; L3_idx++) {
        if (pf_addr < PHYS_ADDR_MAX) {
            struct L2_huge_entry *pd = MMU_pf_alloc();
            memset(pd, '\0', sizeof(struct page_frame));
            one_to_one[L3_idx].US = 0;
            one_to_one[L3_idx].P = 1;
            one_to_one[L3_idx].RW = 1;
            one_to_one[L3_idx].L2_addr = (uint64_t)pd >> 12;
            for (size_t L2_idx=0; L2_idx<NODE_CAPACITY; L2_idx++) {
                if (pf_addr < PHYS_ADDR_MAX) {
                    // set 2M page bit (cleared by memset in the 4K version)
                    pd[L2_idx].one = 1;
                    pd[L2_idx].US = 0;
                    pd[L2_idx].P = 1;
                    pd[L2_idx].RW = 1;
                    //NOT a typo.  21 bits in the 2M L2 entry below the base address
                    pd[L2_idx].phys_addr = pf_addr >> 21; 
                    pf_addr += 0x200000; // 2M
                } else {
                    pd[L2_idx].P = 0;
                }
            }
        } else {
            one_to_one[L3_idx].P = 0;
        }
    }

    // 4K page version
    // for (size_t L2_idx=0; L2_idx<NODE_CAPACITY; L2_idx++) {
    //     // for (size_t i=0; i<100000000; i++);
    //     // kprintf("sizeof(struct page_frame): %ld\n", sizeof(struct page_frame));
    //     // for (size_t i=0; i<100000000; i++);
    //     if (pf_addr < PHYS_ADDR_MAX) {
    //         struct L2_entry *pd = MMU_pf_alloc();
    //         memset(pd, '\0', sizeof(struct page_frame));
    //         one_to_one[L2_idx].US = 0;
    //         one_to_one[L2_idx].P = 1;
    //         one_to_one[L2_idx].RW = 1;
    //         one_to_one[L2_idx].L2_addr = (uint64_t)pd >> 12;
    //         for (size_t L1_idx=0; L1_idx<NODE_CAPACITY; L1_idx++) {
    //             if (pf_addr < PHYS_ADDR_MAX) {
    //                 struct L1_entry *pt = MMU_pf_alloc();
    //                 memset(pt, '\0', sizeof(struct page_frame));
    //                 pd[L1_idx].US = 0;
    //                 pd[L1_idx].P = 1;
    //                 pd[L1_idx].RW = 1;
    //                 pd[L1_idx].L1_addr = (uint64_t)pt >> 12;
    //                 for (size_t phys_idx=0; phys_idx<NODE_CAPACITY; phys_idx++) {
    //                     if (pf_addr < PHYS_ADDR_MAX) {
    //                         pt[phys_idx].US = 0;
    //                         pt[phys_idx].P = 1;
    //                         pt[phys_idx].RW = 1;
    //                         pt[phys_idx].phys_addr =  pf_addr >> 12;
    //                         pf_addr += sizeof(struct page_frame); // 4096
    //                     } else {
    //                         // NOT present
    //                         pt[phys_idx].P = 0;
    //                     }
    //                 }
    //             } else {
    //                 // NOT present
    //                 pd[L1_idx].P = 0;
    //             }
    //         }
    //     } else {
    //         // NOT present
    //         one_to_one[L2_idx].P = 0;
    //     }
    // }

    // other entries
    root[KERNEL_HEAP_L4_IDX].US = 0;
    root[KERNEL_HEAP_L4_IDX].P = 0; //not present
    root[KERNEL_HEAP_L4_IDX].RW = 0;
    root[KERNEL_HEAP_L4_IDX].L3_addr = 0;

    // reserved/growth
    for (size_t i=KERNEL_HEAP_L4_IDX + 1; i<KERNEL_STACKS_L4_IDX; i++) {
        root[i].US = 0;
        root[i].P = 0; //not present
        root[i].RW = 0;
        root[i].L3_addr = 0;
    }

    root[KERNEL_STACKS_L4_IDX].US = 0;
    root[KERNEL_STACKS_L4_IDX].P = 0; //not present
    root[KERNEL_STACKS_L4_IDX].RW = 0;
    root[KERNEL_STACKS_L4_IDX].L3_addr = 0;

    root[USER_SPACE_L4_IDX].US = 1; //user space
    root[USER_SPACE_L4_IDX].P = 0; //not present
    root[USER_SPACE_L4_IDX].RW = 0;
    root[USER_SPACE_L4_IDX].L3_addr = 0;

    // unused entries in L4
    for (size_t i=USER_SPACE_L4_IDX + 1; i<NODE_CAPACITY; i++) {
        root[i].US = 0;
        root[i].P = 0; //not present
        root[i].RW = 0;
        root[i].L3_addr = 0;
    }

    // set cr3
    // struct cr3_val root_addr;
    // memset(&root_addr, '\0', sizeof(struct cr3_val));
    // root_addr.L4_addr = (uint64_t)root >> 12;
    // root_addr.PWT = 0; // both need to be 0??
    // root_addr.PCD = 0;
    __asm__ volatile ("mov %0, %%cr3"::"r"(root):"memory");

    IRQ_set_handler(14, handle_page_fault, NULL);


    next_alloc.kernel_heap.value = KERNEL_HEAP_BASE_ADDR;
    next_alloc.kernel_stacks.value = KERNEL_STACKS_BASE_ADDR;
    next_alloc.user_space.value = USER_SPACE_BASE_ADDR;

    next_alloc.root = root;
}





// // returns a pointer to the unused L1_entry or NULL (which is NOT a valid virtual address :))
// struct L1_entry *is_unused(struct virtual_address vaddr) {
//     struct L3_entry *l3_start = (struct L3_entry *)(next_alloc.root[vaddr.L4_offset].L3_addr << 12);
// }





// void *MMU_alloc_pages(int num) {
//     struct virtual_address res;
//     memset(&res, '\0', sizeof(struct virtual_address));
//     int nb_found = 0;
//     struct cr3_val val;
//     __asm__ volatile ("mov %%cr3, %0" : "=r"(val));
//     struct L4_entry *root = (struct L4_entry *)((uint64_t)(val.L4_addr << 12));
//     struct L3_entry *l3;
//     if (root[KERNEL_HEAP_L4_IDX].P == 0) {
//         l3 = MMU_pf_alloc();
//         memset(l3, '\0', sizeof(struct L3_entry));
//         root[KERNEL_HEAP_L4_IDX].L3_addr = (uint64_t)l3 >> 12;
//         root[KERNEL_HEAP_L4_IDX].US = 0;
//         root[KERNEL_HEAP_L4_IDX].RW = 1;
//         root[KERNEL_HEAP_L4_IDX].P = 1;
//     } else {
//         l3 = (struct L3_entry *)((uint64_t)(root[KERNEL_HEAP_L4_IDX].L3_addr << 12));
//     }
//     for (size_t L3_idx=0; L3_idx<NODE_CAPACITY; L3_idx++) {
//         struct L2_entry *l2;
//         if (l3[L3_idx].P == 0) {
//             l2 = MMU_pf_alloc();
//             memset(l2, '\0', sizeof(struct L2_entry));
//             l3[L3_idx].L2_addr = (uint64_t)l2 >> 12;
//             l3[L3_idx].US = 0;
//             l3[L3_idx].RW = 1;
//             l3[L3_idx].P = 1;
//         } else {
//             l2 = (struct L2_entry *)((uint64_t)(l3[L3_idx].L2_addr << 12));
//         }
//         for (size_t L2_idx=0; L2_idx<NODE_CAPACITY; L2_idx++) {
//             struct L1_entry *l1;
//             if (l2[L2_idx].P == 0) {
//                 l1 = MMU_pf_alloc();
//                 memset(l1, '\0', sizeof(struct L1_entry));
//                 l2[L2_idx].L1_addr = (uint64_t)l1 >> 12;
//                 l2[L2_idx].US = 0;
//                 l2[L2_idx].RW = 1;
//                 l2[L2_idx].P = 1;
//             } else {
//                 l1 = (struct L1_entry *)((uint64_t)(l2[L2_idx].L1_addr << 12));
//             }
//             for (size_t L1_idx=0; L1_idx<NODE_CAPACITY; L1_idx++) {
//                 if (l1[L1_idx].AVL == 0) {
//                     if (nb_found == 0) {
//                         res.phys_offset = 0;
//                         res.L1_offset = L1_idx;
//                         res.L2_offset = L2_idx;
//                         res.L3_offset = L3_idx;
//                         res.L4_offset = KERNEL_HEAP_L4_IDX;
//                         // sign extension
//                         if (res.L4_offset & (1 << 8))
//                             res.sign_extension = 0xffff; //????
//                     } 
//                     if (++nb_found == num) {
//                         // set AVL to 1 for the chosen addresses
//                         while (nb_found--) {
//                             l1[L1_idx--].AVL = 1;
//                         }
//                         uint64_t result;
//                         memcpy(&result, &res, sizeof(struct virtual_address));
//                         return (void *)(uintptr_t)result;
//                     }
                    
//                 } else {
//                     nb_found = 0;
//                     memset(&res, '\0', sizeof(struct virtual_address));
//                 }
//             }
//             // cannot allocate pages on separate L1 nodes (for now)
//             nb_found = 0;
//             memset(&res, '\0', sizeof(struct virtual_address));
//         }
//     }
//     kprintf("No memory addresses left!");
//     __asm__ volatile("hlt");
//     return (void *)KERNEL_NULL;
// }

void *MMU_alloc_page(void) {
    return MMU_alloc_pages(1);
}

void *MMU_alloc_pages(int num) {
    int i=0;
    union vaddr res;
    res.value = next_alloc.kernel_heap.value;
    if (res.addr.L4_offset & (1 << 8)) {
        res.addr.sign_extension = 0xffff;
        next_alloc.kernel_heap.value = res.value;
    }
    while (i<num) {
        struct L1_entry *leaf = walk((void *)res.value, (struct generic_entry *)next_alloc.root, 4, 1, 1, 0);
        if (leaf == (void *)KERNEL_NULL) {
            kprintf("No more memory in kernel heap\n");
            return leaf;
        }
        leaf->P = 0;
        leaf->AVL = 1;
        leaf->RW = 1;
        next_alloc.kernel_heap.value += PAGE_FRAME_SIZE;
        i++;
    }
    return (void *)res.value;
}

void MMU_free_page(void *ptr) {
    MMU_free_pages(ptr, 1);
}

void MMU_free_pages(void *ptr, int num) {
    union vaddr current_addr;
    // current_addr.value = (uint64_t)ptr;
    int i=0;
    while (i < num) {
        struct L1_entry *leaf = walk((void *)current_addr.value, (struct generic_entry *)next_alloc.root, 4, 0, 0, 0);
        if (leaf == (void *)KERNEL_NULL || leaf->P == 0) {
            kprintf("Tried to deallocate a non-allocated frame at %lx\n", (uint64_t)(leaf->phys_addr << 12));
            __asm__ volatile ("hlt");
        }
        leaf->P = 0;
        leaf->AVL = 0;
        MMU_pf_free((void *)((uint64_t)(leaf->phys_addr << 12)));
        i++;
        // current_addr.value += PAGE_FRAME_SIZE;
    }
}


static void *pointers[4000];
void test_paging(void) {
    size_t i=0;
    pointers[i] = MMU_alloc_page();
    kprintf("DEBUG_NB_ALLOCS = %ld\n", DEBUG_NB_ALLOCS);
    uint64_t *n = pointers[i];
    *n = (uint64_t)pointers[i];
    kprintf("%ld %p\n", i, pointers[i]);
    if (*n != (uint64_t)pointers[i]) {
        kprintf("UH OH! *n should be %p, instead *n=%ld\n", pointers[i], *n);
    }
    i++;
    while (n != (void *)MMU_NULL) {
        pointers[i] = MMU_alloc_page();
        kprintf("DEBUG_NB_ALLOCS = %ld\n", DEBUG_NB_ALLOCS);
        n = pointers[i];
        *n = (uint64_t)pointers[i];
        kprintf("%ld %p\n", i, pointers[i]);
        if (*n != (uint64_t)pointers[i]) {
            kprintf("UH OH! *n should be %p, instead *n=%ld\n", pointers[i], *n);
        }
        i++;
    }
    kprintf("All pages allocated\n");
    for (size_t j=0; j<i; j++) {
        MMU_free_page(pointers[j]);
    }
}





// void *MMU_alloc_page(void) {
//     struct L4_entry *root;
//     __asm__ volatile ("mov %%cr3, %0" : "=r"(root));
//     if (root[KERNEL_HEAP_L4_IDX].P == 0) {
//     }
// }