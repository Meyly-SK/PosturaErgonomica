#pragma once

#include "../Types.h"

// ==========================================================================
// Escenario_LevantamientoIncorrecto.h
//
// POSTURA CRITICA — Levantamiento de carga con técnica incorrecta.
//
// Descripción ergonómica:
//   La persona levanta una carga pesada con el torso muy inclinado hacia
//   adelante y los brazos extendidos, en lugar de doblar las rodillas.
//   Esta es una de las posturas más peligrosas en ergonomía laboral.
//
// Zonas de riesgo esperadas:
//   Lumbar  → ROJO   (~74 pts): torso 50° + 20kg + distancia 0.6m
//   Hombros → ROJO   (~73 pts): brazos 60° + 20kg de carga
//   Cuello  → AMARILLO (~32 pts): cuello 20° inclinado para ver la carga
//   Rodillas → VERDE  (~7 pts): piernas casi rectas (postura incorrecta)
// ==========================================================================
inline ScenarioData crearEscenarioLevantamientoIncorrecto()
{
    ScenarioData s;
    s.nombre      = "Levantamiento incorrecto";
    s.descripcion = "Carga pesada levantada con torso inclinado y brazos extendidos. "
                    "Maximos riesgos en zona lumbar y hombros.";

    // ---- Articulaciones ----
    s.anguloTorso    = 50.0f;   // torso muy inclinado hacia adelante
    s.anguloCuello   = 20.0f;   // cuello flexionado para mirar la carga
    s.anguloBrazoDer = 60.0f;   // brazos extendidos al frente
    s.anguloBrazoIzq = 60.0f;
    s.anguloCodoDer  = 20.0f;   // codos ligeramente doblados
    s.anguloCodoIzq  = 20.0f;
    s.anguloMusloDer = 10.0f;   // piernas casi rectas (postura INCORRECTA)
    s.anguloMusloIzq = 10.0f;
    s.anguloRodilla  = 10.0f;

    // ---- Variables ergonómicas ----
    s.pesoCarga        = 20.0f; // kg — carga pesada
    s.distanciaCarga   = 0.60f; // metros — carga alejada del cuerpo
    s.tiempoExposicion = 15.0f; // minutos

    return s;
}
