#include "list.h"
#include <stdio.h>

struct point {
    int x;
    int y;
};

MAKE_LIST(point, struct point *)

void print_list(struct point_list *l) {
    for (size_t i=0; i<l->len; i++) {
        struct point *p = peek(l, i);
        printf("(%d,%d) ", p->x, p->y);
    }
    printf("\n");
}

int has_negative_x(struct point *p) {
    return p->x < 0;
}

int main() {
    struct point_list *l = point_list_new();
    struct point p1 = {5,6};
    push_front(l, &p1);
    struct point p2 = {7,8};
    push_front(l, &p2);
    struct point p3 = {4,3};
    insert(l, &p3, 1);
    struct point neg_point = {-100, 5};
    insert(l, &neg_point, 2);
    struct point *res = find_where(l, &has_negative_x);
    int loc = find_idx_where(l, &has_negative_x);
    printf("res = (%d,%d) @ idx %d\n", res->x, res->y, loc);
    print_list(l);





}