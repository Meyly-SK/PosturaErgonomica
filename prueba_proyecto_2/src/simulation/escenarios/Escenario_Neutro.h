#pragma once

#include "../Types.h"

// ==========================================================================
// Escenario_Neutro.h
// Postura de referencia: de pie, columna recta, brazos caídos.
// Todos los ángulos en 0.
// ==========================================================================
inline ScenarioData crearEscenarioNeutro()
{
    ScenarioData s;
    s.nombre      = "Postura neutra";
    s.descripcion = "De pie, columna recta, brazos relajados a los costados. "
                    "Sin carga. Postura de referencia ergonomica.";
    // Todos los angulos en 0 = postura base
    return s;
}
