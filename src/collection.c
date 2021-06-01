#include <cl/collection.h>
#include <string.h>

size_t string_compare(void* str1, void* str2) {
    return strcmp(str1, str2);
}

size_t string_hash(void* str) {
    size_t len = strlen(str);
    size_t hash = 0;

    size_t i;
    for(i = 0; i + sizeof(size_t) - 1 < len; i += sizeof(size_t)) {
        hash += *(size_t*)(str + i);
    }

    for(; i < len; i++) {
        hash += *(uint8_t*)(str + i);
    }

    return hash;
}

size_t uint8_hash(void* value) {
    return (size_t)(uint8_t)(uintptr_t)value;
}

size_t int8_hash(void* value) {
    return (size_t)(int8_t)(intptr_t)value;
}

size_t uint16_hash(void* value) {
    return (size_t)(uint16_t)(uintptr_t)value;
}

size_t int16_hash(void* value) {
    return (size_t)(int16_t)(intptr_t)value;
}

size_t uint32_hash(void* value) {
    return (size_t)(uint32_t)(uintptr_t)value;
}

size_t int32_hash(void* value) {
    return (size_t)(int32_t)(intptr_t)value;
}

size_t uint64_hash(void* value) {
    return (size_t)(uint64_t)(uintptr_t)value;
}

size_t int64_hash(void* value) {
    return (size_t)(int64_t)(intptr_t)value;
}

size_t float_hash(void* value) {
    return (size_t)(uintptr_t)value;
}

#if UINTPTR_MAX == 0xffffffffffffffffu // 64bit pointer
size_t double_hash(void* value) {
    return (size_t)(uintptr_t)value;
}
#endif
