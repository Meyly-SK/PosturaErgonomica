#include "RiskAnalyzer.h"

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

// ---------------------------------------------------------------------------
// Helpers internos
// ---------------------------------------------------------------------------

// Normaliza un valor: 0 cuando val=0, 1 cuando val=maximo.
// Sirve para convertir ángulos/kilos/distancias a un porcentaje de riesgo.
static float normalizar(float val, float maximo)
{
    if (maximo <= 0.0f) return 0.0f;
    return glm::clamp(std::abs(val) / maximo, 0.0f, 1.0f);
}

// ---------------------------------------------------------------------------
// RiskAnalyzer::analizar
// ---------------------------------------------------------------------------
RiskData RiskAnalyzer::analizar(const ScenarioData& escenario) const
{
    RiskData r;
    r.riesgoCuello   = calcularRiesgoCuello(escenario);
    r.riesgoLumbar   = calcularRiesgoLumbar(escenario);
    r.riesgoHombros  = calcularRiesgoHombros(escenario);
    r.riesgoRodillas = calcularRiesgoRodillas(escenario);
    return r;
}

// ---------------------------------------------------------------------------
// Zona: CUELLO
//
// Factores:
//   Ángulo cuello (principal) → hasta 60 pts  (máximo referencia: 45°)
//   Inclinación lateral       → hasta 20 pts  (máximo: 30°)
//   Tiempo de exposición      → hasta 20 pts  (máximo: 60 min)
// ---------------------------------------------------------------------------
float RiskAnalyzer::calcularRiesgoCuello(const ScenarioData& s) const
{
    float r = 0.0f;
    r += normalizar(s.anguloCuello,        45.0f) * 60.0f;
    r += normalizar(s.anguloLateralCuello, 30.0f) * 20.0f;
    r += normalizar(s.tiempoExposicion,    60.0f) * 20.0f;
    return glm::clamp(r, 0.0f, 100.0f);
}

// ---------------------------------------------------------------------------
// Zona: LUMBAR (torso / columna baja)
//
// Factores:
//   Ángulo torso     (principal) → hasta 40 pts  (máximo: 60°)
//   Peso de la carga             → hasta 35 pts  (máximo: 25 kg)
//   Distancia de la carga        → hasta 15 pts  (máximo: 0.8 m)
//   Tiempo de exposición         → hasta 10 pts  (máximo: 60 min)
// ---------------------------------------------------------------------------
float RiskAnalyzer::calcularRiesgoLumbar(const ScenarioData& s) const
{
    float r = 0.0f;
    r += normalizar(s.anguloTorso,      60.0f) * 40.0f;
    r += normalizar(s.pesoCarga,        25.0f) * 35.0f;
    r += normalizar(s.distanciaCarga,    0.8f) * 15.0f;
    r += normalizar(s.tiempoExposicion, 60.0f) * 10.0f;
    return glm::clamp(r, 0.0f, 100.0f);
}

// ---------------------------------------------------------------------------
// Zona: HOMBROS
//
// Se evalúa el brazo con mayor elevación (el peor caso).
// Factores:
//   Elevación del brazo (principal) → hasta 50 pts  (máximo: 90°)
//   Peso de la carga                → hasta 30 pts  (máximo: 10 kg)
//   Tiempo de exposición            → hasta 20 pts  (máximo: 30 min)
// ---------------------------------------------------------------------------
float RiskAnalyzer::calcularRiesgoHombros(const ScenarioData& s) const
{
    // Tomar el peor brazo (mayor elevación)
    const float maxBrazo = std::max(std::abs(s.anguloBrazoDer), std::abs(s.anguloBrazoIzq));

    float r = 0.0f;
    r += normalizar(maxBrazo,           90.0f) * 50.0f;
    r += normalizar(s.pesoCarga,        10.0f) * 30.0f;
    r += normalizar(s.tiempoExposicion, 30.0f) * 20.0f;
    return glm::clamp(r, 0.0f, 100.0f);
}

// ---------------------------------------------------------------------------
// Zona: RODILLAS / CADERAS
//
// Factores:
//   Ángulo muslo (cadera)     → hasta 40 pts  (máximo: 90°)
//   Flexión de rodilla        → hasta 30 pts  (máximo: 90°)
//   Peso de la carga          → hasta 20 pts  (máximo: 30 kg)
//   Tiempo de exposición      → hasta 10 pts  (máximo: 60 min)
// ---------------------------------------------------------------------------
float RiskAnalyzer::calcularRiesgoRodillas(const ScenarioData& s) const
{
    const float maxMuslo = std::max(std::abs(s.anguloMusloDer), std::abs(s.anguloMusloIzq));

    float r = 0.0f;
    r += normalizar(maxMuslo,           90.0f) * 40.0f;
    r += normalizar(s.anguloRodilla,    90.0f) * 30.0f;
    r += normalizar(s.pesoCarga,        30.0f) * 20.0f;
    r += normalizar(s.tiempoExposicion, 60.0f) * 10.0f;
    return glm::clamp(r, 0.0f, 100.0f);
}
