# Cómo agregar un escenario nuevo

Esta carpeta contiene un archivo `.h` por cada postura/escenario del simulador.

## Estructura de un escenario

Cada archivo debe seguir esta plantilla:

```cpp
#pragma once
#include "../Types.h"

inline ScenarioData crearEscenarioMiPostura()
{
    ScenarioData s;
    s.nombre      = "Nombre legible";
    s.descripcion = "Descripcion breve del escenario ergonomico.";

    // Angulos en grados (positivo = sube/flexiona hacia adelante)
    s.anguloBrazoDer   = 0.0f;
    s.anguloBrazoIzq   = 0.0f;
    s.anguloTorso      = 0.0f;
    s.anguloCuello     = 0.0f;
    s.anguloRodilla    = 0.0f;

    // Variables ergonomicas
    s.pesoCarga        = 0.0f;
    s.distanciaCarga   = 0.0f;
    s.tiempoExposicion = 0.0f;

    return s;
}
```

## Cómo registrar tu escenario

1. Crea tu archivo `Escenario_NombrePostura.h` en esta carpeta.
2. Abre `src/simulation/ScenarioManager.cpp`.
3. Agrega `#include "escenarios/Escenario_NombrePostura.h"`.
4. Dentro de `inicializarEscenariosDefault()`, agrega:
   ```cpp
   mEscenarios.push_back(crearEscenarioNombrePostura());
   ```
5. Listo. El escenario aparecerá al presionar N/P en la app.

## Escenarios existentes

| Archivo | Postura | Responsable |
|---|---|---|
| `Escenario_Neutro.h` | Postura base (todos en 0) | base |
| `Escenario_BrazoDerecho90.h` | Brazo derecho 90° horizontal | evidencia |
| _(tu archivo aquí)_ | _(tu postura)_ | _(tu nombre)_ |

## Nombres de partes disponibles para reference

Partes que `setScenario()` puede mover:
- `BrazoSupDer` / `BrazoSupIzq` ← via `anguloBrazoDer` / `anguloBrazoIzq`
- `AntebrazoDer` / `AntebrazoIzq` ← se mueven junto con el brazo
- `ManoDer` / `ManoIzq` ← se mueven junto con el brazo
- `Pecho` / `ZonaLumbar` / `Cadera` ← via `anguloTorso`
- `Cuello` / `Cabeza` ← via `anguloCuello`
- `MusloIzq/Der` / `PiernaIzq/Der` ← via `anguloRodilla`
