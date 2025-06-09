#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#include <stddef.h>

#define MAKE_LIST(PREFIX, PTR_T)                                                                                      \
                                                                                                                  \
    struct PREFIX##_list_node                                                                                     \
    {                                                                                                             \
        PTR_T val;                                                                                                    \
        struct PREFIX##_list_node *prev;                                                                          \
        struct PREFIX##_list_node *next;                                                                          \
    };                                                                                                            \
                                                                                                                  \
    struct PREFIX##_list                                                                                          \
    {                                                                                                             \
        struct PREFIX##_list_node *head;                                                                          \
        size_t len;                                                                                               \
    };                                                                                                            \
                                                                                                                  \
    struct PREFIX##_list *PREFIX##_list_new(void)                                                                 \
    {                                                                                                             \
        struct PREFIX##_list *lst = (struct PREFIX##_list *)malloc(sizeof(struct PREFIX##_list));                 \
        lst->head = NULL;                                                                                         \
        lst->len = 0;                                                                                             \
        return lst;                                                                                               \
    }                                                                                                             \
                                                                                                                  \
    struct PREFIX##_list_node *create_node(PTR_T val)                                                                 \
    {                                                                                                             \
        struct PREFIX##_list_node *node = (struct PREFIX##_list_node *)malloc(sizeof(struct PREFIX##_list_node)); \
        node->val = val;                                                                                          \
        node->prev = NULL;                                                                                        \
        node->next = NULL;                                                                                        \
        return node;                                                                                              \
    }                                                                                                             \
                                                                                                                  \
    int insert_node(struct PREFIX##_list *lst, struct PREFIX##_list_node *node, size_t idx)                       \
    {                                                                                                             \
        if (lst->len < idx)                                                                                       \
            return 0;                                                                                             \
        if (lst->len == 0)                                                                                        \
        {                                                                                                         \
            lst->head = node;                                                                                     \
            node->next = node;                                                                                    \
            node->prev = node;                                                                                    \
            lst->len++;                                                                                           \
            return 1;                                                                                             \
        }                                                                                                         \
        struct PREFIX##_list_node *curr = lst->head;                                                              \
        if (idx < lst->len / 2)                                                                                   \
        {                                                                                                         \
            for (size_t k = 0; k < idx; k++)                                                                      \
            {                                                                                                     \
                curr = curr->next;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        else                                                                                                      \
        {                                                                                                         \
            for (size_t k = 0; k < lst->len - idx; k++)                                                           \
            {                                                                                                     \
                curr = curr->prev;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        node->prev = curr->prev;                                                                                  \
        node->next = curr;                                                                                        \
        curr->prev->next = node;                                                                                  \
        curr->prev = node;                                                                                        \
        if (idx == 0)                                                                                             \
        {                                                                                                         \
            lst->head = node;                                                                                     \
        }                                                                                                         \
        lst->len++;                                                                                               \
        return 1;                                                                                                 \
    }                                                                                                             \
    int insert(struct PREFIX##_list *lst, PTR_T val, size_t idx)                                                      \
    {                                                                                                             \
        if (lst->len < idx)                                                                                       \
            return 0;                                                                                             \
        struct PREFIX##_list_node *node = create_node(val);                                                       \
        if (lst->len == 0)                                                                                        \
        {                                                                                                         \
            lst->head = node;                                                                                     \
            node->next = node;                                                                                    \
            node->prev = node;                                                                                    \
            lst->len++;                                                                                           \
            return 1;                                                                                             \
        }                                                                                                         \
        struct PREFIX##_list_node *curr = lst->head;                                                              \
        if (idx < lst->len / 2)                                                                                   \
        {                                                                                                         \
            for (size_t k = 0; k < idx; k++)                                                                      \
            {                                                                                                     \
                curr = curr->next;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        else                                                                                                      \
        {                                                                                                         \
            for (size_t k = 0; k < lst->len - idx; k++)                                                           \
            {                                                                                                     \
                curr = curr->prev;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        node->prev = curr->prev;                                                                                  \
        node->next = curr;                                                                                        \
        curr->prev->next = node;                                                                                  \
        curr->prev = node;                                                                                        \
        if (idx == 0)                                                                                             \
        {                                                                                                         \
            lst->head = node;                                                                                     \
        }                                                                                                         \
        lst->len++;                                                                                               \
        return 1;                                                                                                 \
    }                                                                                                             \
                                                                                                                  \
    int push_front(struct PREFIX##_list *lst, PTR_T val)                                                              \
    {                                                                                                             \
        return insert(lst, val, 0);                                                                               \
    }                                                                                                             \
                                                                                                                  \
    int push_back(struct PREFIX##_list *lst, PTR_T val)                                                               \
    {                                                                                                             \
        return insert(lst, val, lst->len);                                                                        \
    }                \
    PTR_T find_where(struct PREFIX##_list *lst, int (*predicate)(PTR_T)) {\
        struct PREFIX##_list_node *curr = lst->head;\
        for (size_t k=0; k<lst->len; k++) {\
            if (predicate(curr->val)) {\
                return curr->val;\
            }\
            curr = curr->next;\
        }\
        return NULL;\
    }\
    \
        int find_idx_where(struct PREFIX##_list *lst, int (*predicate)(PTR_T)) {\
        struct PREFIX##_list_node *curr = lst->head;\
        for (int k=0; k<(int)lst->len; k++) {\
            if (predicate(curr->val)) {\
                return k;\
            }\
            curr = curr->next;\
        }\
        return -1;\
    }\
                                                                                                                  \
    PTR_T peek(struct PREFIX##_list *lst, size_t idx)                                                                \
    {                                                                                                             \
        if (lst->len == 0 || idx > lst->len - 1)                                                                  \
        {                                                                                                         \
            return NULL;                                                                                          \
        }                                                                                                         \
        struct PREFIX##_list_node *curr = lst->head;                                                              \
        if (idx < lst->len / 2)                                                                                   \
        {                                                                                                         \
            for (size_t k = 0; k < idx; k++)                                                                      \
            {                                                                                                     \
                curr = curr->next;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        else                                                                                                      \
        {                                                                                                         \
            for (size_t k = 0; k < lst->len - idx; k++)                                                           \
            {                                                                                                     \
                curr = curr->prev;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        return curr->val;                                                                                        \
    }                                                                                                             \
                                                                                                                  \
    PTR_T peek_front(struct PREFIX##_list *lst)                                                                      \
    {                                                                                                             \
        return peek(lst, 0);                                                                                      \
    }                                                                                                             \
                                                                                                                  \
    PTR_T peek_back(struct PREFIX##_list *lst)                                                                       \
    {                                                                                                             \
        return peek(lst, lst->len - 1);                                                                           \
    }                                                                                                             \
                                                                                                                  \
    int pop(struct PREFIX##_list *lst, size_t idx)                                                                \
    {                                                                                                             \
        if (lst->len == 0 || idx > lst->len - 1)                                                                  \
        {                                                                                                         \
            return 0;                                                                                             \
        }                                                                                                         \
        if (lst->len == 1)                                                                                        \
        {                                                                                                         \
            free(lst->head);                                                                                      \
            lst->head = NULL;                                                                                     \
            lst->len--;                                                                                           \
            return 1;                                                                                             \
        }                                                                                                         \
        struct PREFIX##_list_node *curr = lst->head;                                                              \
        if (idx < lst->len / 2)                                                                                   \
        {                                                                                                         \
            for (size_t k = 0; k < idx; k++)                                                                      \
            {                                                                                                     \
                curr = curr->next;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        else                                                                                                      \
        {                                                                                                         \
            for (size_t k = 0; k < lst->len - idx; k++)                                                           \
            {                                                                                                     \
                curr = curr->prev;                                                                                \
            }                                                                                                     \
        }                                                                                                         \
        curr->prev->next = curr->next;                                                                            \
        curr->next->prev = curr->prev;                                                                            \
        if (idx == 0)                                                                                             \
        {                                                                                                         \
            lst->head = curr->next;                                                                               \
        }                                                                                                         \
        free(curr);                                                                                               \
        lst->len--;                                                                                               \
        return 1;                                                                                                 \
    }                                                                                                             \
                                                                                                                  \
    int pop_front(struct PREFIX##_list *lst)                                                                      \
    {                                                                                                             \
        return pop(lst, 0);                                                                                       \
    }                                                                                                             \
                                                                                                                  \
    int pop_back(struct PREFIX##_list *lst)                                                                       \
    {                                                                                                             \
        return pop(lst, lst->len - 1);                                                                            \
    }                                                                                                             \
                                                                                                                  \
    struct PREFIX##_list *concat(struct PREFIX##_list *first, struct PREFIX##_list *second)                       \
    {                                                                                                             \
        if (first->len == 0)                                                                                      \
        {                                                                                                         \
            free(first);                                                                                          \
            return second;                                                                                        \
        }                                                                                                         \
        else if (second->len == 0)                                                                                \
        {                                                                                                         \
            free(second);                                                                                         \
            return first;                                                                                         \
        }                                                                                                         \
        struct PREFIX##_list_node *tail1 = first->head->prev;                                                     \
        struct PREFIX##_list_node *tail2 = second->head->prev;                                                    \
        tail1->next = second->head;                                                                               \
        second->head->prev = tail1;                                                                               \
        tail2->next = first->head;                                                                                \
        first->head->prev = tail2;                                                                                \
        first->len += second->len;                                                                                \
        free(second);                                                                                             \
        return first;                                                                                             \
    }                                                                                                             \
                                                                                                                  \
    void rotate_fwd(struct PREFIX##_list *lst, size_t n)                                                          \
    {                                                                                                             \
        if (lst->len < 2)                                                                                         \
        {                                                                                                         \
            return;                                                                                               \
        }                                                                                                         \
        for (size_t i = 0; i < n; i++)                                                                            \
        {                                                                                                         \
            lst->head = lst->head->next;                                                                          \
        }                                                                                                         \
    }                                                                                                             \
                                                                                                                  \
    void rotate_bkwd(struct PREFIX##_list *lst, size_t n)                                                         \
    {                                                                                                             \
        if (lst->len < 2)                                                                                         \
        {                                                                                                         \
            return;                                                                                               \
        }                                                                                                         \
        for (size_t i = 0; i < n; i++)                                                                            \
        {                                                                                                         \
            lst->head = lst->head->prev;                                                                          \
        }                                                                                                         \
    }                                                                                                             \
                                                                                                                  \
    void clear(struct PREFIX##_list *lst)                                                                         \
    {                                                                                                             \
        while (lst->len > 0)                                                                                      \
        {                                                                                                         \
            pop_front(lst);                                                                                       \
        }                                                                                                         \
    }                                                                                                             \
                                                                                                                  \
    void PREFIX##_list_free(struct PREFIX##_list *lst)                                                            \
    {                                                                                                             \
        clear(lst);                                                                                               \
        free(lst);                                                                                                \
    }

#endif