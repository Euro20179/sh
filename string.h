#pragma once
#include <stddef.h>
#include "array.h"

typedef struct {
    size_t len;
    char* buf;
    size_t maxlen;
} String;

//initializes string with maxlen of 10
void string_init(String* out);

void string_resize(String* str, size_t newSize);

void string_append(String* to, String* from);

void string_append_str(String* to, const char* from, size_t len);

//mutates the string to only contain the chars between `start` and `end`
void string_substr(String* str, size_t start, size_t end);

//gets the chars between `start` and `end` then puts it into the output cstr
void string_slice_to_cstr(String* str, char* out, size_t start, size_t end);

//`len` should be the real len + 1 to account for \0
void string_to_cstr(String* str, char* out, size_t len);

//essentially calls string_init, but instead of malloc i uses realloc to realloc to a size of 10
void string_clear(String* str);

void string_delete(String* str);

//Array must be a size_t array
//
//This function outputs an array containing all indecies where needle is found
//
//if needle is not found the output array will have a len of 0
void string_needle_at(String* str, char needle, Array* out);
