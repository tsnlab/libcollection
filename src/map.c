#include <malloc.h>
#include <string.h>
#include <cl/map.h>

struct map* map_create(size_t initial_capacity, size_t(*hash)(void*), size_t(*compare)(void*, void*)) {
    size_t capacity = 1;
    while(capacity < initial_capacity)
        capacity <<= 1;

    struct map* map = calloc(1, sizeof(struct map));
    if(map == NULL) {
        return NULL;
    }

    map->table = calloc(1, sizeof(struct list*) * capacity);
    if(map->table == NULL) {
        free(map);
        return NULL;
    }

    map->threshold = CL_MAP_THRESHOLD(capacity);
    map->capacity = capacity;
    map->size = 0;
    map->hash = hash;
    map->compare = compare;

    return map;
}

static void _destroy(struct map* map) {
    for(size_t i = 0; i < map->capacity; i++) {
		if(map->table[i] != NULL) {
			struct list_iterator iter;
			list_iterator_init(&iter, map->table[i]);
			while(list_iterator_has_next(&iter)) {
				struct map_entry* entry = list_iterator_next(&iter);
				free(entry);
			}

			list_destroy(map->table[i]);
		}
    }

    free(map->table);
}

void map_destroy(struct map* map) {
    _destroy(map);
    free(map);
}

bool map_put(struct map* map, void* key, void* data) {
    // Check duplication
    if(map_has(map, key)) {
        return false;
    }

    // Enlarge threshold
    if(map->size + 1 > map->threshold) {
        struct map tmp_map;
        tmp_map.capacity = map->capacity * 2;
        tmp_map.table = calloc(1, sizeof(struct list*) * tmp_map.capacity);
        if(tmp_map.table == NULL) {
            return false;
        }
        tmp_map.threshold = CL_MAP_THRESHOLD(tmp_map.capacity);
        tmp_map.size = 0;
        tmp_map.hash = map->hash;
        tmp_map.compare = map->compare;

        // copy
        struct map_iterator iter;
        map_iterator_init(&iter, map);
        while(map_iterator_has_next(&iter)) {
            struct map_entry* entry = map_iterator_next(&iter);
            if(!map_put(&tmp_map, entry->key, entry->data)) {
                _destroy(&tmp_map);
                return false;
            }
        }

        _destroy(map);
        memcpy(map, &tmp_map, sizeof(struct map));
    }

    // Create list
    size_t idx = map->hash(key) % map->capacity;
    if(map->table[idx] == NULL) {
        map->table[idx] = list_create(NULL);
        if(map->table[idx] == NULL) {
            return false;
        }
    }

    // Create entry
    struct map_entry* entry = calloc(1, sizeof(struct map_entry));
    if(entry == NULL) {
        if(list_size(map->table[idx]) == 0) {
            list_destroy(map->table[idx]);
            map->table[idx] = NULL;
        }

        return false;
    }

    entry->key = key;
    entry->data = data;

    // put
    if(!list_add(map->table[idx], entry)) {
        free(entry);

        if(list_size(map->table[idx]) == 0) {
            list_destroy(map->table[idx]);
            map->table[idx] = NULL;
        }

        return false;
    }

    map->size++;

    return true;
}

void* map_get(struct map* map, void* key) {
    struct map_entry* entry = map_get_entry(map, key);
    if(entry == NULL) {
        return NULL;
    }

    return entry->data;
}

struct map_entry* map_get_entry(struct map* map, void* key) {
    size_t idx = map->hash(key) % map->capacity;
    if(map->table[idx] == NULL) {
        return NULL;
    }

    struct list_iterator iter;
    list_iterator_init(&iter, map->table[idx]);
    while(list_iterator_has_next(&iter)) {
        struct map_entry* entry = list_iterator_next(&iter);
        if((map->compare != NULL && map->compare(entry->key, key) == 0) || entry->key == key) {
            return entry;
        }
    }
    
    return NULL;
}

bool map_has(struct map* map, void* key) {
    struct map_entry* entry = map_get_entry(map, key);
    return entry != NULL;
}

struct map_entry map_remove(struct map* map, void* key) {
	struct map_entry entry2;
	entry2.key = NULL;
	entry2.data = NULL;

    size_t idx = map->hash(key) % map->capacity;
    if(map->table[idx] == NULL) {
        return entry2;
    }

    struct list_iterator iter;
    list_iterator_init(&iter, map->table[idx]);
    while(list_iterator_has_next(&iter)) {
        struct map_entry* entry = list_iterator_next(&iter);
        if(entry->key == key || (map->compare != NULL && map->compare(entry->key, key) == 0)) {
			entry2 = *entry;

            free(entry);
            list_iterator_remove(&iter);

            if(list_size(map->table[idx]) == 0) {
                list_destroy(map->table[idx]);
                map->table[idx] = NULL;
            }

			map->size--;

            return entry2;
        }
    }
    
    return entry2;
}

size_t map_capacity(struct map* map) {
	return map->capacity;
}

size_t map_size(struct map* map) {
	return map->size;
}

void map_iterator_init(struct map_iterator* iter, struct map* map) {
	iter->map = map;

	for(iter->idx = 0; iter->idx < map->capacity && map->table[iter->idx] == NULL; iter->idx++);

	if(iter->idx < map->capacity) {
		list_iterator_init(&iter->list_iter, iter->map->table[iter->idx]);
	}
}

bool map_iterator_has_next(struct map_iterator* iter) {
	if(iter->idx >= iter->map->capacity) {
		return false;
	}

	if(iter->map->table[iter->idx] != NULL && list_iterator_has_next(&iter->list_iter)) {
		return true;
	}

	for(iter->idx++; iter->idx < iter->map->capacity && iter->map->table[iter->idx] == NULL; iter->idx++);

	if(iter->idx < iter->map->capacity) {
		list_iterator_init(&iter->list_iter, iter->map->table[iter->idx]);
		return true;
	} else {
		return false;
	}
}

struct map_entry* map_iterator_next(struct map_iterator* iter) {
	return list_iterator_next(&iter->list_iter);
}

struct map_entry map_iterator_remove(struct map_iterator* iter) {
	struct map_entry* entry = list_iterator_remove(&iter->list_iter);
	struct map_entry entry2 = *entry;
	free(entry);

	if(list_size(iter->map->table[iter->idx]) == 0) {
		list_destroy(iter->map->table[iter->idx]);
		iter->map->table[iter->idx] = NULL;
	}

	iter->map->size--;

	return entry2;
}

