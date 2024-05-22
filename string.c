#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_init(String *out) {
  out->len = 0;
  out->buf = malloc(10);
  out->maxlen = 10;
}

void string_resize(String *str, size_t newSize) {
  str->maxlen = newSize;
  str->buf = realloc(str->buf, newSize);
}

void string_append(String *to, String *from) {
  if (from->len + to->len > to->maxlen) {
    string_resize(to, from->len + to->len);
  }
  memcpy(to->buf + to->len, from->buf, from->len);
  to->len += from->len;
}

void string_append_str(String *to, const char *from, size_t len) {
  if (to->len + len > to->maxlen) {
    string_resize(to, to->len + len);
  }
  memcpy(to->buf + to->len, from, len);
  to->len += len;
}

void string_substr(String *str, size_t start, size_t end) {
  // invalid start/end positions
  if (end > str->len || end < 0 || start >= str->len) {
    return;
  }
  str->len = end - start;
  memmove(str, str + start, str->len);
}

void string_slice_to_cstr(String *str, char *out, size_t start, size_t end) {
  // invalid start/end positions
  if (end > str->len || start >= str->len || end < 0) {
    out[0] = '\0';
    return;
  }

  size_t len = end - start;

  memcpy(out, str->buf + start, len);

  out[len + 1] = 0;
}

void string_to_cstr(String *str, char *out, size_t len) {
  for (size_t i = 0; i < len; i++) {
    out[i] = str->buf[i];
  }
  out[len - 1] = 0;
}

void string_clear(String *str) {
  str->len = 0;
  str->maxlen = 10;
  str->buf = realloc(str->buf, 10);
}

void string_needle_at(String *str, char needle, Array *out) {
  for (size_t i = 0; i < str->len; i++) {
    if (str->buf[i] == needle) {
      array_append(out, &i);
    }
  }
}

void string_delete(String *str) { free(str->buf); }
