#include <stdio.h>
#include <string.h>
#include "heuristics.h"
#include "utils.h"

double heuristic_score(Task* t) {
    if (!t) return 0.0;

    int dleft = days_until(t->due);

    // Base por criterios principales
    double s = 0.0;
    s += t->importance * 18.0; // importancia pesa fuerte
    s += t->urgency * 22.0;    // urgencia pesa más

    // Fecha límite (mientras más cerca, más sube)
    if (dleft <= 0) s += 50.0;          // vencida o hoy -> super prioridad
    else if (dleft == 1) s += 40.0;
    else if (dleft <= 3) s += 30.0;
    else if (dleft <= 7) s += 18.0;
    else if (dleft <= 14) s += 10.0;
    else s += 3.0;

    // Duración: favorece tareas cortas
    if (t->duration_min <= 30) s += 18.0;
    else if (t->duration_min <= 60) s += 10.0;
    else if (t->duration_min <= 120) s += 3.0;
    else s -= 8.0; // muy larga

    // Penalización: larga y poco urgente
    if (t->duration_min > 120 && t->urgency <= 2) s -= 10.0;

    t->score = s;
    return s;
}

void heuristic_explain(Task* t) {
    if (!t) return;

    int dleft = days_until(t->due);
    char tmp[REASON_MAX];
    tmp[0] = '\0';

    // Motivos principales
    if (t->urgency >= 4) strcat(tmp, "Alta urgencia, ");
    else if (t->urgency == 3) strcat(tmp, "Urgencia media, ");
    else strcat(tmp, "Baja urgencia, ");

    if (t->importance >= 4) strcat(tmp, "alta importancia, ");
    else if (t->importance == 3) strcat(tmp, "importancia media, ");
    else strcat(tmp, "baja importancia, ");

    if (dleft <= 0) strcat(tmp, "fecha limite hoy o vencida, ");
    else {
        char buf[64];
        snprintf(buf, sizeof(buf), "fecha limite en %d dias, ", dleft);
        strcat(tmp, buf);
    }

    if (t->duration_min <= 30) strcat(tmp, "tarea corta.");
    else if (t->duration_min <= 60) strcat(tmp, "duracion moderada.");
    else if (t->duration_min <= 120) strcat(tmp, "tarea larga.");
    else strcat(tmp, "muy larga, baja prioridad si no es urgente.");

    strncpy(t->reason, tmp, REASON_MAX - 1);
    t->reason[REASON_MAX - 1] = '\0';
}
