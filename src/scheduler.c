#include <stddef.h> 
#include "../include/scheduler.h"
#include "../include/task.h"
#include "../include/heuristics.h"
#include "../include/utils.h"      

void scheduler_recompute(Task* tasks, size_t count) {
    for (size_t i = 0; i < count; i++) {

        tasks[i].days_left = days_until(tasks[i].due);
        
        heuristic_score(&tasks[i]);
        
        heuristic_explain(&tasks[i]);
    }
}


void scheduler_sort_by_score(Task* tasks, size_t count) {
    if (count < 2) return;

    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            

            if (tasks[j].final_score < tasks[j+1].final_score) {
                Task temp = tasks[j];
                tasks[j] = tasks[j+1];
                tasks[j+1] = temp;
            }
        }
    }
}