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

    // ---- Tobillo / Pie ----
    // Eje Y: rotación axial del pie (dedo apunta a izq/der = pronación/supinación)
    // Eje X: dorsiflexión (punta arriba +) / flexión plantar (punta abajo -)
    // Uso: tobilloDer.Y=30 → pie derecho girado 30° (mirando al frente cuando pierna abierta)
    float anguloPieDerY       = 0.0f;   // rotación axial pie derecho
    float anguloPieIzqY       = 0.0f;   // rotación axial pie izquierdo
    float anguloPieDerX       = 0.0f;   // dorsiflexión pie derecho
    float anguloPieIzqX       = 0.0f;   // dorsiflexión pie izquierdo

    // ---- Muñeca / Mano ----
    // Eje Y: pronación/supinación (palma arriba/abajo)
    // Eje X: flexión de muñeca (hacia adelante/atrás)
    // Eje Z: desviación cubital/radial (lateral)
    float anguloManoDerY      = 0.0f;   // pronación/supinación mano derecha
    float anguloManoIzqY      = 0.0f;
    float anguloManoDerX      = 0.0f;   // flexión de muñeca derecha
    float anguloManoIzqX      = 0.0f;
    float anguloManoDerZ      = 0.0f;   // desviación lateral mano derecha
    float anguloManoIzqZ      = 0.0f;

    // ---- Torso lateral (eje Z) ----
    float anguloTorsoLateral  = 0.0f;   // inclinación lateral del torso (+= hacia derecha)

    // ---- Variables ergonómicas ----
    float pesoCarga           = 0.0f;   // kg
    float distanciaCarga      = 0.0f;   // metros desde el cuerpo
    float tiempoExposicion    = 0.0f;   // minutos
};

// --------------------------------------------------------------------------
// interpolar — interpolación lineal de todos los campos float de ScenarioData
//
// Uso en App::actualizar(dt):
//   float t = glm::smoothstep(0.0f, 1.0f, progreso);
//   ScenarioData actual = interpolar(origen, destino, t);
//   cuerpo.setScenario(actual);
//
// glm::smoothstep produce una curva S (empieza lento, acelera, termina lento).
// --------------------------------------------------------------------------
#include <glm/glm.hpp>
inline ScenarioData interpolar(const ScenarioData& a, const ScenarioData& b, float t)
{
    ScenarioData r;
    // Metadatos: usar los del destino
    r.nombre      = b.nombre;
    r.descripcion = b.descripcion;

    // Todos los campos float se interpolan linealmente con el factor t
    #define LERP(campo) r.campo = glm::mix(a.campo, b.campo, t)
    LERP(anguloCuello);        LERP(anguloLateralCuello);
    LERP(anguloTorso);         LERP(anguloTorsoLateral);
    LERP(anguloBrazoIzq);      LERP(anguloBrazoDer);
    LERP(anguloBrazoIzqX);     LERP(anguloBrazoDerX);
    LERP(anguloCodoIzq);       LERP(anguloCodoDer);
    LERP(anguloCodoIzqX);      LERP(anguloCodoDerX);
    LERP(anguloMusloIzq);      LERP(anguloMusloDer);
    LERP(anguloMusloIzqX);     LERP(anguloMusloDerX);
    LERP(anguloRodilla);       LERP(anguloRodillaX);
    LERP(anguloPieDerY);       LERP(anguloPieIzqY);
    LERP(anguloPieDerX);       LERP(anguloPieIzqX);
    LERP(anguloManoDerY);      LERP(anguloManoIzqY);
    LERP(anguloManoDerX);      LERP(anguloManoIzqX);
    LERP(anguloManoDerZ);      LERP(anguloManoIzqZ);
    LERP(pesoCarga);           LERP(distanciaCarga);
    LERP(tiempoExposicion);
    #undef LERP
    return r;
}

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
