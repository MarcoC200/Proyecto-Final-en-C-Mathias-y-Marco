#include <stdio.h>
#include <string.h>
#include "../include/heuristics.h"
#include "../include/utils.h"

double heuristic_score(Task* t) {
    if (!t) return 0.0;

    int dleft = days_until(t->due);

    double s = 0.0;


    s += t->importance * 30.0; 


    s -= (t->difficulty * 10.0);


    if (t->importance >= 4 && t->difficulty <= 2) {
        s += 40.0; 
    }

    if (dleft <= 0) s += 500.0;       
    else if (dleft == 1) s += 200.0;  
    else if (dleft <= 3) s += 100.0;  
    else if (dleft <= 7) s += 50.0;
    else {
   
        s += (100.0 / (dleft + 1));
    }

    if (t->duration_min <= 15) s += 15.0;
    else if (t->duration_min > 180) s -= 20.0; 

    t->final_score = s;
    return s;
}

void heuristic_explain(Task* t) {
    if (!t) return;

    int dleft = days_until(t->due);
    char tmp[REASON_MAX];
    tmp[0] = '\0';


    if (dleft <= 1) {
        strcat(tmp, "🚨 CRÍTICO: Vence inmediatamente. ");
    }

    else if (t->importance >= 4 && t->difficulty <= 2) {
        strcat(tmp, "💎 QUICK WIN: Alto valor y poco esfuerzo. ");
    }

    else if (t->importance <= 2 && t->difficulty >= 4) {
        strcat(tmp, "🐢 DESGASTE: Difícil y aporta poco (Postergar). ");
    }

    else if (t->importance >= 4 && t->difficulty >= 4) {
        strcat(tmp, "🐘 RETO: Importante pero requiere mucha energía. ");
    }
    else {
        strcat(tmp, "Prioridad estándar calculada. ");
    }

    if (dleft > 1) {
        char buf[64];
        if (dleft < 365) 
            snprintf(buf, sizeof(buf), "(Vence en %d dias)", dleft);
        else 
            strcpy(buf, "(Sin fecha cercana)");
        
        strcat(tmp, buf);
    }

    strncpy(t->reason, tmp, REASON_MAX - 1);
    t->reason[REASON_MAX - 1] = '\0';
}