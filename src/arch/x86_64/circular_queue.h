#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H
#include <stddef.h>

#define MAKE_CIRCULAR_QUEUE(T, SIZE)                           \
    struct T##_circular_queue                                  \
    {                                                          \
        size_t consumer;                                       \
        size_t producer;                                       \
        void (*consume_next)(T);                               \
        int is_full;                                           \
        T buffer[SIZE];                                        \
    };                                                         \
                                                               \
    static void T##_circular_queue_init(                       \
        struct T##_circular_queue *q, void (*consume_next)(T)) \
    {                                                          \
        q->consumer = q->producer = 0;                         \
        q->consume_next = consume_next;                        \
        q->is_full = 0;                                        \
    }                                                          \
                                                               \
    static int produce(struct T##_circular_queue *q, T c)      \
    {                                                          \
        if (q->is_full)                                        \
            return 0;                                          \
        q->buffer[q->producer] = c;                            \
        q->producer = (q->producer + 1) % SIZE;                \
        q->is_full = q->producer == q->consumer;               \
        return 1;                                              \
    }                                                          \
                                                               \
    static int consume(struct T##_circular_queue *q)           \
    {                                                          \
        if (!q->is_full && q->consumer == q->producer)         \
            return 0;                                          \
        q->consume_next(q->buffer[q->consumer]);               \
        q->consumer = (q->consumer + 1) % SIZE;                \
        q->is_full = 0;                                        \
        return 1;                                              \
    }
#endif