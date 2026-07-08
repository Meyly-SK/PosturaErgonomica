# Guía para crear escenarios — ErgoSim 3D

Esta carpeta contiene un archivo `.h` por cada postura/escenario del simulador.
Cada integrante crea **su propio archivo** para evitar conflictos de merge.

---

## Articulaciones disponibles

Todas se especifican como **ángulos en grados** dentro de `ScenarioData`:

| Campo | Articulación | 0° | Positivo | Rango útil |
|---|---|---|---|---|
| `anguloBrazoDer` | Hombro derecho | Brazo colgando | Sube el brazo | 0–180° |
| `anguloBrazoIzq` | Hombro izquierdo | Brazo colgando | Sube el brazo | 0–180° |
| `anguloCodoDer` | Codo derecho | Antebrazo extendido | Dobla el antebrazo | 0–150° |
| `anguloCodoIzq` | Codo izquierdo | Antebrazo extendido | Dobla el antebrazo | 0–150° |
| `anguloMusloDer` | Cadera derecha | De pie | Muslo hacia adelante (sentarse) | 0–90° |
| `anguloMusloIzq` | Cadera izquierda | De pie | Muslo hacia adelante | 0–90° |
| `anguloRodilla` | Ambas rodillas | Pierna extendida | Pantorrilla hacia atrás | 0–90° |
| `anguloTorso` | Columna lumbar | Recto | Inclinar torso adelante | 0–60° |
| `anguloCuello` | Cuello | Recto | Mirar hacia abajo | -30–45° |
| `anguloLateralCuello` | Cuello lateral | Recto | Inclinar hacia hombro der. | -30–30° |

**Variables ergonómicas adicionales:**

| Campo | Descripción | Unidad |
|---|---|---|
| `pesoCarga` | Peso que sostiene la persona | kg |
| `distanciaCarga` | Distancia de la carga al cuerpo | metros |
| `tiempoExposicion` | Tiempo en esta postura | minutos |

---

## Plantilla para un escenario nuevo

```cpp
#pragma once
#include "../Types.h"

inline ScenarioData crearEscenarioMiPostura()
{
    ScenarioData s;
    s.nombre      = "Nombre legible del escenario";
    s.descripcion = "Descripcion breve del riesgo ergonomico.";

    // Articulaciones (solo los que necesitas cambiar, el resto queda en 0)
    s.anguloBrazoDer   = 0.0f;
    s.anguloBrazoIzq   = 0.0f;
    s.anguloCodoDer    = 0.0f;
    s.anguloCodoIzq    = 0.0f;
    s.anguloMusloDer   = 0.0f;
    s.anguloMusloIzq   = 0.0f;
    s.anguloRodilla    = 0.0f;
    s.anguloTorso      = 0.0f;
    s.anguloCuello     = 0.0f;

    // Variables ergonomicas
    s.pesoCarga        = 0.0f;
    s.distanciaCarga   = 0.0f;
    s.tiempoExposicion = 0.0f;

    return s;
}
```

---

## Cómo registrar tu escenario (2 pasos)

### Paso 1: Crear tu archivo
Guarda el archivo como `Escenario_NombrePostura.h` en esta carpeta.

### Paso 2: Registrar en ScenarioManager
Abre `src/simulation/ScenarioManager.cpp` y agrega en los comentarios marcados con `PUNTO DE EXTENSIÓN`:

```cpp
// Al inicio del archivo:
#include "escenarios/Escenario_NombrePostura.h"

// Dentro de inicializarEscenariosDefault():
mEscenarios.push_back(crearEscenarioNombrePostura());
```

---

## Ejemplos de posturas completas

### Postura sentado correcto

```cpp
s.nombre        = "Sentado correcto";
s.anguloMusloDer = 90.0f;   // muslos horizontales
s.anguloMusloIzq = 90.0f;
s.anguloRodilla  = 90.0f;   // pantorrillas verticales hacia abajo
s.anguloCodoDer  = 90.0f;   // codos apoyados en escritorio
s.anguloCodoIzq  = 90.0f;
```

### Levantamiento incorrecto (riesgo lumbar alto)

```cpp
s.nombre         = "Levantamiento incorrecto";
s.anguloTorso    = 45.0f;   // torso inclinado
s.anguloBrazoDer = 30.0f;   // brazos extendidos
s.anguloBrazoIzq = 30.0f;
s.pesoCarga      = 15.0f;
s.distanciaCarga = 0.5f;
s.tiempoExposicion = 2.0f;
```

### Trabajo en pantalla (cuello flexionado)

```cpp
s.nombre         = "Trabajo frente a pantalla";
s.anguloCuello   = 25.0f;   // cuello inclinado hacia abajo
s.anguloCodoDer  = 90.0f;
s.anguloCodoIzq  = 90.0f;
s.tiempoExposicion = 120.0f;
```

---

## Escenarios existentes

| Archivo | Postura | Responsable |
|---|---|---|
| `Escenario_Neutro.h` | Postura de referencia (todos en 0°) | base |
| `Escenario_BrazoDerecho90.h` | Brazo derecho 90° (evidencia) | base |
| _(agrega el tuyo aquí)_ | | |
