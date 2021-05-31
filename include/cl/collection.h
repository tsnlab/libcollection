#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t string_compare(void* str1, void* str2);
size_t string_hash(void* str);
size_t uint8_hash(void* value);
size_t int8_hash(void* value);
size_t uint16_hash(void* value);
size_t int16_hash(void* value);
size_t uint32_hash(void* value);
size_t int32_hash(void* value);
size_t uint64_hash(void* value);
size_t int64_hash(void* value);
size_t float_hash(void* value);
#if UINTPTR_MAX == 0xffffffffffffffffu // 64bit pointer
size_t double_hash(void* value);
#endif

#define from_u8(value) ((void*)(uintptr_t)value)
#define to_u8(value) ((uint8_t)(uintptr_t)value)
#define from_i8(value) ((void*)(intptr_t)value)
#define to_i8(value) ((int8_t)(intptr_t)value)
#define from_u16(value) ((void*)(uintptr_t)value)
#define to_u16(value) ((uint16_t)(uintptr_t)value)
#define from_i16(value) ((void*)(intptr_t)value)
#define to_i16(value) ((int16_t)(intptr_t)value)
#define from_u32(value) ((void*)(uintptr_t)value)
#define to_u32(value) ((uint32_t)(uintptr_t)value)
#define from_i32(value) ((void*)(intptr_t)value)
#define to_i32(value) ((int32_t)(intptr_t)value)
#define from_bool(value) ((void*)(uintptr_t)value)
#define to_bool(value) ((bool)(uintptr_t)value)
#define from_str(value) ((void*)value)
#define to_str(value) ((char*)value)

static inline void* from_f(float value) {
    float tmp = value;                                 
    return (void*)(uintptr_t)*(uint32_t*)&tmp;         
}                                                      
                                                       
static inline float to_f(void* ptr) {   
    uint32_t tmp = (uint32_t)(uintptr_t)ptr;       
    return *(float*)&tmp;                             
}

#if UINTPTR_MAX == 0xffffffffffffffffu // 64bit pointer

#define from_u64(value) ((void*)(uintptr_t)(value))
#define to_u64(value) ((uint64_t)(uintptr_t)(value))
#define from_i64(value) ((void*)(intptr_t)(value))
#define to_i64(value) ((int64_t)(intptr_t)(value))

static inline void* from_d(double value) {
    double tmp = value;                                 
    return (void*)(uintptr_t)*(uint64_t*)&tmp;         
}                                                      
                                                       
static inline double to_d(void* ptr) {   
    uint64_t tmp = (uint64_t)(uintptr_t)ptr;       
    return *(double*)&tmp;                             
}

#endif
