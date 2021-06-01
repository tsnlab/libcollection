#include <cl/list.h>
#include <malloc.h>
#include <string.h>

struct list* list_create(size_t (*compare)(void* v1, void* v2)) {
    struct list* list = calloc(1, sizeof(struct list));
    if(list == NULL) {
        return NULL;
    }

    list->compare = compare;

    return list;
}

void list_destroy(struct list* list) {
    struct list_iterator iter;
    list_iterator_init(&iter, list);

    while(list_iterator_has_next(&iter)) {
        list_iterator_next(&iter);

        if(iter.current != NULL) {
            free(iter.current);
        }
    }

    free(list);
}

static void _add(struct list* list, struct listNode* prev, struct listNode* node) {
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

bool list_add(struct list* list, void* data) {
    struct listNode* node = calloc(1, sizeof(struct listNode));
    if(node == NULL) {
        return NULL;
    }

    node->data = data;
    _add(list, list->tail, node);

    return true;
}

bool list_add_at(struct list* list, size_t idx, void* data) {
    struct listNode* node = calloc(1, sizeof(struct listNode));
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

        struct listNode* cursor = list->head;
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

static void _remove(struct list* list, struct listNode* node) {
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

bool list_remove(struct list* list, void* data) {
    struct listNode* node = list->head;

    while(node != NULL) {
        if(node->data == data || (list->compare != NULL && list->compare(node->data, data))) {
            _remove(list, node);
            return true;
        }

        node = node->next;
    }

    return false;
}

void* list_remove_at(struct list* list, size_t idx) {
    struct listNode* node = list->head;

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

void* list_get(struct list* list, size_t idx) {
    struct listNode* node = list->head;

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

size_t list_size(struct list* list) {
    return list->size;
}

void list_iterator_init(struct list_iterator* iter, struct list* list) {
    iter->list = list;
    iter->current = NULL;
    iter->next = list->head;
}

bool list_iterator_has_next(struct list_iterator* iter) {
    return iter->next != NULL;
}

void* list_iterator_next(struct list_iterator* iter) {
    if(iter->next != NULL) {
        void* data = iter->next->data;
        iter->current = iter->next;
        iter->next = iter->next->next;

        return data;
    } else {
        return NULL;
    }
}

void* list_iterator_remove(struct list_iterator* iter) {
    if(iter->current != NULL) {
        void* data = iter->current->data;
        _remove(iter->list, iter->current);
        return data;
    } else {
        return NULL;
    }
}
