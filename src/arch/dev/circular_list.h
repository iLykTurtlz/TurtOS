#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <stdlib.h>

#include <stddef.h>

struct char_list_node {
    char val;
    struct char_list_node *prev;
    struct char_list_node *next;
};

struct char_list {
    struct char_list_node *head;
    size_t len;
};

struct char_list *char_list_new(void) {
    struct char_list *lst = (struct char_list *)malloc(sizeof(struct char_list));
    lst->head = NULL;
    lst->len = 0;
    return lst;
}

struct char_list_node *create_node(char val) {
    struct char_list_node *node = (struct char_list_node *)malloc(sizeof(struct char_list_node));
    node->val = val;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

char *find_where(struct char_list *lst, int (*predicate)(char)) {
    struct char_list_node *curr = lst->head;
    for (size_t k=0; k<lst->len; k++) {
        if (predicate(curr->val)) {
            return &curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}
    
int find_idx_where(struct char_list *lst, int (*predicate)(char)) {
    struct char_list_node *curr = lst->head;
    for (int k=0; k<(int)lst->len; k++) {
        if (predicate(curr->val)) {
            return k;
        }
        curr = curr->next;
    }
    return -1;
}


int insert(struct char_list *lst, char val, size_t idx) {
    if (lst->len < idx)
        return 0;
    struct char_list_node *node = create_node(val);
    if (lst->len == 0) {
        lst->head = node;
        node->next = node;
        node->prev = node;
        lst->len++;
        return 1;
    }
    struct char_list_node *curr = lst->head;
    if (idx < lst->len / 2) {
        for (size_t k=0; k<idx; k++) {
            curr = curr->next;
        }

    } else {
        for (size_t k=0; k < lst->len - idx; k++) {
            curr = curr->prev;
        }
    }
    node->prev = curr->prev;
    node->next = curr;
    curr->prev->next = node;
    curr->prev = node;
    if (idx == 0) {
        lst->head = node; 
    }
    lst->len++;
    return 1;
}

int push_front(struct char_list *lst, char val) {
    return insert(lst, val, 0);
}

int push_back(struct char_list *lst, char val) {
    return insert(lst, val, lst->len);
}

char *peek(struct char_list *lst, size_t idx) {
    if (lst->len == 0 || idx > lst->len - 1) {
        return NULL;
    }
    struct char_list_node *curr = lst->head;
    if (idx < lst->len / 2) {
        for (size_t k=0; k<idx; k++) {
            curr = curr->next;
        }

    } else {
        for (size_t k=0; k < lst->len - idx; k++) {
            curr = curr->prev;
        }
    }
    return &curr->val;
}

char *peek_front(struct char_list *lst) {
    return peek(lst, 0);
}

char *peek_back(struct char_list *lst) {
    return peek(lst, lst->len - 1);
}

int pop(struct char_list *lst, size_t idx) {
    if (lst->len == 0 || idx > lst->len - 1) {
        return 0;
    }
    if (lst->len == 1) {
        free(lst->head);
        lst->head = NULL;
        lst->len--;
        return 1;
    }
    struct char_list_node *curr = lst->head;
    if (idx < lst->len / 2) {
        for (size_t k=0; k<idx; k++) {
            curr = curr->next;
        }

    } else {
        for (size_t k=0; k < lst->len - idx; k++) {
            curr = curr->prev;
        }
    }
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    if (idx == 0) {
        lst->head = curr->next;
    }
    free(curr);
    lst->len--;
    return 1;
}

int pop_front(struct char_list *lst) {
    return pop(lst, 0);
}

int pop_back(struct char_list *lst) {
    return pop(lst, lst->len - 1);
}

struct char_list *concat(struct char_list *first, struct char_list *second) {
    if (first->len == 0) {
        free(first);
        return second;
    }
    else if (second->len == 0) {
        free(second);
        return first;
    }
    struct char_list_node *tail1 = first->head->prev;
    struct char_list_node *tail2 = second->head->prev;
    tail1->next = second->head;
    second->head->prev = tail1;
    tail2->next = first->head;
    first->head->prev = tail2;
    first->len += second->len;
    free(second);
    return first;
}

void rotate_fwd(struct char_list *lst, size_t n) {
    if (lst->len < 2) {
        return;
    }
    for (size_t i=0; i<n; i++) {
        lst->head = lst->head->next;
    }
}

void rotate_bkwd(struct char_list *lst, size_t n) {
     if (lst->len < 2) {
        return;
    }
    for (size_t i=0; i<n; i++) {
        lst->head = lst->head->prev;
    }
}

void clear(struct char_list *lst) {
    while (lst->len > 0) {
        pop_front(lst);
    }
}

void char_list_free(struct char_list *lst) {
    clear(lst);
    free(lst);
}


#endif