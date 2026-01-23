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
#else
    #define SLEEP_MS(x) usleep((x)*1000)
#endif

#define MAX_TASKS 200
#define BAR_CHAR "█"
#define COL_CYAN "\033[36m"
#define COL_RED "\033[31m"
#define COL_GREEN "\033[32m"
#define COL_YELLOW "\033[33m"
#define COL_RESET "\033[0m"

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_typing(const char* text, int speed_ms) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        SLEEP_MS(speed_ms);
    }
    printf("\n");
}

void ia_diagnostico_profundo(Task tasks[], size_t n) {
    clear_screen();
    
    if (n == 0) {
        printf("\n   [!] No hay datos para analizar. Agrega tareas primero.\n");
        SLEEP_MS(1500);
        return;
    }

    printf("\n\n");
    printf("   🧠 INICIANDO RED NEURONAL DE ANALISIS...\n");
    printf("   [");
    for(int i=0; i<40; i++) {
        printf("=");
        fflush(stdout);
        SLEEP_MS(20);
    }
    printf("] 100%%\n\n");
    SLEEP_MS(500);

    int mins_uni = 0, mins_vida = 0, mins_ocio = 0, mins_trabajo = 0;
    int total_mins = 0;
    int urgentes = 0;

    for (size_t i = 0; i < n; i++) {
        total_mins += tasks[i].duration_min;
        if (tasks[i].importance >= 4) urgentes++;

        if (strcmp(tasks[i].category, "UNIVERSIDAD") == 0) 
            mins_uni += tasks[i].duration_min;
        
        // AQUI ESTA LA CORRECCION: Agregamos HOGAR a la suma de vida
        else if (strcmp(tasks[i].category, "PERSONAL") == 0 || strcmp(tasks[i].category, "SALUD") == 0 || strcmp(tasks[i].category, "HOGAR") == 0 || strcmp(tasks[i].category, "MANDADOS") == 0) 
            mins_vida += tasks[i].duration_min;
        
        else if (strcmp(tasks[i].category, "TRABAJO") == 0) 
            mins_trabajo += tasks[i].duration_min;
        
        else 
            mins_ocio += tasks[i].duration_min;
    }

    printf(COL_CYAN "   📊 REPORTE DE EQUILIBRIO DE VIDA:\n" COL_RESET);
    printf("   --------------------------------------\n");

    void dibujar_barra(const char* label, int val, int total, const char* color) {
        int porcentaje = (total > 0) ? (val * 100 / total) : 0;
        int bar_len = porcentaje / 2;
        
        printf("   %-12s |", label);
        printf("%s", color);
        for(int i=0; i<bar_len; i++) printf(BAR_CHAR);
        printf(COL_RESET " %d%% (%d min)\n", porcentaje, val);
    }

    dibujar_barra("ESTUDIO", mins_uni, total_mins, COL_RED);
    dibujar_barra("TRABAJO", mins_trabajo, total_mins, COL_YELLOW);
    dibujar_barra("VIDA/HOGAR", mins_vida, total_mins, COL_GREEN);
    dibujar_barra("OTROS", mins_ocio, total_mins, COL_CYAN);

    printf("\n");

    printf(COL_YELLOW "   🤖 DIAGNOSTICO DE LA IA:\n   " COL_RESET);
    
    char mensaje[512];
    
    if (urgentes > n/2 && n > 2) {
        sprintf(mensaje, "CRITICO: Estas viviendo al limite. El 50%% de tus tareas son urgentes.\n   Mi recomendacion: Deja de procrastinar o vas a sufrir un colapso nervioso.");
    } 
    else if (mins_uni > total_mins * 0.7) {
        sprintf(mensaje, "ALERTA DE BURNOUT: Estas dedicando demasiada vida a la Universidad.\n   Prescripcion: Necesitas agendar 'Ir al parque' o 'Dormir' urgentemente.");
    }
    else if (mins_ocio > total_mins * 0.6) {
        sprintf(mensaje, "DETECTADA PEREZA EXTREMA: Tu agenda parece vacaciones.\n   Ponte a trabajar antes de que repruebes el semestre.");
    }
    else if (total_mins > 600) { 
        sprintf(mensaje, "MODO MAQUINA: Tienes mas de 10 horas de trabajo hoy.\n   Eres productivo, pero recuerda hidratarte, humano.");
    }
    else {
        sprintf(mensaje, "ESTADO OPTIMO: Tienes un equilibrio saludable entre vida y deberes.\n   Sigue asi, estoy orgulloso de mi creador.");
    }

    print_typing(mensaje, 30);

    printf("\n   --------------------------------------\n");
    printf("   Presiona Enter para cerrar el reporte...");
    getchar(); getchar();
}

