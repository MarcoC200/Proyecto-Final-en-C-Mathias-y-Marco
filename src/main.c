#include <stdio.h>
#include <string.h>
#include "task.h"
#include "io.h"
#include "scheduler.h"

#define MAX_TASKS 200

static void print_menu() {
    printf("\nPLANIFICADOR DE TAREAS\n");
    printf("1. Agregar tarea\n");
    printf("2. Ver tareas\n");
    printf("3. Generar plan recomendado\n");
    printf("4. Cargar tareas de ejemplo (data/tasks_sample.txt)\n");
    printf("5. Salir\n");
}

int main() {
    Task tasks[MAX_TASKS];
    size_t n = 0;
    int next_id = 1;

    int op = 0;

    while (1) {
        print_menu();
        printf("\nOpcion: ");
        if (scanf("%d", &op) != 1) {
            // limpiar input
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        // limpiar salto de línea
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (op == 1) {
            if (n >= MAX_TASKS) {
                printf("Limite de tareas alcanzado.\n");
                continue;
            }
            Task t;
            if (!io_read_task(&t, next_id)) {
                printf("Entrada invalida. Intenta de nuevo.\n");
                continue;
            }
            next_id++;
            tasks[n++] = t;
            printf("Tarea agregada.\n");
        }
        else if (op == 2) {
            if (n == 0) {
                printf("No hay tareas.\n");
                continue;
            }
            printf("\n--- TAREAS ---\n");
            for (size_t i = 0; i < n; i++) {
                task_print_compact(&tasks[i]);
            }
        }
        else if (op == 3) {
            if (n == 0) {
                printf("No hay tareas para recomendar.\n");
                continue;
            }

            scheduler_recompute(tasks, n);
            scheduler_sort_by_score(tasks, n);

            printf("\n## PLAN RECOMENDADO PARA HOY:\n\n");
            for (size_t i = 0; i < n; i++) {
                printf("%zu. %s\n", i + 1, tasks[i].title);
                printf("   Motivo: %s\n", tasks[i].reason);
            }
        }
        else if (op == 4) {
            size_t loaded = 0;
            if (!io_load_tasks_from_file("data/tasks_sample.txt", tasks, MAX_TASKS, &loaded, &next_id)) {
                printf("No se pudo cargar el archivo.\n");
            } else {
                n = loaded;
                printf("Cargadas %zu tareas desde data/tasks_sample.txt\n", n);
            }
        }
        else if (op == 5) {
            printf("Saliendo...\n");
            break;
        }
        else {
            printf("Opcion invalida.\n");
        }
    }

    return 0;
}
