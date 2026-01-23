#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include "../include/io.h"
#include "../include/utils.h"
#include "../include/heuristics.h"

#define TITLE_MAX 100

#define COLOR_CYAN   "\033[36m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_RED    "\033[31m"  
#define COLOR_RESET  "\033[0m"
#define COLOR_BOLD   "\033[1m"
#define COLOR_MAGENTA "\033[35m"

void to_lower_str(const char* src, char* dest) {
    for (int i = 0; src[i]; i++) dest[i] = tolower(src[i]);
    dest[strlen(src)] = '\0';
}

void sumar_tiempo_html(int *h, int *m, int dur) {
    *m += dur;
    while (*m >= 60) {
        *m -= 60;
        *h += 1;
    }
    if (*h >= 24) *h -= 24;
}

void imprimir_barra_html(FILE *f, const char* label, int val, int total, const char* color) {
    int pct = (total > 0) ? (val * 100) / total : 0;
    fprintf(f, "<div class='stat-row'><div class='label'><span>%s</span><span>%d%% (%d min)</span></div>", label, pct, val);
    fprintf(f, "<div class='bar-bg'><div class='bar-fill' style='width:%d%%; background:%s;'></div></div></div>", pct, color);
}

void escribir_css_comun(FILE *f) {
    fprintf(f, "<style>");
    fprintf(f, ":root { --bg: #f8fafc; --text: #1e293b; --primary: #3b82f6; --danger: #ef4444; --success: #22c55e; }");
    fprintf(f, "body { font-family: 'Segoe UI', system-ui, sans-serif; background: var(--bg); color: var(--text); padding: 40px; margin: 0; }");
    fprintf(f, ".container { max-width: 900px; margin: 0 auto; background: white; padding: 40px; border-radius: 16px; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1); }");
    fprintf(f, "h1 { font-size: 2rem; color: #0f172a; margin-bottom: 10px; }");
    fprintf(f, ".subtitle { color: #64748b; margin-bottom: 30px; }");
    fprintf(f, "</style>");
}

void detectar_categoria(const char* titulo_raw, char* out_cat, int* out_urgencia) {
    char t[TITLE_MAX];
    to_lower_str(titulo_raw, t); 

    if (strstr(t, "mecanic") || strstr(t, "carro") || strstr(t, "auto") || strstr(t, "taller") || strstr(t, "gasolina")) strcpy(out_cat, "VEHICULO");
    else if (strstr(t, "medico") || strstr(t, "doctor") || strstr(t, "hospital") || strstr(t, "cita") || strstr(t, "dentista") || strstr(t, "pastilla")) strcpy(out_cat, "SALUD");
    else if (strstr(t, "comprar") || strstr(t, "super") || strstr(t, "pagar") || strstr(t, "tienda") || strstr(t, "comida")) strcpy(out_cat, "MANDADOS");
    else if (strstr(t, "gym") || strstr(t, "ejercicio") || strstr(t, "futbol") || strstr(t, "entrenar")) strcpy(out_cat, "DEPORTE");
    else if (strstr(t, "calculo") || strstr(t, "deber") || strstr(t, "examen") || strstr(t, "tarea") || strstr(t, "fisica") || strstr(t, "quimica") || strstr(t, "universidad")) strcpy(out_cat, "UNIVERSIDAD");
    else if (strstr(t, "reunion") || strstr(t, "informe") || strstr(t, "trabajo") || strstr(t, "jefe")) strcpy(out_cat, "TRABAJO");
    else if (strstr(t, "casa") || strstr(t, "limpiar") || strstr(t, "cocinar") || strstr(t, "almuerzo") || strstr(t, "hogar")) strcpy(out_cat, "HOGAR");
    else strcpy(out_cat, "GENERAL"); 

    if (strstr(t, "urgente") || strstr(t, "ahora") || strstr(t, "hoy") || strstr(t, "examen")) *out_urgencia = 5;
    else *out_urgencia = 0;
}

