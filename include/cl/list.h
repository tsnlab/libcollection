#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct cl_ListNode {
    struct cl_ListNode* prev;
    struct cl_ListNode* next;

    void* data;
};

struct cl_List {
    struct cl_ListNode* head;
    struct cl_ListNode* tail;
    size_t size;

    int (*compare)(void*, void*);
};

struct cl_List* cl_List_create(int (*compare)(void*, void*));
void cl_List_destroy(struct cl_List* list);

bool cl_List_add(struct cl_List* list, void* data);
bool cl_List_add_at(struct cl_List* list, size_t idx, void* data);
bool cl_List_remove(struct cl_List* list, void* data);
void* cl_List_remove_at(struct cl_List* list, size_t idx);
void* cl_List_get(struct cl_List* list, size_t idx);
size_t cl_List_size(struct cl_List* list);

struct cl_ListIterator {
    struct cl_List* list;
    struct cl_ListNode* current;
    struct cl_ListNode* next;
};

void cl_ListIterator_init(struct cl_ListIterator* iter, struct cl_List* list);
bool cl_ListIterator_has_next(struct cl_ListIterator* iter);
void* cl_ListIterator_next(struct cl_ListIterator* iter);
void* cl_ListIterator_remove(struct cl_ListIterator* iter);

#define cl_List_create_u32() cl_List_create(NULL)
#define cl_List_destroy_u32(list) cl_List_destroy(list)
#define cl_List_add_u32(list, data) cl_List_add(list, (void*)(uintptr_t)data)
#define cl_List_add_at_u32(list, idx, data) cl_List_add_at_u32(list, idx, (void*)(uintptr_t)data)
#define cl_List_remove_u32(list, data) cl_List_remove(list, (void*)(uintptr_t)data)
#define cl_List_remove_at_u32(list, idx) cl_List_remove_at(list, idx)
#define cl_List_get_u32(list, idx) (uint32_t)(uintptr_t) cl_List_get(list, idx)
#define cl_List_size_u32(list) cl_List_size(list)

#define cl_ListIterator_init_u32(iter, list) cl_ListIterator_init(iter, list)
#define cl_ListIterator_has_next_u32(iter) cl_ListIterator_has_next(iter)
#define cl_ListIterator_next_u32(iter) (uint32_t)(uintptr_t) cl_ListIterator_next(iter)
#define cl_ListIterator_remove_u32(iter) (uint32_t)(uintptr_t) cl_ListIterator_remove(iter)

#if UINTPTR_MAX == 0xffff                // 16bit pointer
#elif UINTPTR_MAX == 0xffffffff          // 32bit pointer
#elif UINTPTR_MAX == 0xffffffffffffffffu // 64bit pointer
#endif
