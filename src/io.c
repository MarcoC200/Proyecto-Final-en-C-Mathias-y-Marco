#include <stdio.h>
#include <string.h>
#include "io.h"
#include "utils.h"
#include "heuristics.h"

static int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

int io_read_task(Task* out, int next_id) {
    if (!out) return 0;

    Task t;
    memset(&t, 0, sizeof(t));
    t.id = next_id;

    char buf[128];

    printf("Titulo de la tarea: ");
    if (!read_line(t.title, sizeof(t.title))) return 0;

    printf("Importancia (1-5): ");
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.importance)) return 0;
    t.importance = clamp(t.importance, 1, 5);

    printf("Urgencia (1-5): ");
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.urgency)) return 0;
    t.urgency = clamp(t.urgency, 1, 5);

    printf("Duracion (min): ");
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_int(buf, &t.duration_min)) return 0;
    if (t.duration_min <= 0) t.duration_min = 1;

    printf("Fecha limite (YYYY-MM-DD): ");
    if (!read_line(buf, sizeof(buf))) return 0;
    if (!parse_date_yyyy_mm_dd(buf, &t.due)) return 0;

    heuristic_score(&t);
    heuristic_explain(&t);

    *out = t;
    return 1;
}

// Formato por línea en el archivo:
// titulo|importancia|urgencia|duracion_min|YYYY-MM-DD
int io_load_tasks_from_file(const char* path, Task* tasks, size_t cap, size_t* out_n, int* out_next_id) {
    if (!path || !tasks || !out_n || !out_next_id) return 0;

    FILE* f = fopen(path, "r");
    if (!f) return 0;

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
        int imp=0, urg=0, dur=0;
        char datebuf[32];

        // parse simple con sscanf (título sin |)
        if (sscanf(line, "%79[^|]|%d|%d|%d|%31s", title, &imp, &urg, &dur, datebuf) != 5) {
            continue;
        }

        strncpy(t.title, title, TITLE_MAX - 1);
        t.importance = imp;
        t.urgency = urg;
        t.duration_min = dur;

        if (!parse_date_yyyy_mm_dd(datebuf, &t.due)) continue;

        heuristic_score(&t);
        heuristic_explain(&t);

        tasks[n++] = t;
    }

    fclose(f);
    *out_n = n;
    return 1;
}
