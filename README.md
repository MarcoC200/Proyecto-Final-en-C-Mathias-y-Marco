# Task Planner AI en C

Proyecto universitario que implementa un **planificador inteligente de tareas** usando el lenguaje **C**.  
El sistema analiza las tareas ingresadas por el usuario y **recomienda el orden óptimo de ejecución** según importancia, urgencia, duración y fecha límite.

---

## ¿Qué hace este proyecto?

Como usuario:

> Ingreso las tareas que tengo para hoy y el sistema me dice **cuál hacer primero y por qué**.

El programa actúa como un **asistente inteligente de organización**, ayudando a tomar decisiones cuando hay muchas tareas y poco tiempo.

---

## Objetivo del proyecto

- Aplicar conceptos de **Inteligencia Artificial simbólica** (heurísticas y reglas).
- Desarrollar un sistema funcional en **C**, sin librerías externas.
- Organizar un proyecto real con estructura profesional en GitHub.
- Simular la toma de decisiones humanas para priorizar tareas.

---

## ¿Dónde está la Inteligencia Artificial?

Este proyecto **no usa Machine Learning**.  
La IA está basada en:

- Heurísticas (reglas de decisión)
- Scoring inteligente
- Priorización multi-criterio
- Planificación automática

Cada tarea recibe un **puntaje (score)** calculado con reglas como:
- Mayor urgencia → mayor prioridad
- Fecha límite cercana → sube prioridad
- Tareas cortas → se priorizan
- Tareas largas y no urgentes → bajan

Esto permite que el sistema **razone y explique sus decisiones**.

---

## Ejemplo de uso

El usuario ve algo como:

```txt
PLANIFICADOR DE TAREAS

1. Agregar tarea
2. Ver tareas
3. Generar plan recomendado
4. Salir
```
```txt
## PLAN RECOMENDADO PARA HOY:

1. Estudiar cálculo
   Motivo: Alta urgencia y fecha límite cercana.

2. Hacer deber de historia
   Motivo: Prioridad media y corta duración.

3. Ordenar apuntes
   Motivo: No es urgente y puede esperar.
```
## Estrucutura del proyecto

```txt
task-planner-c/
├─ src/        # Código fuente (.c)
├─ include/    # Headers (.h)
├─ docs/       # Documentación del proyecto
├─ data/       # Datos de ejemplo (CSV / TXT)
├─ tests/      # Pruebas básicas
├─ Makefile    # Compilación
├─ .gitignore  # Archivos ignorados por Git
└─ README.md   # Este archivo
```




