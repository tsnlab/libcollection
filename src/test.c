#include <stdio.h>
#include <cl/list.h>
#include <cl/map.h>

#define PASS "\033[92mPASS\033[0m\n"
#define FAIL "\033[91mFAIL\033[0m\n"

static void dump_list(struct list* list) {
    printf("[%lu] ", list_size(list));

    struct list_iterator iter;
    list_iterator_init(&iter, list);
    while(list_iterator_has_next(&iter)) {
        uintptr_t data = (uintptr_t)list_iterator_next(&iter);
        printf("%lu ", data);
    }

    printf("\n");
}

static void dump_map(struct map* map) {
    printf("[%lu] ", map_size(map));

    struct map_iterator iter;
    map_iterator_init(&iter, map);
    while(map_iterator_has_next(&iter)) {
        uintptr_t data = (uintptr_t)map_iterator_next(&iter);
        printf("%lu ", data);
    }

    printf("\n");
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

    size_t  i;
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
    uintptr_t array1[] = { 10 };
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
    uintptr_t array2[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    if(!check_list("Add multiple", list, sizeof(array2) / sizeof(uintptr_t), array2))
        return false;

    uintptr_t array3[] = { 10, 30, 40, 50, 60, 70, 80, 90, 100 };
    list_remove_at(list, 1);
    if(!check_list("Remove by index", list, sizeof(array3) / sizeof(uintptr_t), array3))
        return false;

    uintptr_t array4[] = { 10, 30, 40, 50, 60, 70, 80, 100 };
    list_remove(list, from_u32(90));
    if(!check_list("Remove by data", list, sizeof(array4) / sizeof(uintptr_t), array4))
        return false;

    uintptr_t array5[] = { 30, 40, 50, 60, 70, 80, 100 };
    list_remove_at(list, 0);
    if(!check_list("Remove first by index", list, sizeof(array5) / sizeof(uintptr_t), array5))
        return false;

    uintptr_t array6[] = { 30, 40, 50, 60, 70, 80 };
    list_remove_at(list, 6);
    if(!check_list("Remove last by index", list, sizeof(array6) / sizeof(uintptr_t), array6))
        return false;

    uintptr_t array7[] = { 40, 50, 60, 70, 80 };
    list_remove(list, from_u32(30));
    if(!check_list("Remove first by data", list, sizeof(array7) / sizeof(uintptr_t), array7))
        return false;

    uintptr_t array8[] = { 40, 50, 60, 70 };
    list_remove(list, from_u32(80));
    if(!check_list("Remove first by data", list, sizeof(array8) / sizeof(uintptr_t), array8))
        return false;

    uintptr_t array9[] = { 50, 60, 70 };
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
	map_destroy(map);

    return 0;
}
