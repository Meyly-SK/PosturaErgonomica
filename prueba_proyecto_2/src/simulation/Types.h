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
// Convención de ángulos: GRADOS, positivo = flexión hacia adelante / sube.
// --------------------------------------------------------------------------
struct ScenarioData
{
    std::string nombre;
    std::string descripcion;

    // Ángulos de articulaciones (grados)
    float anguloCuello   = 0.0f;   // flexión del cuello
    float anguloTorso    = 0.0f;   // flexión del torso/lumbar
    float anguloBrazoIzq = 0.0f;   // elevación brazo izquierdo (0 = colgando)
    float anguloBrazoDer = 0.0f;   // elevación brazo derecho
    float anguloRodilla  = 0.0f;   // flexión de rodillas

    // Variables ergonómicas
    float pesoCarga        = 0.0f; // kg
    float distanciaCarga   = 0.0f; // metros
    float tiempoExposicion = 0.0f; // minutos
};

// --------------------------------------------------------------------------
// RiskData — resultado del análisis ergonómico por zona
// Escala: 0 (sin riesgo) a 100 (riesgo máximo)
// --------------------------------------------------------------------------
struct RiskData
{
    float riesgoCuello   = 0.0f;
    float riesgoLumbar   = 0.0f;
    float riesgoHombros  = 0.0f;
    float riesgoRodillas = 0.0f;
};
