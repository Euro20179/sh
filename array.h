#include <stddef.h>
typedef struct {
    size_t itemSize;
    size_t len;
    size_t maxLen;
    void* items;
} Array;

void array_init(Array* out, size_t itemSize, size_t maxLen);

void array_append(Array* arr, void* item);

//if idx is out of bounds this returns 0
//
//otherwise returns 1
int array_at(Array* arr, size_t idx, void* out);

void array_delete(Array* arr);
