#include <stdio.h>
#include "task.h"
#include "utils.h"

int task_is_valid(const Task* t) {
    if (!t) return 0;
    if (t->importance < 1 || t->importance > 5) return 0;
    if (t->urgency < 1 || t->urgency > 5) return 0;
    if (t->duration_min <= 0) return 0;
    if (!date_is_valid(t->due)) return 0;
    return 1;
}

void task_print(const Task* t) {
    if (!t) return;
    printf("ID: %d\n", t->id);
    printf("Tarea: %s\n", t->title);
    printf("Importancia: %d/5\n", t->importance);
    printf("Urgencia: %d/5\n", t->urgency);
    printf("Duracion: %d min\n", t->duration_min);
    printf("Fecha limite: %04d-%02d-%02d\n", t->due.year, t->due.month, t->due.day);
    printf("Score: %.2f\n", t->score);
    printf("Motivo: %s\n", t->reason);
}

void task_print_compact(const Task* t) {
    if (!t) return;
    printf("[%d] %s | score=%.2f | vence %04d-%02d-%02d | %dmin\n",
           t->id, t->title, t->score, t->due.year, t->due.month, t->due.day, t->duration_min);
}
