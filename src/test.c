#include <cl/list.h>
#include <cl/map.h>
#include <stdio.h>

#define PASS "\033[92mPASS\033[0m\n"
#define FAIL "\033[91mFAIL\033[0m\n"

static void dump_list(struct list* list) {
    printf("list(size=%lu) ", list_size(list));

    struct list_iterator iter;
    list_iterator_init(&iter, list);
    while(list_iterator_has_next(&iter)) {
        void* data = list_iterator_next(&iter);
        printf("%u ", to_u32(data));
    }

    printf("\n");
}

static void dump_map(struct map* map) {
    printf("map(size=%lu) ", map_size(map));

    struct map_iterator iter;
    map_iterator_init(&iter, map);
    while(map_iterator_has_next(&iter)) {
        struct map_entry* entry = map_iterator_next(&iter);
        printf("%u: %u ", to_u32(entry->key), to_u32(entry->data));
    }

    printf("\n");
}

static bool check_map(const char* title, struct map* map, size_t size, uintptr_t* array) {
    printf("Test: %s: ", title);

    if(map_size(map) != size) {
        printf("Worng map size: %lu, expected: %lu ", map_size(map), size);
        printf(FAIL);
        dump_map(map);
        return false;
    }

    struct map_iterator iter;
    map_iterator_init(&iter, map);

    size_t i;
    for(i = 0; i < size; i++) {
        if(!map_iterator_has_next(&iter)) {
            printf("map_iterator_has_next[%lu] is false ", i);
            printf(FAIL);
            dump_map(map);
            return false;
        }

        struct map_entry* entry = map_iterator_next(&iter);
        for(int j = 0; j < size; j++) {
            uintptr_t key = array[j * 2];
            uintptr_t data = array[j * 2 + 1];

            if(key == (uintptr_t)entry->key && data == (uintptr_t)entry->data) {
                goto found;
            }
        }

        printf("cannot find entry: %lu: %lu ", (uintptr_t)entry->key, (uintptr_t)entry->data);
        printf(FAIL);
        dump_map(map);
        return false;
found:;
    }

    if(map_iterator_has_next(&iter)) {
        printf("map_iterator_has_next[%lu] is true ", i);
        printf(FAIL);
        dump_map(map);
        return false;
    }

    printf(PASS);

    return true;
}

static bool check_list(const char* title, struct list* list, size_t size, uintptr_t* array) {
    printf("Test: %s: ", title);

    if(list_size(list) != size) {
        printf("Worng list size: %lu, expected: %lu ", list_size(list), size);
        printf(FAIL);
        dump_list(list);
        return false;
    }

    struct list_iterator iter;
    list_iterator_init(&iter, list);

    size_t i;
    for(i = 0; i < size; i++) {
        if(!list_iterator_has_next(&iter)) {
            printf("list_iterator_has_next[%lu] is false ", i);
            printf(FAIL);
            dump_list(list);
            return false;
        }

        uintptr_t data = (uintptr_t)list_iterator_next(&iter);
        if(data != array[i]) {
            printf("content is different: %lu, exptected: %lu ", data, array[i]);
            printf(FAIL);
            dump_list(list);
            return false;
        }
    }

    if(list_iterator_has_next(&iter)) {
        printf("list_iterator_has_next[%lu] is true ", i);
        printf(FAIL);
        dump_list(list);
        return false;
    }

    printf(PASS);

    return true;
}

