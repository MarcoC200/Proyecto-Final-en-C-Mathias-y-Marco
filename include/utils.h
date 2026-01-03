#ifndef UTILS_H
#define UTILS_H

#include "task.h"
#include <stddef.h>

int read_line(char* buf, size_t cap);
void trim_newline(char* s);

int parse_int(const char* s, int* out);
int parse_date_yyyy_mm_dd(const char* s, Date* out);

int days_until(Date due);      // puede ser negativo si ya pasó
int date_is_valid(Date d);

#endif
