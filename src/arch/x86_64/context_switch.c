#include "context_switch.h"
#include <stdint.h>
#include "list.h"
#include "phys_memory.h"
#include "string.h"
#include "kprint.h"
#include "irq.h"


void test_entry(void *arg) {
	int *num = arg;
	for (int i=0; i<5; i++) {
		kprintf("Hello from thread num %d\n", *num);
        yield();
	}
}




// "list.h" doubly linked circular list of struct Process *
MAKE_LIST(PROC, struct Process *)


static struct Process *proc0 = KERNEL_NULL;
struct Process *curr_proc = KERNEL_NULL;
static struct Process *next_proc = KERNEL_NULL;
static struct PROC_list *ready_proc, *all_proc;
static int next_pid = 1;

// for DEBUG
void display_procs() {
    if (ready_proc->len == 0)
        return;
    struct PROC_list_node *curr = ready_proc->head;
    kprintf("ready_proc: [ ");
    for (size_t i=0; i<ready_proc->len; i++) {
        kprintf("%d@0x%p ", curr->val->pid, curr);
        curr = curr->next;
    }
    kprintf("]\n");
}

inline void yield(void)
{
    PROC_reschedule();
    __asm__ volatile("int $0x80");
}

// leverage existing interrupt handler...-> call yield from it?

void handle_yield(uint8_t irq, uint32_t err, void *arg)
{
    struct Context *ctx = arg;  
    // uint64_t cs = ctx->cs;
    // uint64_t ss = ctx->ss;
    // kprintf("cs = %ld, ss = %ld\n", cs, ss);

    if (curr_proc != next_proc) {  
        if (next_proc == KERNEL_NULL) {
            kprintf("handle_yield: next_proc is null!\n");
            asm volatile ("hlt");
        }
        //save the current process's context
        memcpy(&curr_proc->ctx, ctx, sizeof(struct Context));
        push_back(ready_proc, curr_proc);
        curr_proc = next_proc;
        // curr_proc->ctx.cs = cs;
        // curr_proc->ctx.ss = ss;
        memcpy(ctx, &curr_proc->ctx, sizeof(struct Context));
    }
}

void handle_kexit(uint8_t irq, uint32_t err, void *arg) {
    struct Context *ctx = arg; // useful if I wanna save zombies later
    kfree(curr_proc);
    curr_proc = next_proc;
    next_proc = KERNEL_NULL; 
    memcpy(ctx, &curr_proc->ctx, sizeof(struct Context));
}

void PROC_init(void)
{
    ready_proc = PROC_list_new();
    all_proc = PROC_list_new();

    // original proc
    proc0 = kmalloc(sizeof(struct Process));
    memset(proc0, 0, sizeof(struct Process));
    proc0->pid = 0; //main thread has pid 0
    proc0->state = READY;
    proc0->ctx.cs = 0x08;
    curr_proc = proc0;
   
    //set handlers
    IRQ_set_handler(0x80, handle_yield, KERNEL_NULL);
    IRQ_set_handler(0x81, handle_kexit, KERNEL_NULL);

    // PROC_test();
}


static int curr_proc_exit = 0;
// Exits and destroys all the state of the thread that calls kexit.
// Needs to run the scheduler to pick another process.
// I also suggest you use a trap-based implementation AND the IST mechanism so that the trap handler
// runs on a different stack. Running on a different stack makes it possible to free the thread's stack
// without pulling the rug out from under yourself.
void kexit(void)
{
    // for batch only
    curr_proc_exit = 1;


    PROC_reschedule();
    // kprintf("curr_proc = %d, next_proc = %d\n", curr_proc->pid, next_proc->pid);
    // display_procs();
    __asm__ volatile("int $0x81");    
}



// Adds a new thread to the multi-tasking system.
// This requires allocating a new stack in the virtual address space and initializing the thread's context
// such that the entry_point function gets executed the next time this thread is scheduled. This function
// does not actually schedule the thread.
struct Process *PROC_create_kthread(kproc_t entry_point, void *arg)
{
    struct Process *proc = kmalloc(sizeof(struct Process));
    memset(proc, 0, sizeof(struct Process));
    proc->pid = next_pid++;
    proc->state = READY;

    //set exit
    proc->stack.exit = kexit;

    // initialize context?
    proc->ctx.rdi = (uint64_t)arg;
    proc->ctx.rip = (uint64_t)entry_point;
    proc->ctx.rsp = (uint64_t)&proc->stack.exit; 
    proc->ctx.cs = 0x08;
    // ss?

    


    // add to queue(s)
    push_back(ready_proc, proc);
    // push_back(all_proc, proc);
    return proc;
}

// Selects the next thread to run. It must select the "thread" that called PROC_run if no other threads
// are available to run. This function does not actually perform a context switch.
void PROC_reschedule(void)
{
    // BATCH
    if (curr_proc_exit || curr_proc->pid == 0) {
        next_proc = peek_front(ready_proc); 
        if (!pop_front(ready_proc))
        {
            // kprintf("PROC_reschedule: failed to pop first from ready_proc\n");
            // I probably won't have kprintf fully initialized
            __asm__ volatile("hlt");
        }
        curr_proc_exit = 0;
    } else {
        next_proc = curr_proc;
    }



    // ROUND ROBIN
    // if (ready_proc->len == 0)
    // {
    //     next_proc = curr_proc;
    // }
    // else
    // {
    //     next_proc = peek_front(ready_proc); 
    //     if (!pop_front(ready_proc))
    //     {
    //         // kprintf("PROC_reschedule: failed to pop first from ready_proc\n");
    //         // I probably won't have kprintf fully initialized
    //         __asm__ volatile("hlt");
    //     }
    // }
}

// predicate
int is_main_thread(struct Process *proc) {
    return proc->pid == 0;
}


void PROC_run(void) {
    while (ready_proc->len > 0)
        yield();
}


void PROC_test() {
    int *arr = kmalloc(5 * sizeof(int));
    for (int i=0; i<5; i++) {
        arr[i] = i;
        PROC_create_kthread(test_entry, arr+i);
    }
    kfree(arr);
}
