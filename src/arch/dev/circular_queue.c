#include <stddef.h>
#include <stdio.h>

#define SIZE 8

struct circular_queue {
    size_t consumer;
    size_t producer;
    void (*consume_next)(char);
    int is_full;
    char buffer[SIZE];
};
   
    

static void init_circular_queue(
    struct circular_queue *q, void (*consume_next)(char)
)
{
    q->consumer = q->producer = 0;
    q->consume_next = consume_next;
    q->is_full = 0;
}

static int produce(struct circular_queue *q, char c)
{
    if (q->is_full)
        return 0;
    q->buffer[q->producer] = c;
    q->producer = (q->producer + 1) % SIZE;
    q->is_full = q->producer == q->consumer;
}

static int consume(struct circular_queue *q)
{
    if (!q->is_full && q->consumer == q->producer) //empty
        return;
    q->consume_next(q->buffer[q->consumer]);
    q->consumer = (q->consumer + 1) % SIZE;
    q->is_full = 0;
}

void consumer(char c) {
    printf("%c", c);
}


int main() {
    char *text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    struct circular_queue buff;
    init_circular_queue(&buff, consumer);
    int i;
    for (i=0; i<10; i++)
        produce(&buff, text[i]);
    int j;
    for (j=0; j<10; j++)
        consume(&buff);
    for (j=8; j<50; j++){
        for (int k=1; k<j; k++) {
            produce(&buff, text[i++]);
        }
        for (int k=1; k<j; k++) {
            consume(&buff);
        }
    }
    return 0;
    
}

