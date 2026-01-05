#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../include/utils.h"


void trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}


int read_line(char *buffer, int size) {
    if (!fgets(buffer, size, stdin)) return 0;
    trim_newline(buffer);
    return 1;
}

int parse_int(const char *str, int *out) {
    char *end;
    long val = strtol(str, &end, 10);
    if (end == str) return 0;
    *out = (int)val;
    return 1;
}


int parse_date_yyyy_mm_dd(const char *str, struct tm *out_tm) {
    memset(out_tm, 0, sizeof(struct tm));

    if (sscanf(str, "%d-%d-%d", &out_tm->tm_year, &out_tm->tm_mon, &out_tm->tm_mday) != 3) {
        return 0;
    }
    out_tm->tm_year -= 1900; 
    out_tm->tm_mon -= 1;    
    out_tm->tm_isdst = -1;   
    return 1;
}

int days_until(struct tm target) {
    time_t now_t = time(NULL);
    struct tm target_copy = target;
    

    target_copy.tm_hour = 12; 
    target_copy.tm_min = 0;
    target_copy.tm_sec = 0;
    

    time_t target_t = mktime(&target_copy);
    if (target_t == -1) return 0; 


    double seconds = difftime(target_t, now_t);
    

    return (int)(seconds / 86400.0);
}