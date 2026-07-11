#pragma once
#include "../Types.h"

// ==========================================================================
// Escenario_TrabajoComputadora.h
//
// Postura de trabajo en computadora con mala ergonomia.
//
// Descripcion:
//   La persona esta sentada frente a la computadora con:
//   - Cuello inclinado hacia adelante ("cuello de tortuga")
//   - Torso ligeramente encorvado
//   - Hombros levemente elevados por tension
//   - Codos doblados en posicion de teclear
//   - Muñecas en extension dorsal (teclas mas altas que los codos)
//   - Piernas y caderas en angulo de 90° (sentado)
//
// Zonas de riesgo esperadas:
//   Cuello     → ROJO     (~65 pts): flexion 25° + tiempo prolongado
//   Hombros    → MODERADO (~35 pts): elevacion + tension sostenida
//   Lumbar     → MODERADO (~30 pts): torso encorvado + tiempo
//   Rodillas   → VERDE    (~ 5 pts): sentado neutro
// ==========================================================================
inline ScenarioData crearEscenarioTrabajoComputadora()
{
    ScenarioData s;
    s.nombre      = "Trabajo en computadora";
    s.descripcion = "Postura incorrecta frente a pantalla: cuello y torso encorvados, "
                    "hombros tensos. Riesgo critico en cuello.";

    // ---- Torso: ligeramente encorvado hacia la pantalla ----
    s.anguloTorso     = 20.0f;   // inclinado 20° hacia adelante

    // ---- Cuello: flexionado hacia adelante mirando pantalla baja ----
    s.anguloCuello    = 25.0f;   // "cuello de tortuga" — posicion clasica de oficina

    // ---- Hombros: levemente elevados (tension muscular) + brazos al frente para teclear ----
    s.anguloBrazoDer  = 10.0f;   // hombros subidos levemente por tension
    s.anguloBrazoIzq  = 10.0f;
    s.anguloBrazoDerX = -30.0f;  // brazos ligeramente hacia adelante (negativo = adelante)
    s.anguloBrazoIzqX = -30.0f;

    // ---- Codos: doblados para teclear (~60°) ----
    // Con la nueva convención: anguloCodoDer = flexión eje X (positivo = doblar adelante)
    s.anguloCodoDer   = 60.0f;   // dobla el codo hacia adelante
    s.anguloCodoIzq   = 60.0f;

    // ---- Muñecas: leve extension dorsal al teclear ----
    s.anguloManoDerX  = 15.0f;
    s.anguloManoIzqX  = 15.0f;

    // ---- Caderas: flexion 80° hacia adelante (posicion sentado) ----
    // POSITIVO = muslos hacia adelante en nuestro sistema de coordenadas
    s.anguloMusloDerX = 80.0f;   // muslo hacia adelante simulando cadera en 90°
    s.anguloMusloIzqX = 80.0f;

    // ---- Rodillas: dobladas ~80° (piernas hacia abajo desde rodilla) ----
    s.anguloRodillaDer = 70.0f;
    s.anguloRodillaIzq = 70.0f;

    // ---- Variables ergonómicas ----
    s.pesoCarga        = 0.5f;   // kg — peso de las manos sobre teclado
    s.distanciaCarga   = 0.40f;  // metros — distancia del teclado al cuerpo
    s.tiempoExposicion = 120.0f; // minutos — 2 horas continuas (tipico en oficina)

    return s;
}
