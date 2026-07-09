#pragma once
#include "../Types.h"

// ==========================================================================
// Escenario_PiernaLateral.h
//
// Pierna derecha levantada 35° hacia el lado derecho (abducción lateral),
// con el pie derecho compensado para seguir mirando hacia adelante.
//
// Movimientos:
//   anguloMusloDer = 35° (eje Z) → pierna derecha abierta lateralmente
//   anguloPieDerY  = 35°          → pie derecho rota axialmente para mantener
//                                   el dedo apuntando al frente (compensa la apertura)
//
// Este es un movimiento clásico de abducción de cadera.
// En ergonomía se evalúa cuando se requiere alcance lateral con carga.
// ==========================================================================
inline ScenarioData crearEscenarioPiernaLateral()
{
    ScenarioData s;
    s.nombre      = "Pierna derecha lateral";
    s.descripcion = "Pierna derecha abierta 35 grados hacia el lado. "
                    "Pie compensado para mirar al frente. Riesgo en rodilla/cadera.";

    // ---- Pierna derecha abierta al lado DERECHO ----
    // Nota de convención: en setScenario(), la cadera derecha usa:
    //   setRotCad("PivCaderaDer", -anguloMusloDer, ...)
    // Con valor negativo (-35), el signo doble negativo da positivo → pierna va a la derecha visible.
    s.anguloMusloDer = -35.0f;  // negativo = pierna derecha hacia la derecha (abducción)

    // ---- Pie derecho compensado ----
    // El pie rota axialmente (eje Y) para que el dedo siga mirando al frente.
    // También invertimos la compensación acorde a la dirección real.
    s.anguloPieDerY  = -35.0f;  // compensación invertida

    // ---- Variables ergonómicas ----
    s.tiempoExposicion = 5.0f;

    return s;
}