void asistente_inteligente(Task tasks[], size_t n) {
    int minutos_disponibles;
    printf("\n   🤖 IA: Cuantos minutos tienes libres AHORA MISMO? > ");
    if (scanf("%d", &minutos_disponibles) != 1) return;
    
    scheduler_recompute(tasks, n);
    scheduler_sort_by_score(tasks, n);

    printf("\n   🧠 RECOMENDACION RAPIDA:\n");
    printf("   ------------------------------------------------\n");
    
    int encontrado = 0;
    for (size_t i = 0; i < n; i++) {
        if (tasks[i].duration_min <= minutos_disponibles && tasks[i].final_score > 50) {
            printf("   ✅ HAZ ESTO: %s (%s)\n", tasks[i].title, tasks[i].category);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado && n > 0) {
        printf("   ⚡ AVANCE: Dedica %d min a '%s' (%s)\n", minutos_disponibles, tasks[0].title, tasks[0].category);
    }
    if (n==0) printf("   🎉 Nada pendiente.\n");
    
    printf("   ------------------------------------------------\n");
    printf("   Enter...");
    getchar(); getchar();
}

static void print_menu() {
    printf("\n");
    printf("   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║           SISTEMA INTEGRAL (IA v5.0 - FINAL)       ║\n");
    printf("   ╠════════════════════════════════════════════════════╣\n");
    printf("   ║  1. Agregar Actividad                              ║\n");
    printf("   ║  2. Ver Tareas                                     ║\n");
    printf("   ║  3. Analisis Rapido                                ║\n");
    printf("   ║  4. Analisis Profundo de Productividad             ║\n");
    printf("   ║  5. Ver y Exportar Reporte HTML                    ║\n");
    printf("   ║  6. Salir                                          ║\n");
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
            printf("\n   --- AGENDA ---\n");
            printf("   %-20s | %-12s | %-5s | %s\n", "Actividad", "Categoria", "Mins", "Estado");
            printf("   ------------------------------------------------------\n");
            for (size_t i = 0; i < n; i++) {
                printf("   %-20s | %-12s | %-5d | %s\n", 
                    tasks[i].title, tasks[i].category, tasks[i].duration_min, 
                    (tasks[i].importance >= 4 ? "URGENTE" : "Normal"));
            }
            printf("\n   Enter...");
            getchar();
            clear_screen();
        }
        else if (op == 3) {
            clear_screen();
            if (n == 0) printf("\n   [!] Vacio.\n");
            else asistente_inteligente(tasks, n);
            clear_screen();
        }
        else if (op == 4) {
            ia_diagnostico_profundo(tasks, n);
            clear_screen();
        }
        else if (op == 5) {
            clear_screen();
            if (n == 0) {
                printf("\n   [!] No hay datos para exportar.\n");
            } else {
                scheduler_recompute(tasks, n);
                scheduler_sort_by_score(tasks, n);
                if (exportar_reporte_html(tasks, n)) {
                    printf("\n   📄 Reporte generado correctamente.\n");
                    #ifdef _WIN32
                        system("start Plan_Inteligente.html");
                    #endif
                }
            }
            printf("   Enter...");
            getchar();
            clear_screen();
        }
        else if (op == 6) break;
    }
    return 0;
}