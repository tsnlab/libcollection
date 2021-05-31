#include <stdio.h>
#include <cl/list.h>

#define PASS "\033[92mPASS\033[0m\n"
#define FAIL "\033[91mFAIL\033[0m\n"

static void dump_list(struct cl_List* list) {
	printf("[%lu] ", cl_List_size(list));

	struct cl_ListIterator iter;
	cl_ListIterator_init(&iter, list);
	while(cl_ListIterator_has_next(&iter)) {
		uintptr_t data = (uintptr_t)cl_ListIterator_next(&iter);
		printf("%lu ", data);
	}

	printf("\n");
}

static bool check_list(const char* title, struct cl_List* list, size_t size, uintptr_t* array) {
	printf("Test: %s ", title);

	if(cl_List_size(list) != size) {
		printf("Worng list size: %lu, expected: %lu ", cl_List_size(list), size);
		printf(FAIL);
		dump_list(list);
		return false;
	}

	struct cl_ListIterator iter;
	cl_ListIterator_init(&iter, list);

	size_t  i;
	for(i = 0; i < size; i++) {
		if(!cl_ListIterator_has_next(&iter)) {
			printf("cl_ListIterator_has_next[%lu] is false ", i);
			printf(FAIL);
			dump_list(list);
			return false;
		}

		uintptr_t data = (uintptr_t)cl_ListIterator_next(&iter);
		if(data != array[i]) {
			printf("content is different: %lu, exptected: %lu ", data, array[i]);
			printf(FAIL);
			dump_list(list);
			return false;
		}
	}

	if(cl_ListIterator_has_next(&iter)) {
		printf("cl_ListIterator_has_next[%lu] is true ", i);
		printf(FAIL);
		dump_list(list);
		return false;
	}

	printf(PASS);

	return true;
}

int main(int argc, char** argv) {
	struct cl_List* list = cl_List_create_u32();
	if(!check_list("Empty list", list, 0, NULL))
		return false;

	cl_List_add_u32(list, 10);
	uintptr_t array1[] = { 10 };
	if(!check_list("Add 10", list, sizeof(array1) / sizeof(uintptr_t), array1))
		return false;

	cl_List_add_u32(list, 20);
	cl_List_add_u32(list, 30);
	cl_List_add_u32(list, 40);
	cl_List_add_u32(list, 50);
	cl_List_add_u32(list, 60);
	cl_List_add_u32(list, 70);
	cl_List_add_u32(list, 80);
	cl_List_add_u32(list, 90);
	cl_List_add_u32(list, 100);
	uintptr_t array2[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	if(!check_list("Add multiple", list, sizeof(array2) / sizeof(uintptr_t), array2))
		return false;

	uintptr_t array3[] = { 10, 30, 40, 50, 60, 70, 80, 90, 100 };
	cl_List_remove_at(list, 1);
	if(!check_list("Remove by index", list, sizeof(array3) / sizeof(uintptr_t), array3))
		return false;

	uintptr_t array4[] = { 10, 30, 40, 50, 60, 70, 80, 100 };
	cl_List_remove_u32(list, 90);
	if(!check_list("Remove by data", list, sizeof(array4) / sizeof(uintptr_t), array4))
		return false;

	uintptr_t array5[] = { 30, 40, 50, 60, 70, 80, 100 };
	cl_List_remove_at(list, 0);
	if(!check_list("Remove first by index", list, sizeof(array5) / sizeof(uintptr_t), array5))
		return false;

	uintptr_t array6[] = { 30, 40, 50, 60, 70, 80 };
	cl_List_remove_at(list, 6);
	if(!check_list("Remove last by index", list, sizeof(array6) / sizeof(uintptr_t), array6))
		return false;

	uintptr_t array7[] = { 40, 50, 60, 70, 80 };
	cl_List_remove_u32(list, 30);
	if(!check_list("Remove first by data", list, sizeof(array7) / sizeof(uintptr_t), array7))
		return false;

	uintptr_t array8[] = { 40, 50, 60, 70 };
	cl_List_remove_u32(list, 80);
	if(!check_list("Remove first by data", list, sizeof(array8) / sizeof(uintptr_t), array8))
		return false;

	uintptr_t array9[] = { 50, 60, 70 };
	cl_List_remove_u32(list, 40);
	if(!check_list("Remove first by data", list, sizeof(array9) / sizeof(uintptr_t), array9))
		return false;

	if(cl_List_remove_at(list, -1)) {
		printf("Remove underbound returns true" FAIL);
	}

	if(!check_list("Remove underbound", list, sizeof(array9) / sizeof(uintptr_t), array9))
		return false;

	if(cl_List_remove_at(list, 100)) {
		printf("Remove overbound returns true" FAIL);
	}

	if(!check_list("Remove overbound", list, sizeof(array9) / sizeof(uintptr_t), array9))
		return false;

	struct cl_ListIterator iter;
	cl_ListIterator_init(&iter, list);
	while(cl_ListIterator_has_next(&iter)) {
		cl_ListIterator_next(&iter);
		cl_ListIterator_remove(&iter);
	}

	if(!check_list("Remove all by iterator", list, 0, NULL))
		return false;

	cl_List_destroy_u32(list);

	return 0;
}
