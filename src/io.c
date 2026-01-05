#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io.h"
#include "../include/utils.h"
#include "../include/heuristics.h"

#define COLOR_CYAN   "\033[36m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_RED    "\033[31m"  
#define COLOR_RESET  "\033[0m"
#define COLOR_BOLD   "\033[1m"
#define COLOR_YELLOW "\033[33m"

static int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void print_separator() {
    printf(COLOR_CYAN "   ╠════════════════════════════════════════════════════╣\n" COLOR_RESET);
}

int io_read_task(Task* out, int next_id) {
    if (!out) return 0;

    Task t;
    memset(&t, 0, sizeof(t));
    t.id = next_id;

    char buf[128];

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n");
    printf(COLOR_CYAN "   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║                  NUEVA TAREA                       ║\n" COLOR_RESET);
    print_separator();

    printf(COLOR_CYAN "   ║" COLOR_RESET "  📝 Tarea        : ");
    if (!read_line(t.title, sizeof(t.title))) return 0;

    printf(COLOR_CYAN "   ║" COLOR_RESET "  ⭐ Importancia  : ");
    printf(COLOR_YELLOW "(1=Poco ... 5=Vital) " COLOR_RESET);
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.importance)) t.importance = 1; 
    t.importance = clamp(t.importance, 1, 5);

    printf(COLOR_CYAN "   ║" COLOR_RESET "  🧠 Dificultad   : ");
    printf(COLOR_YELLOW "(1=Fácil ... 5=Difícil) " COLOR_RESET);
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.difficulty)) t.difficulty = 1; 
    t.difficulty = clamp(t.difficulty, 1, 5);

    printf(COLOR_CYAN "   ║" COLOR_RESET "  ⏳ Duración     : ");
    printf(COLOR_YELLOW "(en minutos) " COLOR_RESET);
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.duration_min)) t.duration_min = 30;
    if (t.duration_min <= 0) t.duration_min = 1;

    while (1) {
        printf(COLOR_CYAN "   ║" COLOR_RESET "  📅 Vence        : ");
        printf(COLOR_YELLOW "(YYYY-MM-DD del 2026) " COLOR_RESET);
        
        if (!read_line(buf, sizeof(buf))) return 0;

        if (parse_date_yyyy_mm_dd(buf, &t.due)) {
            int year = t.due.tm_year + 1900;
            
            if (year == 2026) {
                strncpy(t.deadline_str, buf, sizeof(t.deadline_str) - 1);
                break; 
            } else {
                printf(COLOR_RED "   ║  [X] Error: Solo se permite el año 2026.\n" COLOR_RESET);
            }
        } else {
            printf(COLOR_RED "   ║  [X] Error: Formato invalido. Usa Año-Mes-Dia.\n" COLOR_RESET);
        }
    }

    printf(COLOR_CYAN "   ║                                                    ║\n");
    printf("   ╚════════════════════════════════════════════════════╝\n" COLOR_RESET);
    
    printf("\n   " COLOR_GREEN "✅ Tarea guardada correctamente.\n" COLOR_RESET);
    printf("   Presiona Enter para continuar...");
    getchar();

    heuristic_score(&t);
    heuristic_explain(&t);

    *out = t;
    return 1;
}

int io_load_tasks_from_file(const char* path, Task* tasks, size_t cap, size_t* out_n, int* out_next_id) {
    if (!path || !tasks || !out_n || !out_next_id) return 0;

    FILE* f = fopen(path, "r");
    if (!f) {
        printf("\n   [!] Aviso: No se encontro el archivo %s (Se creara uno nuevo al guardar).\n", path);
        return 0;
    }

    char line[512];
    size_t n = 0;

    while (fgets(line, sizeof(line), f)) {
        if (n >= cap) break;
        trim_newline(line);
        if (line[0] == '\0') continue;

        Task t;
        memset(&t, 0, sizeof(t));
        t.id = (*out_next_id)++;

        char title[TITLE_MAX];
        int imp=0, diff=0, dur=0;
        char datebuf[32];
        int parsed = 0;

        if (!parsed && sscanf(line, "%79[^,],%d,%d,%d,%31s", title, &imp, &diff, &dur, datebuf) == 5) parsed = 1;
        
        if (!parsed && sscanf(line, "%79[^|]|%d|%d|%d|%31s", title, &imp, &diff, &dur, datebuf) == 5) parsed = 1;

        if (!parsed && sscanf(line, "%79[^;];%d;%d;%d;%31s", title, &imp, &diff, &dur, datebuf) == 5) parsed = 1;

        if (!parsed) {
            (*out_next_id)--;
            continue; 
        }

        strncpy(t.title, title, TITLE_MAX - 1);
        t.importance = imp;
        t.difficulty = diff;
        t.duration_min = dur;
        
        strncpy(t.deadline_str, datebuf, sizeof(t.deadline_str) - 1);
        parse_date_yyyy_mm_dd(datebuf, &t.due);

        heuristic_score(&t);
        heuristic_explain(&t);

        tasks[n++] = t;
    
    }

    fclose(f);
    *out_n = n;
    return 1;
}