#pragma once

#include <cl/collection.h>

struct listNode {
    struct listNode* prev;
    struct listNode* next;

    void* data;
};

struct list {
    struct listNode* head;
    struct listNode* tail;
    size_t size;

    size_t (*compare)(void*, void*);
};

// Double linked list for pointer
struct list* list_create(size_t (*compare)(void*, void*));
void list_destroy(struct list* list);

bool list_add(struct list* list, void* data);
bool list_add_at(struct list* list, size_t idx, void* data);
bool list_remove(struct list* list, void* data);
void* list_remove_at(struct list* list, size_t idx);
void* list_get(struct list* list, size_t idx);
size_t list_size(struct list* list);

struct list_iterator {
    struct list* list;
    struct listNode* current;
    struct listNode* next;
};

void list_iterator_init(struct list_iterator* iter, struct list* list);
bool list_iterator_has_next(struct list_iterator* iter);
void* list_iterator_next(struct list_iterator* iter);
void* list_iterator_remove(struct list_iterator* iter);
