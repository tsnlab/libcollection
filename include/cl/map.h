#pragma once

#include <cl/list.h>

#define CL_MAP_THRESHOLD(cap) (((cap) >> 1) + ((cap) >> 2)) // 75%

struct map_entry {
    void* key;
    void* data;
};

struct map {
    struct list** table;
    size_t threshold;
    size_t capacity;
    size_t size;

    size_t (*hash)(void*);
    size_t (*compare)(void*, void*);
};

// Hash map
struct map* map_create(size_t initial_capacity, size_t (*hash)(void*), size_t (*compare)(void*, void*));
void map_destroy(struct map* map);

bool map_put(struct map* map, void* key, void* data);
void* map_get(struct map* map, void* key);
struct map_entry* map_get_entry(struct map* map, void* key);
bool map_has(struct map* map, void* key);
struct map_entry map_remove(struct map* map, void* key);
size_t map_capacity(struct map* map);
size_t map_size(struct map* map);

struct map_iterator {
    struct map* map;
    size_t idx;
    struct list_iterator list_iter;
};

void map_iterator_init(struct map_iterator* iter, struct map* map);
bool map_iterator_has_next(struct map_iterator* iter);
struct map_entry* map_iterator_next(struct map_iterator* iter);
struct map_entry map_iterator_remove(struct map_iterator* iter);
