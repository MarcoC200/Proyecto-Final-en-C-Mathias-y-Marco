#include "scheduler.h"
#include "heuristics.h"

static void swap(Task* a, Task* b) {
    Task tmp = *a;
    *a = *b;
    *b = tmp;
}

void scheduler_recompute(Task* tasks, size_t n) {
    for (size_t i = 0; i < n; i++) {
        heuristic_score(&tasks[i]);
        heuristic_explain(&tasks[i]);
    }
}

// sort desc por score (simple y estable con bubble mejorado para n chico)
void scheduler_sort_by_score(Task* tasks, size_t n) {
    if (!tasks || n < 2) return;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j + 1 < n - i; j++) {
            if (tasks[j].score < tasks[j+1].score) {
                swap(&tasks[j], &tasks[j+1]);
            }
        }
    }
}
