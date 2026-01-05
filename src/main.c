#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "../include/task.h"
#include "../include/io.h"
#include "../include/scheduler.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define MAX_TASKS 200

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

static void print_menu() {
    printf("\n");
    printf("   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║           PLANIFICADOR DE TAREAS (IA)              ║\n");
    printf("   ╠════════════════════════════════════════════════════╣\n");
    printf("   ║  1. Agregar tarea                                  ║\n");
    printf("   ║  2. Ver tareas                                     ║\n");
    printf("   ║  3. Generar plan recomendado                       ║\n");
    printf("   ║  4. Cargar tareas de ejemplo                       ║\n");
    printf("   ║  5. Salir                                          ║\n");
    printf("   ╚════════════════════════════════════════════════════╝\n");
    printf("\n   Opcion > ");
}

int main() {

    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    Task tasks[MAX_TASKS];
    size_t n = 0;
    int next_id = 1;
    int op = 0;

    clear_screen();

    while (1) {
        print_menu();
        
        if (scanf("%d", &op) != 1) {

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (op == 1) {
            if (n >= MAX_TASKS) {
                printf("Limite de tareas alcanzado.\n");
                continue;
            }
            Task t;

            if (io_read_task(&t, next_id)) {
                next_id++;
                tasks[n++] = t;

            } else {
                printf("Entrada invalida.\n");
            }
            clear_screen(); 
        }
        else if (op == 2) {
            clear_screen();
            if (n == 0) {
                printf("\n   [!] No hay tareas registradas.\n");
            } else {
                printf("\n   --- LISTA DE TAREAS ---\n");
                for (size_t i = 0; i < n; i++) {
                    task_print_compact(&tasks[i]);
                }
            }
            printf("\n   Presiona Enter para volver...");
            getchar();
            clear_screen();
        }
        else if (op == 3) {
            clear_screen();
            if (n == 0) {
                printf("\n   [!] No hay tareas para analizar.\n");
            } else {
            
                scheduler_recompute(tasks, n);    
                scheduler_sort_by_score(tasks, n); 

                printf("\n   🧠 PLAN RECOMENDADO POR IA (Prioridad Alta a Baja):\n");
                for (size_t i = 0; i < n; i++) {
                    
                    task_print(&tasks[i]);
                }
            }
            printf("\n   Presiona Enter para volver...");
            getchar();
            clear_screen();
        }
        else if (op == 4) {
            size_t loaded = 0;
          
            if (!io_load_tasks_from_file("data/tasks_sample.txt", tasks, MAX_TASKS, &loaded, &next_id)) {
                printf("\n   [X] Error: No se encontro 'data/tasks_sample.txt'\n");
            } else {
            
                n = loaded; 
                printf("\n   [OK] Se cargaron %zu tareas de ejemplo.\n", n);
            }
            printf("   Presiona Enter para continuar...");
            getchar();
            clear_screen();
        }
        else if (op == 5) {
            printf("\n   Hasta luego. ¡Mantente productivo!\n");
            break;
        }
        else {
            printf("   Opcion invalida.\n");
          
        }
    }

    return 0;
}