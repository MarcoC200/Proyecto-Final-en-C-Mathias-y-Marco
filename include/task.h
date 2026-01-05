#ifndef TASK_H
#define TASK_H

#include <time.h> 

#define REASON_MAX 256

typedef struct {
    int id;
    char title[100];
    
    int importance;   
    int difficulty;   
    int duration_min;
    
    char deadline_str[20]; 
    struct tm due;         
    int days_left;         
    
    double final_score;
    char reason[REASON_MAX];
} Task;

void task_print(const Task *t);         
void task_print_compact(const Task *t); 

#endif