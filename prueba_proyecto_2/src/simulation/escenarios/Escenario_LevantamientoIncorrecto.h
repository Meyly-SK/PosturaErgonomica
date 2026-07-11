#pragma once
#include "../Types.h"

// ==========================================================================
// Escenario_LevantamientoIncorrecto.h
//
// Levantamiento de carga del suelo con tecnica incorrecta.
// El error principal es doblar la espalda en vez de las rodillas.
//
// Postura natural resultante:
//   - Torso 45° hacia adelante (espalda doblada — CRITICO)
//   - Brazos extendidos al frente + ligeramente abiertos
//   - Codos doblados hacia abajo para tomar la carga
//   - Piernas casi rectas (deberia doblar rodillas)
//   - Cuello leve flexion para mirar la carga
//
// Zonas de riesgo:
//   Lumbar   → ROJO     (~70 pts): torso 45° + 20kg + 0.6m
//   Hombros  → AMARILLO (~45 pts): brazos al frente + carga
//   Cuello   → AMARILLO (~25 pts): cuello 15° flexionado
//   Rodillas → VERDE    (~ 5 pts): piernas casi rectas
// ==========================================================================
inline ScenarioData crearEscenarioLevantamientoIncorrecto()
{
    ScenarioData s;
    s.nombre      = "Levantamiento incorrecto";
    s.descripcion = "Levantamiento del suelo doblando la espalda. "
                    "Riesgo critico en zona lumbar.";

    // ---- Torso: inclinado hacia adelante (el error principal) ----
    s.anguloTorso     = 45.0f;   // espalda doblada 45° (INCORRECTO — deberia doblar rodillas)

    // ---- Cuello: leve flexion para mirar la carga ----
    s.anguloCuello    = 15.0f;

    // ---- Brazos: extendidos al frente + ligeramente abiertos ----
    // Combinacion de eje X (hacia adelante) y eje Z (laterales) = natural al tomar una caja
    s.anguloBrazoDerX = 35.0f;   // brazo derecho hacia adelante
    s.anguloBrazoIzqX = 35.0f;   // brazo izquierdo hacia adelante
    s.anguloBrazoDer  = 25.0f;   // ligeramente elevados/abiertos (eje Z)
    s.anguloBrazoIzq  = 25.0f;

    // ---- Codos: doblados tomando la carga (eje X = dobla hacia abajo) ----
    s.anguloCodoDerX  = 35.0f;   // codo derecho dobla hacia abajo/adelante
    s.anguloCodoIzqX  = 35.0f;   // codo izquierdo
    s.anguloCodoDer   = 10.0f;   // leve doblez lateral
    s.anguloCodoIzq   = 10.0f;

    // ---- Piernas casi rectas (postura INCORRECTA para levantar) ----
    s.anguloMusloDer  = 5.0f;
    s.anguloMusloIzq  = 5.0f;
    s.anguloRodillaDer = 10.0f;
    s.anguloRodillaIzq = 10.0f;

    // ---- Variables ergonómicas ----
    s.pesoCarga        = 20.0f;  // kg — carga pesada
    s.distanciaCarga   = 0.60f;  // metros del cuerpo
    s.tiempoExposicion = 15.0f;  // minutos

    return s;
}
