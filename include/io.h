#ifndef IO_H
#define IO_H

#include "task.h"
#include <stddef.h>

int io_read_task(Task* out, int next_id);
int io_load_tasks_from_file(const char* path, Task* tasks, size_t cap, size_t* out_n, int* out_next_id);
int exportar_reporte_html(Task tasks[], size_t n);

#endif
