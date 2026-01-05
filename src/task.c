#include <stdio.h>
#include <time.h> 
#include "../include/task.h"
#include "../include/utils.h"


#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"

int task_is_valid(const Task* t) {
    if (!t) return 0;
    if (t->importance < 1 || t->importance > 5) return 0;
    if (t->difficulty < 1 || t->difficulty > 5) return 0; 
    if (t->duration_min <= 0) return 0;

    if (t->due.tm_mday == 0) return 0;
    
    return 1;
}

void task_print(const Task* t) {
    if (!t) return;


    int dleft = days_until(t->due); 

    printf("\n");
    printf(COLOR_CYAN "   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║  ID: %-4d %-38s   ║\n", t->id, "DETALLES DE TAREA");
    printf("   ╠════════════════════════════════════════════════════╣\n" COLOR_RESET);
    
    printf("   ║  📝 Tarea        : " COLOR_YELLOW "%-30s" COLOR_RESET " ║\n", t->title);
    printf("   ║  ⭐ Importancia  : %d/5                          ║\n", t->importance);
    printf("   ║  🧠 Dificultad   : %d/5                          ║\n", t->difficulty);
    printf("   ║  ⏳ Duración     : %-3d min                       ║\n", t->duration_min);
    

    printf("   ║  📅 Vence        : %04d-%02d-%02d (%d días rest.)     ║\n", 
           t->due.tm_year + 1900, 
           t->due.tm_mon + 1, 
           t->due.tm_mday,
           dleft);

    printf(COLOR_CYAN "   ╠════════════════════════════════════════════════════╣\n" COLOR_RESET);
    

    if (t->final_score != 0) {
        printf("   ║  🤖 SCORE IA     : " COLOR_GREEN "%.2f" COLOR_RESET "                        ║\n", t->final_score);
        printf("   ║  💡 MOTIVO       : %-30s ║\n", t->reason);
    } else {
        printf("   ║  🤖 (Pendiente de análisis)                        ║\n");
    }
    
    printf(COLOR_CYAN "   ╚════════════════════════════════════════════════════╝\n" COLOR_RESET);
}


void task_print_compact(const Task* t) {
    if (!t) return;

    printf(" [%d] %-20s | Imp:%d Dif:%d | %04d-%02d-%02d | IA: %.1f\n",
           t->id, 
           t->title, 
           t->importance, 
           t->difficulty, 
           t->due.tm_year + 1900, 
           t->due.tm_mon + 1, 
           t->due.tm_mday,
           t->final_score);
}
