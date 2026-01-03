#include <stdio.h>
#include "../include/task.h"
#include "../include/scheduler.h"

int main() {
    Task t[3] = {
        {1,"Tarea A",5,5,60,{2026,1,3},0,""},
        {2,"Tarea B",2,1,30,{2026,1,20},0,""},
        {3,"Tarea C",4,4,20,{2026,1,4},0,""}
    };

    scheduler_recompute(t, 3);
    scheduler_sort_by_score(t, 3);

    printf("Orden esperado: A o C arriba, B al final.\n");
    for (int i = 0; i < 3; i++) {
        printf("%d) %s score=%.2f\n", i+1, t[i].title, t[i].score);
    }
    return 0;
}
