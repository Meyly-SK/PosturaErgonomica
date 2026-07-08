#pragma once

#include "Types.h"

// ==========================================================================
// RiskAnalyzer — calcula el nivel de riesgo ergonómico por zona
//
// Responsabilidad: recibir un ScenarioData y devolver un RiskData (0-100 por zona).
// NO dibuja, NO modifica el cuerpo, NO lee input.
//
// Escala de riesgo:
//   0  – 29  → Verde    (riesgo bajo / aceptable)
//   30 – 59  → Amarillo (riesgo moderado / atención)
//   60 – 100 → Rojo     (riesgo alto / crítico)
//
// Las fórmulas son heurísticas demostrativas, no clínicas.
// Cada zona tiene un presupuesto de 100 puntos distribuido entre sus factores:
//   - Ángulo de la articulación (factor principal)
//   - Peso de la carga
//   - Distancia de la carga al cuerpo
//   - Tiempo de exposición
// ==========================================================================
class RiskAnalyzer
{
public:
    // Calcula el riesgo de todas las zonas para un escenario dado.
    RiskData analizar(const ScenarioData& escenario) const;

private:
    // Funciones por zona (devuelven 0-100)
    float calcularRiesgoCuello(const ScenarioData& s)   const;
    float calcularRiesgoLumbar(const ScenarioData& s)   const;
    float calcularRiesgoHombros(const ScenarioData& s)  const;
    float calcularRiesgoRodillas(const ScenarioData& s) const;
};
