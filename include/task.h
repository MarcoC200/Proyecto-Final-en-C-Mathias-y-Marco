#ifndef TASK_H
#define TASK_H

#include <stddef.h>

#define TITLE_MAX 80
#define REASON_MAX 256

typedef struct {
    int year, month, day;
} Date;

typedef struct {
    int id;
    char title[TITLE_MAX];

    int importance;     // 1..5
    int urgency;        // 1..5
    int duration_min;   // minutos
    Date due;           // fecha límite (YYYY-MM-DD)

    double score;       // calculado por heurística
    char reason[REASON_MAX]; // explicación corta
} Task;

int task_is_valid(const Task* t);
void task_print(const Task* t);
void task_print_compact(const Task* t);

#endif
