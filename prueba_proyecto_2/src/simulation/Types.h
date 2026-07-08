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
    float anguloBrazoIzq      = 0.0f;   // elevación brazo izq (0=colgando, +90=horizontal)
    float anguloBrazoDer      = 0.0f;   // elevación brazo der

    // ---- Codos (flexión del antebrazo desde el codo) ----
    float anguloCodoIzq       = 0.0f;   // 0=extendido, +90=doblado 90°, +150=muy doblado
    float anguloCodoDer       = 0.0f;

    // ---- Piernas (desde cadera) ----
    float anguloMusloIzq      = 0.0f;   // 0=de pie, +90=sentado (muslo horizontal)
    float anguloMusloDer      = 0.0f;

    // ---- Rodillas (pantorrilla hacia atrás desde rodilla) ----
    float anguloRodilla       = 0.0f;   // 0=extendida, +90=pantorrilla horizontal hacia atrás

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