int exportar_agenda_html(Task tasks[], size_t n) {
    FILE *f = fopen("Reporte_Agenda.html", "w");
    if (!f) return 0;

    fprintf(f, "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><title>Agenda IA</title>");
    escribir_css_comun(f);
    fprintf(f, "<style>");
    fprintf(f, "table { width: 100%%; border-collapse: collapse; margin-top: 20px; }");
    fprintf(f, "th { text-align: left; padding: 15px; background: #f1f5f9; color: #475569; font-weight: 700; text-transform: uppercase; font-size: 0.85rem; }");
    fprintf(f, "td { padding: 15px; border-bottom: 1px solid #e2e8f0; }");
    fprintf(f, "tr:hover { background: #f8fafc; }");
    fprintf(f, ".badge { padding: 5px 10px; border-radius: 20px; font-size: 0.75rem; font-weight: 700; text-transform: uppercase; }");
    fprintf(f, ".badge-urgente { background: #fef2f2; color: #dc2626; border: 1px solid #fecaca; }");
    fprintf(f, ".badge-normal { background: #f0fdf4; color: #16a34a; border: 1px solid #bbf7d0; }");
    fprintf(f, "</style></head><body>");

    fprintf(f, "<div class='container'>");
    fprintf(f, "<h1>📋 Dashboard de Tareas</h1>");
    fprintf(f, "<p class='subtitle'>Vista general de la memoria del sistema (%zu tareas activas)</p>", n);

    if (n == 0) {
        fprintf(f, "<div style='text-align:center; padding:40px; color:#94a3b8;'>No hay datos. La IA esta esperando inputs.</div>");
    } else {
        fprintf(f, "<table><thead><tr><th>Actividad</th><th>Categoria</th><th>Fecha Limite</th><th>Duracion</th><th>Estado IA</th></tr></thead><tbody>");
        for(size_t i=0; i<n; i++) {
            fprintf(f, "<tr>");
            fprintf(f, "<td style='font-weight:600;'>%s</td>", tasks[i].title);
            fprintf(f, "<td><span style='background:#e0f2fe; color:#0369a1; padding:2px 8px; border-radius:4px; font-size:0.8rem;'>%s</span></td>", tasks[i].category);
            fprintf(f, "<td>%s</td>", tasks[i].deadline_str);
            fprintf(f, "<td>%d min</td>", tasks[i].duration_min);
            
            if (tasks[i].importance >= 4)
                fprintf(f, "<td><span class='badge badge-urgente'>⚡ URGENTE</span></td>");
            else
                fprintf(f, "<td><span class='badge badge-normal'>✅ AL DIA</span></td>");
            fprintf(f, "</tr>");
        }
        fprintf(f, "</tbody></table>");
    }
    fprintf(f, "</div></body></html>");
    fclose(f);
    return 1;
}

int exportar_analisis_html(Task tasks[], size_t n) {
    FILE *f = fopen("Reporte_Analisis.html", "w");
    if (!f) return 0;

    int mins_academico=0, mins_deporte_salud=0, mins_hogar_mandados=0, mins_otros=0, total=0, urgentes=0;
    
    for(size_t i=0; i<n; i++) {
        total += tasks[i].duration_min;
        if(tasks[i].importance >= 4) urgentes++;
        
        if(strcmp(tasks[i].category, "UNIVERSIDAD")==0) 
            mins_academico += tasks[i].duration_min;
            
        else if(strcmp(tasks[i].category, "DEPORTE")==0 || strcmp(tasks[i].category, "SALUD")==0) 
            mins_deporte_salud += tasks[i].duration_min;
            
        else if(strcmp(tasks[i].category, "HOGAR")==0 || strcmp(tasks[i].category, "MANDADOS")==0 || strcmp(tasks[i].category, "VEHICULO")==0 || strcmp(tasks[i].category, "PERSONAL")==0) 
            mins_hogar_mandados += tasks[i].duration_min;
            
        else 
            mins_otros += tasks[i].duration_min;
    }

    fprintf(f, "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><title>Analisis IA</title>");
    escribir_css_comun(f);
    fprintf(f, "<style>");
    fprintf(f, ".stat-row { margin-bottom: 25px; }");
    fprintf(f, ".label { display: flex; justify-content: space-between; margin-bottom: 8px; font-weight: 600; font-size: 0.9rem; }");
    fprintf(f, ".bar-bg { background: #e2e8f0; height: 24px; border-radius: 12px; overflow: hidden; }");
    fprintf(f, ".bar-fill { height: 100%%; transition: width 1s ease; }");
    fprintf(f, ".card-ia { background: #fffbeb; border-left: 5px solid #f59e0b; padding: 20px; border-radius: 8px; margin-top: 40px; }");
    fprintf(f, ".card-title { color: #b45309; font-weight: 800; text-transform: uppercase; font-size: 0.8rem; margin-bottom: 10px; }");
    fprintf(f, ".card-body { color: #78350f; font-size: 1.1rem; line-height: 1.6; }");
    fprintf(f, "</style></head><body>");

    fprintf(f, "<div class='container'>");
    fprintf(f, "<h1>📊 Analisis de Productividad</h1>");
    fprintf(f, "<p class='subtitle'>Diagnostico profundo del motor heuristico</p>");

    if (total == 0) {
        fprintf(f, "<p>No hay datos suficientes para analizar.</p>");
    } else {
        imprimir_barra_html(f, "ACADEMICO", mins_academico, total, "#ef4444");
        imprimir_barra_html(f, "DEPORTE Y SALUD", mins_deporte_salud, total, "#eab308");
        imprimir_barra_html(f, "HOGAR Y LOGISTICA", mins_hogar_mandados, total, "#22c55e");
        imprimir_barra_html(f, "OTROS / VARIOS", mins_otros, total, "#3b82f6");

        char consejo[512];
        if (urgentes > n/2) sprintf(consejo, "DETECTADO RIESGO DE ESTRES. El 50%% de tus tareas son urgentes. <br><strong>Estrategia:</strong> Usa el metodo 'Snowball': elimina las tareas pequeñas primero.");
        else if (mins_academico > total * 0.6) sprintf(consejo, "CARGA ACADEMICA INTENSA. <br><strong>Estrategia:</strong> El motor recomienda pausas activas cada 50 minutos (Pomodoro) para evitar saturacion.");
        else if (mins_hogar_mandados > total * 0.5) sprintf(consejo, "DIA LOGISTICO. <br><strong>Estrategia:</strong> Agrupa tus salidas por zona geografica para ahorrar tiempo de transporte.");
        else sprintf(consejo, "BALANCE OPTIMO. <br><strong>Estrategia:</strong> Buen momento para adelantar trabajo futuro y reducir deuda tecnica.");

        fprintf(f, "<div class='card-ia'><div class='card-title'>💡 Estrategia Sugerida</div><div class='card-body'>%s</div></div>", consejo);
    }
    fprintf(f, "</div></body></html>");
    fclose(f);
    return 1;
}

