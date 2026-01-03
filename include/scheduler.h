#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <stddef.h>

void scheduler_recompute(Task* tasks, size_t n);
void scheduler_sort_by_score(Task* tasks, size_t n);

#endif
