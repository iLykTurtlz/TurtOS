#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

#include "tss.h"
#include <stdint.h>
#include "list.h"
#include "phys_memory.h"

//type declarations
typedef void (*kproc_t)(void*);
typedef enum {READY, BLOCKED, ELECTED, TERMINATED} PROC_state; // ZOMBIE?


struct Context
{
    uint64_t gs, fs, es, ds;
    uint64_t rbp;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi;
    uint64_t rdx, rcx, rbx, rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
}__attribute__((packed));

struct Stack {
    char available[STACK_SIZE - sizeof(void *)];
    void *exit;
}__attribute__((packed));

struct Process
{
    int pid;
    struct Context ctx;
    PROC_state state;
    struct Stack stack;
};

// forward declarations (cf. list.h)
struct PROC_list;



extern struct Process *curr_proc;
extern struct PROC_list *ready_proc, *all_proc, *keyboard_wait, *ata_read_wait;


void PROC_init(void);
void PROC_run(void);
struct Process *PROC_create_kthread(kproc_t entry_point, void *arg); //needed for snakes
void PROC_reschedule(void);
void yield(void);
void kexit(void);
// void PROC_test(void);



// void PROC_block_on(struct ProcessQueue *, int enable_ints);
// void PROC_unblock_all(struct ProcessQueue *);
// void PROC_unblock_head(struct ProcessQueue *);
// void PROC_init_queue(struct ProcessQueue *);

void PROC_block_on(struct PROC_list *, int enable_ints);
void PROC_unblock_all(struct PROC_list *);
void PROC_unblock_head(struct PROC_list *);
struct PROC_list *PROC_list_new();




#endif