int exportar_reporte_html(Task tasks[], size_t n) {
    FILE *f = fopen("Plan_Inteligente.html", "w");
    if (!f) return 0;

    time_t now = time(NULL);
    struct tm *t_now = localtime(&now);
    int h = (t_now->tm_hour < 8) ? 8 : t_now->tm_hour;
    int m = t_now->tm_min;

    fprintf(f, "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><title>Horario IA</title>");
    escribir_css_comun(f);
    fprintf(f, "<style>");
    fprintf(f, ".timeline { border-left: 3px solid #e2e8f0; margin-left: 20px; padding-left: 30px; }");
    fprintf(f, ".slot { position: relative; margin-bottom: 30px; }");
    fprintf(f, ".slot::before { content: ''; position: absolute; left: -38px; top: 5px; width: 14px; height: 14px; background: white; border: 3px solid #3b82f6; border-radius: 50%%; }");
    fprintf(f, ".time { font-weight: 700; color: #94a3b8; font-size: 0.9rem; margin-bottom: 5px; }");
    fprintf(f, ".card { background: white; border: 1px solid #e2e8f0; border-radius: 10px; padding: 15px; display: flex; justify-content: space-between; align-items: center; }");
    fprintf(f, ".break { background: #ecfdf5; border: 1px dashed #22c55e; color: #15803d; padding: 10px; border-radius: 8px; margin-top: 10px; font-size: 0.9rem; font-weight: 600; }");
    fprintf(f, "</style></head><body>");
    
    fprintf(f, "<div class='container'><h1>📅 Tu Horario del Dia</h1><p class='subtitle'>Time Blocking generado por IA</p><div class='timeline'>");
    
    for(size_t i=0; i<n; i++) {
        if(tasks[i].importance >= 3 || tasks[i].final_score > 50) {
            fprintf(f, "<div class='slot'><div class='time'>%02d:%02d</div>", h, m);
            fprintf(f, "<div class='card'><div><strong>%s</strong><br><small style='color:#64748b'>%s</small></div>", tasks[i].title, tasks[i].category);
            fprintf(f, "<div style='font-weight:bold'>%d min</div></div>", tasks[i].duration_min);
            
            sumar_tiempo_html(&h, &m, tasks[i].duration_min);

            if(tasks[i].duration_min > 50) {
                fprintf(f, "<div class='break'>☕ IA: Brain Break (15 min)</div>");
                sumar_tiempo_html(&h, &m, 15);
            }
            fprintf(f, "</div>");
        }
    }
    fprintf(f, "<div class='slot'><div class='time'>%02d:%02d</div><div style='color:#cbd5e1'>🏁 Fin del dia productivo</div></div>", h, m);
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
            fecha_valida = 1; dias_diferencia = 0;
        } else if (strstr(input_lower, "ana") != NULL) { 
            t.due.tm_mday += 1; mktime(&t.due); fecha_valida = 1; dias_diferencia = 1;
        } else {
            int d, m;
            if (sscanf(buf, "%d %d", &d, &m) == 2) {
                t.due.tm_year = 2026 - 1900; t.due.tm_mon = m - 1; t.due.tm_mday = d;
                mktime(&t.due); fecha_valida = 1;
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
            printf(COLOR_RED "   ║  [X] No te entendi.\n" COLOR_RESET);
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