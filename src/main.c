#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "../include/task.h"
#include "../include/io.h"
#include "../include/scheduler.h"

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(x) Sleep(x)
    #define OPEN_CMD "start"
#else
    #define SLEEP_MS(x) usleep((x)*1000)
    #define OPEN_CMD "xdg-open" 
#endif

#define MAX_TASKS 200
#define COL_CYAN "\033[36m"
#define COL_RESET "\033[0m"

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void asistente_rapido(Task tasks[], size_t n) {
    int minutos_disponibles;
    printf("\n   🤖 IA: Cuantos minutos tienes libres AHORA MISMO? > ");
    if (scanf("%d", &minutos_disponibles) != 1) return;
    
    scheduler_recompute(tasks, n);
    scheduler_sort_by_score(tasks, n);

    printf("\n   🧠 RECOMENDACION OPTIMA (Greedy Algorithm):\n");
    printf("   ------------------------------------------------\n");
    
    int encontrado = 0;
    for (size_t i = 0; i < n; i++) {
        if (tasks[i].duration_min <= minutos_disponibles && tasks[i].final_score > 50) {
            printf("   ✅ MEJOR OPCION: %s\n", tasks[i].title);
            printf("      (Categoria: %s | Duracion: %d min)\n", tasks[i].category, tasks[i].duration_min);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado && n > 0) {
        printf("   ⚡ AVANCE SUGERIDO: Dedica estos %d min a '%s'\n", minutos_disponibles, tasks[0].title);
    }
    if (n==0) printf("   🎉 No tienes pendientes urgentes.\n");
    
    printf("   ------------------------------------------------\n");
    printf("   Enter...");
    getchar(); getchar();
}

static void print_menu() {
    printf("\n");
    printf(COL_CYAN "   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║        PLANIFICADOR INTELIGENTE (IA v8.0)          ║\n");
    printf("   ╠════════════════════════════════════════════════════╣\n" COL_RESET);
    printf("   ║  1. Agregar Actividad                              ║\n");
    printf("   ║  2. Ver Agenda                                     ║\n");
    printf("   ║  3. Asistente Rapido                               ║\n");
    printf("   ║  4. Analisis Profundo                              ║\n");
    printf("   ║  5. Ver Reporte y Planificacion                    ║\n");
    printf("   ║  6. Salir                                          ║\n");
    printf(COL_CYAN "   ╚════════════════════════════════════════════════════╝\n" COL_RESET);
    printf("\n   Opcion (Escribe el numero)> ");
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    Task tasks[MAX_TASKS];
    size_t n = 0;
    int next_id = 1;
    int op = 0;
    char cmd[256];

    clear_screen();

    while (1) {
        print_menu();
        if (scanf("%d", &op) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {} continue;
        }
        int c; while ((c = getchar()) != '\n' && c != EOF) {}

        if (op == 1) { 
            if (n >= MAX_TASKS) { printf("Lleno.\n"); continue; }
            Task t;
            if (io_read_task(&t, next_id)) {
                next_id++;
                tasks[n++] = t;
            }
            clear_screen(); 
        }
        else if (op == 2) { 
            clear_screen();
            printf("\n   ⏳ Generando Dashboard HTML...\n");
            if (exportar_agenda_html(tasks, n)) {
                sprintf(cmd, "%s Reporte_Agenda.html", OPEN_CMD);
                system(cmd);
                printf("   ✅ Abriendo navegador.\n");
            }
            SLEEP_MS(1000);
            clear_screen();
        }
        else if (op == 3) {
            clear_screen();
            if (n == 0) printf("\n   [!] Vacio.\n");
            else asistente_rapido(tasks, n);
            clear_screen();
        }
        else if (op == 4) {
            clear_screen();
            printf("\n   ⏳ IA procesando datos y estrategias...\n");
            if (exportar_analisis_html(tasks, n)) {
                sprintf(cmd, "%s Reporte_Analisis.html", OPEN_CMD);
                system(cmd);
                printf("   ✅ Abriendo reporte.\n");
            }
            SLEEP_MS(1000);
            clear_screen();
        }
        else if (op == 5) {
            clear_screen();
            scheduler_recompute(tasks, n);
            scheduler_sort_by_score(tasks, n);
            printf("\n   ⏳ Generando Time Blocking...\n");
            if (exportar_reporte_html(tasks, n)) {
                sprintf(cmd, "%s Plan_Inteligente.html", OPEN_CMD);
                system(cmd);
                printf("   ✅ Abriendo horario.\n");
            }
            SLEEP_MS(1000);
            clear_screen();
        }
        else if (op == 6) break;
    }
    return 0;
}