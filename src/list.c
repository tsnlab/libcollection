#include <cl/list.h>
#include <malloc.h>
#include <string.h>

struct cl_List* cl_List_create(int (*compare)(void* v1, void* v2)) {
    struct cl_List* list = calloc(1, sizeof(struct cl_List));
    if(list == NULL) {
        return NULL;
    }

    list->compare = compare;

    return list;
}

void cl_List_destroy(struct cl_List* list) {
    struct cl_ListIterator iter;
    cl_ListIterator_init(&iter, list);

    while(cl_ListIterator_has_next(&iter)) {
        cl_ListIterator_next(&iter);

        if(iter.current != NULL) {
            free(iter.current);
        }
    }

    free(list);
}

static void _add(struct cl_List* list, struct cl_ListNode* prev, struct cl_ListNode* node) {
    if(prev != NULL) {
        if(prev->next != NULL) {
            prev->next->prev = node;
            node->next = prev->next;
        } else {
            list->tail = node;
        }

        prev->next = node;
        node->prev = prev;
    } else {
        list->head = list->tail = node;
    }

    list->size++;
}

bool cl_List_add(struct cl_List* list, void* data) {
    struct cl_ListNode* node = calloc(1, sizeof(struct cl_ListNode));
    if(node == NULL) {
        return NULL;
    }

    node->data = data;
    _add(list, list->tail, node);

    return true;
}

bool cl_List_add_at(struct cl_List* list, size_t idx, void* data) {
    struct cl_ListNode* node = calloc(1, sizeof(struct cl_ListNode));
    if(node == NULL) {
        return NULL;
    }

    node->data = data;

    if(idx <= 0) {
        if(list->head != NULL) {
            list->head->prev = node;
            node->next = list->head;
            list->head = node;
        } else {
            list->head = list->tail = node;
        }
        list->size++;
    } else if(idx >= list->size) {
        _add(list, list->tail, node);
    } else {
        idx--;

        struct cl_ListNode* cursor = list->head;
        while(idx > 0) {
            if(cursor->next != NULL) {
                return NULL;
            }

            cursor = cursor->next;
            idx--;
        }

        _add(list, cursor, node);
    }

    return true;
}

static void _remove(struct cl_List* list, struct cl_ListNode* node) {
    if(list->head == node) {
        list->head = node->next;
    }

    if(list->tail == node) {
        list->tail = node->prev;
    }

    list->size--;

    if(node->prev != NULL) {
        node->prev->next = node->next;
    }

    if(node->next != NULL) {
        node->next->prev = node->prev;
    }

    free(node);
}

bool cl_List_remove(struct cl_List* list, void* data) {
    struct cl_ListNode* node = list->head;

    while(node != NULL) {
        void* node_data = node->data;
        if(node_data == data || (list->compare != NULL && list->compare(node_data, data))) {
            _remove(list, node);
            return true;
        }

        node = node->next;
    }

    return false;
}

void* cl_List_remove_at(struct cl_List* list, size_t idx) {
    struct cl_ListNode* node = list->head;

    while(idx > 0 && node != NULL) {
        node = node->next;
        idx--;
    }

    if(node != NULL) {
        void* data = node->data;
        _remove(list, node);
        return data;
    } else {
        return NULL;
    }
}

void* cl_List_get(struct cl_List* list, size_t idx) {
    struct cl_ListNode* node = list->head;

    while(idx > 0) {
        if(node->next == NULL) {
            return NULL;
        }

        node = node->next;
        idx--;
    }

    if(node != NULL) {
        return node->data;
    } else {
        return NULL;
    }
}

size_t cl_List_size(struct cl_List* list) {
    return list->size;
}

void cl_ListIterator_init(struct cl_ListIterator* iter, struct cl_List* list) {
    iter->list = list;
    iter->current = NULL;
    iter->next = list->head;
}

bool cl_ListIterator_has_next(struct cl_ListIterator* iter) {
    return iter->next != NULL;
}

void* cl_ListIterator_next(struct cl_ListIterator* iter) {
    if(iter->next != NULL) {
        void* data = iter->next->data;
        iter->current = iter->next;
        iter->next = iter->next->next;

        return data;
    } else {
        return NULL;
    }
}

void* cl_ListIterator_remove(struct cl_ListIterator* iter) {
    if(iter->current != NULL) {
        void* data = iter->current->data;
        _remove(iter->list, iter->current);
        return data;
    } else {
        return NULL;
    }
}
