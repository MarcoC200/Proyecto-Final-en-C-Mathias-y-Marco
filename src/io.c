#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include "../include/io.h"
#include "../include/utils.h"
#include "../include/heuristics.h"

#define COLOR_CYAN   "\033[36m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_RED    "\033[31m"  
#define COLOR_RESET  "\033[0m"
#define COLOR_BOLD   "\033[1m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_MAGENTA "\033[35m"

void to_lower_str(const char* src, char* dest) {
    for (int i = 0; src[i]; i++) dest[i] = tolower(src[i]);
    dest[strlen(src)] = '\0';
}

void sumar_minutos(int* hora, int* min, int duracion) {
    *min += duracion;
    while (*min >= 60) {
        *min -= 60;
        *hora += 1;
    }
    if (*hora >= 24) *hora -= 24;
}

void detectar_categoria(const char* titulo_raw, char* out_cat, int* out_urgencia) {
    char t[TITLE_MAX];
    to_lower_str(titulo_raw, t); 

    if (strstr(t, "mecanic") || strstr(t, "carro") || strstr(t, "auto") || strstr(t, "taller") || strstr(t, "gasolina") || strstr(t, "lavadora")) {
        strcpy(out_cat, "VEHICULO");
    }
    else if (strstr(t, "medico") || strstr(t, "doctor") || strstr(t, "hospital") || strstr(t, "cita") || strstr(t, "dentista") || strstr(t, "pastilla") || strstr(t, "terapia") || strstr(t, "dolor")) {
        strcpy(out_cat, "SALUD");
    }
    else if (strstr(t, "comprar") || strstr(t, "super") || strstr(t, "tienda") || strstr(t, "pagar") || strstr(t, "banco") || strstr(t, "deposito") || strstr(t, "transferencia") || strstr(t, "cobrar")) {
        strcpy(out_cat, "MANDADOS");
    }
    else if (strstr(t, "gym") || strstr(t, "ejercicio") || strstr(t, "entrenar") || strstr(t, "correr") || strstr(t, "futbol") || strstr(t, "caminar") || strstr(t, "partido") || strstr(t, "gimnasio")) {
        strcpy(out_cat, "DEPORTE");
    }
    else if (strstr(t, "calculo") || strstr(t, "deber") || strstr(t, "estudiar") || strstr(t, "examen") || strstr(t, "proyecto") || strstr(t, "tesis") || strstr(t, "clase") || strstr(t, "universidad") || strstr(t, "leccion") || strstr(t, "grupo") || strstr(t, "exposicion") || strstr(t, "fisica") || strstr(t, "quimica") || strstr(t, "mate")) {
        strcpy(out_cat, "UNIVERSIDAD");
    }
    else if (strstr(t, "reunion") || strstr(t, "informe") || strstr(t, "cliente") || strstr(t, "trabajo") || strstr(t, "jefe") || strstr(t, "correo") || strstr(t, "llamada") || strstr(t, "presentacion") || strstr(t, "oficina")) {
        strcpy(out_cat, "TRABAJO");
    }
    else if (strstr(t, "casa") || strstr(t, "limpiar") || strstr(t, "lavar") || strstr(t, "cocinar") || strstr(t, "almuerzo") || strstr(t, "desayuno") || strstr(t, "cena") || strstr(t, "merienda") || strstr(t, "platos") || strstr(t, "ropa") || strstr(t, "perro") || strstr(t, "gato") || strstr(t, "barrer") || strstr(t, "cuarto") || strstr(t, "hogar")) {
        strcpy(out_cat, "HOGAR");
    }
    else {
        strcpy(out_cat, "GENERAL"); 
    }

    if (strstr(t, "urgente") || strstr(t, "ahora") || strstr(t, "hoy") || strstr(t, "vence") || strstr(t, "examen")) {
        *out_urgencia = 5;
    } else {
        *out_urgencia = 0;
    }
}

