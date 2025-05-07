#include "circular_queue.h"
#include <stdio.h>

void consumer(int num) {
    printf("%d ", num);
} 

MAKE_CIRCULAR_QUEUE(int, 5)

int main() {
    struct int_circular_queue buff;
    init_int_circular_queue(&buff, consumer);

    int i;
    for (i=0; i<10; i++)
        produce(&buff, i);
    int j;
    for (j=0; j<10; j++)
        consume(&buff);
    for (j=8; j<; j++){
        for (int k=1; k<j; k++) {
            produce(&buff, i++);
        }
        for (int k=1; k<j; k++) {
            consume(&buff);
        }
    }
    return 0;
}