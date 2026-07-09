#pragma once

#include <string>

// ==========================================================================
// Types.h — Contratos compartidos entre módulos
// Estos structs/enums son el "lenguaje común" del proyecto.
// Ningún módulo debe depender del código interno de otro; solo de estos tipos.
// ==========================================================================

// --------------------------------------------------------------------------
// ZonaRiesgo — identifica qué zona anatómica es cada BodyPart
// --------------------------------------------------------------------------
enum class ZonaRiesgo
{
    Ninguna,
    Cuello,
    Lumbar,
    Hombros,
    Rodillas
};

// --------------------------------------------------------------------------
// ScenarioData — descripción completa de una postura/escenario
//
// Convención de ángulos: GRADOS
//   Positivo = flexión hacia adelante / elevación / inclinación hacia derecha
//   Negativo = extensión hacia atrás / descenso / inclinación hacia izquierda
//
// ARTICULACIONES DISPONIBLES:
// ┌─────────────────────────────────────────────────────────────────────────┐
// │  anguloCuello          Flexión del cuello (adelante/atrás)             │
// │  anguloLateralCuello   Inclinación lateral de la cabeza (der/izq)      │
// │  anguloTorso           Flexión del torso (adelante/atrás)              │
// │  anguloBrazoDer/Izq    Elevación del brazo desde el hombro             │
// │  anguloCodoDer/Izq     Flexión del antebrazo desde el codo             │
// │  anguloMusloDer/Izq    Flexión del muslo desde la cadera               │
// │  anguloRodilla         Flexión de las pantorrillas (ambas rodillas)    │
// └─────────────────────────────────────────────────────────────────────────┘
// --------------------------------------------------------------------------
struct ScenarioData
{
    std::string nombre;
    std::string descripcion;

    // ---- Cabeza / Cuello ----
    float anguloCuello        = 0.0f;   // flexión cuello (0=recto, +30=mirar abajo)
    float anguloLateralCuello = 0.0f;   // inclinación lateral (+= hacia hombro derecho)

    // ---- Torso ----
    float anguloTorso         = 0.0f;   // flexión torso (0=recto, +45=inclinado adelante)

    // ---- Brazos (desde hombro) ----
    // Eje Z: elevación lateral (arriba/abajo)  Eje X: adelante/atrás (sagital)
    float anguloBrazoIzq      = 0.0f;   // elevación brazo izq (eje Z: 0=colgando, +90=arriba)
    float anguloBrazoDer      = 0.0f;   // elevación brazo der
    float anguloBrazoIzqX     = 0.0f;   // brazo izq adelante(+) / atrás(-) (eje X sagital)
    float anguloBrazoDerX     = 0.0f;   // brazo der adelante(+) / atrás(-)

    // ---- Codos (flexión del antebrazo desde el codo) ----
    // Eje Z: dobla lateral  Eje X: dobla sagital (hacia adelante)
    float anguloCodoIzq       = 0.0f;   // 0=extendido, +90=doblado
    float anguloCodoDer       = 0.0f;
    float anguloCodoIzqX      = 0.0f;   // codo izq hacia adelante/atrás
    float anguloCodoDerX      = 0.0f;   // codo der hacia adelante/atrás

    // ---- Piernas (desde cadera) ----
    // Eje Z: movimiento lateral  Eje X: adelante/atrás (sagital — levantar pierna al frente)
    float anguloMusloIzq      = 0.0f;   // muslo izq lateral (eje Z)
    float anguloMusloDer      = 0.0f;   // muslo der lateral
    float anguloMusloIzqX     = 0.0f;   // muslo izq adelante(+) / atrás(-) (eje X sagital)
    float anguloMusloDerX     = 0.0f;   // muslo der adelante(+) / atrás(-) — clave para "pierna levantada"

    // ---- Rodillas (pantorrilla hacia atrás desde rodilla) ----
    // Eje Z: dobla lateral  Eje X: dobla sagital
    float anguloRodilla       = 0.0f;   // ambas rodillas eje Z (0=extendida, +90=doblada)
    float anguloRodillaX      = 0.0f;   // ambas rodillas eje X (sagital)

    // ---- Torso lateral (eje Z) ----
    float anguloTorsoLateral  = 0.0f;   // inclinación lateral del torso (+= hacia derecha)

    // ---- Variables ergonómicas ----
    float pesoCarga           = 0.0f;   // kg
    float distanciaCarga      = 0.0f;   // metros desde el cuerpo
    float tiempoExposicion    = 0.0f;   // minutos
};

// --------------------------------------------------------------------------
// RiskData — resultado del análisis ergonómico por zona
// Escala: 0 (sin riesgo) a 100 (riesgo máximo)
// Verde: <30 | Amarillo: 30-60 | Rojo: >60
// --------------------------------------------------------------------------
struct RiskData
{
    float riesgoCuello   = 0.0f;
    float riesgoLumbar   = 0.0f;
    float riesgoHombros  = 0.0f;
    float riesgoRodillas = 0.0f;
};