int exportar_reporte_html(Task tasks[], size_t n) {
    FILE *f = fopen("Plan_Inteligente.html", "w");
    if (!f) return 0;

    time_t now = time(NULL);
    struct tm *t_now = localtime(&now);
    int hora_actual = t_now->tm_hour;
    int min_actual = t_now->tm_min;
    
    if (hora_actual < 8) { hora_actual = 8; min_actual = 0; }

    fprintf(f, "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'>");
    fprintf(f, "<title>Smart Schedule AI</title>");
    fprintf(f, "<style>");
    fprintf(f, ":root { --bg: #f3f4f6; --primary: #3b82f6; --text: #1f2937; }");
    fprintf(f, "body { font-family: 'Segoe UI', sans-serif; background: var(--bg); color: var(--text); padding: 40px; }");
    fprintf(f, ".container { max-width: 800px; margin: 0 auto; background: white; padding: 40px; border-radius: 20px; box-shadow: 0 10px 25px -5px rgba(0,0,0,0.1); }");
    
    fprintf(f, "h1 { font-size: 2rem; margin-bottom: 5px; color: #111827; }");
    fprintf(f, ".subtitle { color: #6b7280; margin-bottom: 30px; font-size: 1.1rem; }");
    
    fprintf(f, ".timeline { position: relative; border-left: 3px solid #e5e7eb; margin-left: 20px; padding-left: 30px; }");
    fprintf(f, ".time-slot { margin-bottom: 25px; position: relative; }");
    fprintf(f, ".time-slot::before { content: ''; position: absolute; left: -39px; top: 5px; width: 15px; height: 15px; border-radius: 50%%; background: white; border: 3px solid var(--primary); }");
    
    fprintf(f, ".time-label { font-size: 0.9rem; font-weight: 700; color: #9ca3af; margin-bottom: 5px; }");
    fprintf(f, ".task-card { background: #f8fafc; border: 1px solid #e2e8f0; border-radius: 10px; padding: 15px; display: flex; justify-content: space-between; align-items: center; }");
    fprintf(f, ".task-card:hover { border-color: var(--primary); box-shadow: 0 4px 6px -1px rgba(0,0,0,0.05); }");
    
    fprintf(f, ".break-card { background: #ecfdf5; border: 1px dashed #34d399; color: #065f46; padding: 10px 15px; border-radius: 10px; font-size: 0.9rem; font-weight: 600; }");
    
    fprintf(f, ".tag { font-size: 0.75rem; text-transform: uppercase; font-weight: 800; padding: 3px 8px; border-radius: 4px; letter-spacing: 0.5px; }");
    fprintf(f, ".tag-univ { background: #dbeafe; color: #1e40af; }");
    fprintf(f, ".tag-vida { background: #fce7f3; color: #9d174d; }");
    fprintf(f, ".tag-work { background: #fef3c7; color: #92400e; }");

    fprintf(f, "</style></head><body>");
    
    fprintf(f, "<div class='container'>");
    fprintf(f, "<h1>📅 Tu Horario Inteligente (AI)</h1>");
    fprintf(f, "<div class='subtitle'>Generado automaticamente segun prioridades y carga cognitiva.</div>");
    
    fprintf(f, "<div class='timeline'>");

    int h = hora_actual;
    int m = min_actual;

    for(size_t i=0; i<n; i++) {
        if(tasks[i].importance >= 3 || tasks[i].final_score > 50) {
            
            fprintf(f, "<div class='time-slot'>");
            fprintf(f, "<div class='time-label'>%02d:%02d</div>", h, m);
            
            char* tag_class = "tag-work";
            if(strcmp(tasks[i].category, "UNIVERSIDAD")==0) tag_class = "tag-univ";
            else if(strcmp(tasks[i].category, "PERSONAL")==0 || strcmp(tasks[i].category, "HOGAR")==0) tag_class = "tag-vida";

            fprintf(f, "<div class='task-card'>");
            fprintf(f, "<div><div style='font-weight:700; font-size:1.1rem;'>%s</div>", tasks[i].title);
            fprintf(f, "<span class='tag %s'>%s</span></div>", tag_class, tasks[i].category);
            fprintf(f, "<div style='font-weight:bold; color:#4b5563;'>%d min</div>", tasks[i].duration_min);
            fprintf(f, "</div></div>"); 
            
            sumar_minutos(&h, &m, tasks[i].duration_min);

            if(tasks[i].duration_min > 50) {
                fprintf(f, "<div class='time-slot' style='margin-top:10px;'>");
                fprintf(f, "<div class='time-label'>%02d:%02d</div>", h, m);
                fprintf(f, "<div class='break-card'>☕ IA Suggestion: Brain Break (15 min)</div>");
                fprintf(f, "</div>");
                sumar_minutos(&h, &m, 15);
            }
        }
    }

    fprintf(f, "<div class='time-slot'>");
    fprintf(f, "<div class='time-label'>%02d:%02d</div>", h, m);
    fprintf(f, "<div style='color:#9ca3af; font-style:italic;'>🏁 Fin de la jornada productiva.</div>");
    fprintf(f, "</div>");

    fprintf(f, "</div></div></body></html>");
    fclose(f);
    return 1;
}

