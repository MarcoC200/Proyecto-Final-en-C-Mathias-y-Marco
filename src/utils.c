#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

int read_line(char* buf, size_t cap) {
    if (!fgets(buf, (int)cap, stdin)) return 0;
    trim_newline(buf);
    return 1;
}

void trim_newline(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) s[n-1] = '\0';
    n = strlen(s);
    if (n > 0 && (s[n-1] == '\r')) s[n-1] = '\0';
}

int parse_int(const char* s, int* out) {
    if (!s || !out) return 0;
    char* end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || *end != '\0') return 0;
    *out = (int)v;
    return 1;
}

int date_is_valid(Date d) {
    if (d.year < 1900 || d.year > 3000) return 0;
    if (d.month < 1 || d.month > 12) return 0;
    if (d.day < 1 || d.day > 31) return 0;
    return 1;
}

int parse_date_yyyy_mm_dd(const char* s, Date* out) {
    if (!s || !out) return 0;
    int y=0,m=0,d=0;
    if (sscanf(s, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    Date tmp = {y,m,d};
    if (!date_is_valid(tmp)) return 0;
    *out = tmp;
    return 1;
}

static time_t to_time_t(Date d) {
    struct tm tmv;
    memset(&tmv, 0, sizeof(tmv));
    tmv.tm_year = d.year - 1900;
    tmv.tm_mon  = d.month - 1;
    tmv.tm_mday = d.day;
    tmv.tm_hour = 12; // evita problemas de DST
    return mktime(&tmv);
}

int days_until(Date due) {
    time_t now = time(NULL);
    struct tm* lt = localtime(&now);

    Date today = { lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday };

    time_t t_due = to_time_t(due);
    time_t t_today = to_time_t(today);

    double diff = difftime(t_due, t_today);
    int days = (int)(diff / (60.0 * 60.0 * 24.0));
    return days;
}
