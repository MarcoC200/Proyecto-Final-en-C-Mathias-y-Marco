#ifndef UTILS_H
#define UTILS_H

#include <time.h> 

#define TITLE_MAX 100

void trim_newline(char *str);
int read_line(char *buffer, int size);
int parse_int(const char *str, int *out);
int parse_date_yyyy_mm_dd(const char *str, struct tm *out_tm);
int days_until(struct tm target);

#endif