void print_separator() {
    printf(COLOR_CYAN "   ╠════════════════════════════════════════════════════╣\n" COLOR_RESET);
}

int io_read_task(Task* out, int next_id) {
    if (!out) return 0;
    Task t; memset(&t, 0, sizeof(t)); t.id = next_id;
    strcpy(t.category, "GENERAL");
    char buf[128], input_lower[128];

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n");
    printf(COLOR_CYAN "   ╔════════════════════════════════════════════════════╗\n");
    printf("   ║              NUEVA ACTIVIDAD (ASISTENTE IA)        ║\n" COLOR_RESET);
    print_separator();

    printf(COLOR_CYAN "   ║" COLOR_RESET "  📝 Que tienes que hacer? : ");
    if (!read_line(t.title, sizeof(t.title))) return 0;

    int urgencia_ia = 0;
    detectar_categoria(t.title, t.category, &urgencia_ia);

    printf(COLOR_MAGENTA "   🤖 IA: Clasificado como %s.\n" COLOR_RESET, t.category);

    printf(COLOR_CYAN "   ║" COLOR_RESET "  ⏳ Tiempo aprox (mins)   : ");
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.duration_min)) t.duration_min = 45;

    while (1) {
        printf(COLOR_CYAN "   ║" COLOR_RESET "  📅 Para cuando? (Ej: 'Hoy', 'Mañana', '23 1'): ");
        if (!read_line(buf, sizeof(buf))) return 0;
        to_lower_str(buf, input_lower);
        
        time_t now = time(NULL); 
        struct tm *tm_now = localtime(&now);
        t.due = *tm_now; t.due.tm_sec = 0; t.due.tm_min = 0; t.due.tm_hour = 0;
        
        int fecha_valida = 0;
        int dias_diferencia = 0; 

        if (strstr(input_lower, "hoy") != NULL) {
            fecha_valida = 1;
            dias_diferencia = 0;
        }
        else if (strstr(input_lower, "ana") != NULL) { 
            t.due.tm_mday += 1; 
            mktime(&t.due); 
            fecha_valida = 1;
            dias_diferencia = 1;
        } 
        else {
            int d, m;
            if (sscanf(buf, "%d %d", &d, &m) == 2) {
                t.due.tm_year = 2026 - 1900; t.due.tm_mon = m - 1; t.due.tm_mday = d;
                mktime(&t.due); 
                fecha_valida = 1;
                dias_diferencia = d - tm_now->tm_mday + (m - (tm_now->tm_mon + 1))*30;
            }
        }

        if (fecha_valida) {
            sprintf(t.deadline_str, "%04d-%02d-%02d", t.due.tm_year + 1900, t.due.tm_mon + 1, t.due.tm_mday);
            
            if (urgencia_ia > 0) t.importance = 5; 
            else if (dias_diferencia <= 1) t.importance = 5; 
            else if (dias_diferencia <= 3) t.importance = 4; 
            else t.importance = 3; 

            
            break;
        } else {
            printf(COLOR_RED "   ║  [X] No te entendi. Prueba 'Hoy' o '23 1'.\n" COLOR_RESET);
        }
    }
    
    printf("\n   " COLOR_GREEN "✅ Actividad guardada.\n" COLOR_RESET);
    getchar();
    heuristic_score(&t); *out = t; return 1;
}

int io_load_tasks_from_file(const char* path, Task* tasks, size_t cap, size_t* out_n, int* out_next_id) {
    (void)path; (void)tasks; (void)cap; (void)out_n; (void)out_next_id;
    return 0; 
}