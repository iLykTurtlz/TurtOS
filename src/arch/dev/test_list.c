#include "circular_list.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int main() {
    char *s = "abcdefghijklmnopqrstuvwxyz";
    struct char_list *l1 = char_list_new();
    struct char_list *l2 = char_list_new();

    for (size_t i=0; i<strlen(s); i++) {
        push_back(l1, s[i]);
        push_front(l2, s[i]);
    }
    for (size_t i=0; i<l1->len; i++) {
        printf("%c", *peek(l1, i));
    }
    printf("\n");
    for (size_t i=0; i<l2->len; i++) {
        printf("%c", *peek(l2, i));
    }
    printf("\n");
    struct char_list *together = concat(l1, l2);
    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");
    printf("front and back: %c, %c\n", *peek_front(together), *peek_back(together));

    // pop_front(together);
    // pop_front(together);
    // pop_back(together);
    // pop_back(together);

    // rotate_fwd(together, 200 % 52);
    // rotate_bkwd(together, 2);

    insert(together, 'X', 3);

    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");

    insert(together, 'X', 50);


    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");

    pop(together, 50);

    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");

    pop(together, 3);

    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");


    struct char_list *singleton = char_list_new();
    push_front(singleton, 'X');
    assert(pop(singleton, 25) == 0);
    pop(singleton, 0);
    assert(singleton->head == NULL);

    rotate_fwd(together, 35);

    for (size_t i=0; i<together->len; i++) {
        printf("%c", *peek(together, i));
    }
    printf("\n");

    int loc = find_idx_where(together, &is_vowel);
    char *c = find_where(together, &is_vowel);
    printf("loc = %d, vowel=%c\n", loc, *c);


    char_list_free(singleton);
    char_list_free(together);

    


    return 0;


}