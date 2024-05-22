#include "array.h"
#include <string.h>
#include <stdlib.h>

//if maxLen is 0, defaults to 4
void array_init(Array *out, size_t itemSize, size_t maxLen){
    out->itemSize = itemSize;

    out->len = 0;
    if(maxLen == 0){
        maxLen = 4;
    }

    out->maxLen = maxLen;
    out->items = malloc(itemSize * maxLen);
}

void array_append(Array *arr, void *item){
    if(arr->len >= arr->maxLen){
        arr->maxLen *= 2;
        arr->items = realloc(arr->items, arr->maxLen * arr->itemSize);
    }
    //can't do
    //arr->items[arr->len*arr->itemSize] = item because it wont know how many bytes to copy from item
    memcpy(arr->items + (arr->len * arr->itemSize), item, arr->itemSize);
    arr->len++;
}

int array_at(Array *arr, size_t idx, void *out){
    if(idx >= arr->len){
        return 0;
    }
    memcpy(out, arr->items + (idx * arr->itemSize), arr->itemSize);
    return 1;
}

void array_delete(Array *arr){
    free(arr->items);
}