int main(int argc, char** argv) {
    struct list* list = list_create(NULL);
    if(!check_list("Empty list", list, 0, NULL))
        return false;

    list_add(list, from_u32(10));
    uintptr_t array1[] = {10};
    if(!check_list("Add 10", list, sizeof(array1) / sizeof(uintptr_t), array1))
        return false;

    list_add(list, from_u32(20));
    list_add(list, from_u32(30));
    list_add(list, from_u32(40));
    list_add(list, from_u32(50));
    list_add(list, from_u32(60));
    list_add(list, from_u32(70));
    list_add(list, from_u32(80));
    list_add(list, from_u32(90));
    list_add(list, from_u32(100));
    uintptr_t array2[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    if(!check_list("Add multiple", list, sizeof(array2) / sizeof(uintptr_t), array2))
        return false;

    uintptr_t array3[] = {10, 30, 40, 50, 60, 70, 80, 90, 100};
    list_remove_at(list, 1);
    if(!check_list("Remove by index", list, sizeof(array3) / sizeof(uintptr_t), array3))
        return false;

    uintptr_t array4[] = {10, 30, 40, 50, 60, 70, 80, 100};
    list_remove(list, from_u32(90));
    if(!check_list("Remove by data", list, sizeof(array4) / sizeof(uintptr_t), array4))
        return false;

    uintptr_t array5[] = {30, 40, 50, 60, 70, 80, 100};
    list_remove_at(list, 0);
    if(!check_list("Remove first by index", list, sizeof(array5) / sizeof(uintptr_t), array5))
        return false;

    uintptr_t array6[] = {30, 40, 50, 60, 70, 80};
    list_remove_at(list, 6);
    if(!check_list("Remove last by index", list, sizeof(array6) / sizeof(uintptr_t), array6))
        return false;

    uintptr_t array7[] = {40, 50, 60, 70, 80};
    list_remove(list, from_u32(30));
    if(!check_list("Remove first by data", list, sizeof(array7) / sizeof(uintptr_t), array7))
        return false;

    uintptr_t array8[] = {40, 50, 60, 70};
    list_remove(list, from_u32(80));
    if(!check_list("Remove first by data", list, sizeof(array8) / sizeof(uintptr_t), array8))
        return false;

    uintptr_t array9[] = {50, 60, 70};
    list_remove(list, from_u32(40));
    if(!check_list("Remove first by data", list, sizeof(array9) / sizeof(uintptr_t), array9))
        return false;

    if(list_remove_at(list, -1)) {
        printf("Remove underbound returns true" FAIL);
    }

    if(!check_list("Remove underbound", list, sizeof(array9) / sizeof(uintptr_t), array9))
        return false;

    if(list_remove_at(list, 100)) {
        printf("Remove overbound returns true" FAIL);
    }

    if(!check_list("Remove overbound", list, sizeof(array9) / sizeof(uintptr_t), array9))
        return false;

    struct list_iterator iter;
    list_iterator_init(&iter, list);
    while(list_iterator_has_next(&iter)) {
        list_iterator_next(&iter);
        list_iterator_remove(&iter);
    }

    if(!check_list("Remove all by iterator", list, 0, NULL))
        return false;

    list_destroy(list);

    struct map* map = map_create(8, uint32_hash, NULL);
    check_map("Empty map", map, 0, NULL);

    uintptr_t array21[] = {10, 100};
    map_put(map, from_u32(10), from_u32(100));
    check_map("Put 10: 100", map, sizeof(array21) / sizeof(uintptr_t) / 2, array21);

    uintptr_t array22[] = {10, 100, 20, 200, 30, 300, 40, 400, 50, 500};
    map_put(map, from_u32(20), from_u32(200));
    map_put(map, from_u32(30), from_u32(300));
    map_put(map, from_u32(40), from_u32(400));
    map_put(map, from_u32(50), from_u32(500));
    check_map("Put multiple", map, sizeof(array22) / sizeof(uintptr_t) / 2, array22);

    uintptr_t array23[] = {20, 200, 30, 300, 40, 400, 50, 500};
    struct map_entry entry = map_remove(map, from_u32(10));
    if(to_u32(entry.key) != 10 || to_u32(entry.data) != 100) {
        printf("remove return value is not corrent: %lu: %lu, expected: %u: %u " FAIL, (uintptr_t)entry.key,
               (uintptr_t)entry.data, 10, 100);
    }
    check_map("Put remove first", map, sizeof(array23) / sizeof(uintptr_t) / 2, array23);

    uintptr_t array24[] = {20, 200, 30, 300, 40, 400};
    entry = map_remove(map, from_u32(50));
    if(to_u32(entry.key) != 50 || to_u32(entry.data) != 500) {
        printf("remove return value is not corrent: %lu: %lu, expected: %u: %u " FAIL, (uintptr_t)entry.key,
               (uintptr_t)entry.data, 10, 100);
    }
    check_map("Put remove last", map, sizeof(array24) / sizeof(uintptr_t) / 2, array24);

    uintptr_t array25[] = {20, 200, 40, 400};
    entry = map_remove(map, from_u32(30));
    if(to_u32(entry.key) != 30 || to_u32(entry.data) != 300) {
        printf("remove return value is not corrent: %lu: %lu, expected: %u: %u " FAIL, (uintptr_t)entry.key,
               (uintptr_t)entry.data, 30, 300);
    }
    check_map("Put remove middle", map, sizeof(array25) / sizeof(uintptr_t) / 2, array25);

    uintptr_t array26[] = {20, 200, 40, 400, 70, 700, 80, 800, 90, 900, 100, 1000, 110, 1100, 120, 1200};
    map_put(map, from_u32(70), from_u32(700));
    map_put(map, from_u32(80), from_u32(800));
    map_put(map, from_u32(90), from_u32(900));
    map_put(map, from_u32(100), from_u32(1000));
    map_put(map, from_u32(110), from_u32(1100));
    map_put(map, from_u32(120), from_u32(1200));

    if(map_capacity(map) != 16) {
        printf("capacity is differ: %lu, expected: %u " FAIL, map_capacity(map), 16);
    }

    check_map("Exlarge table ", map, sizeof(array26) / sizeof(uintptr_t) / 2, array26);

    map_destroy(map);

    return 0;
}
