#ifndef TASK_H
#define TASK_H

#include <time.h>

#ifdef TITLE_MAX
    #undef TITLE_MAX
#endif
#define TITLE_MAX 100
#define DATE_STR_MAX 32
#define REASON_MAX 128
#define CAT_MAX 20  

typedef struct {
    int id;
    char title[TITLE_MAX];
    char category[CAT_MAX]; 
    int importance;
    int difficulty;
    int duration_min;
    
    char deadline_str[DATE_STR_MAX];
    struct tm due;

    float final_score;
    int days_left;
    char reason[REASON_MAX];
} Task;


void task_print(const Task* t);
void task_print_compact(const Task* t);

#endif