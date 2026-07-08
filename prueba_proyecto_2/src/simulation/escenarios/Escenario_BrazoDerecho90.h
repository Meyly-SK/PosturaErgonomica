#pragma once

#include "../Types.h"

// ==========================================================================
// Escenario_BrazoDerecho90.h
// Brazo derecho levantado a 90 grados (posicion horizontal).
//
// Uso ergonomico: simula alcance alto con un brazo.
// Riesgo principal: hombro derecho (ZonaRiesgo::Hombros).
//
// EJEMPLO DE EVIDENCIA — primer escenario de postura implementado.
// Este archivo demuestra como agregar un nuevo escenario:
//   1. Crear este archivo en src/simulation/escenarios/
//   2. Incluirlo en ScenarioManager.cpp
//   3. Llamar crearEscenarioBrazoDerecho90() dentro de inicializarEscenariosDefault()
// ==========================================================================
inline ScenarioData crearEscenarioBrazoDerecho90()
{
    ScenarioData s;
    s.nombre           = "Brazo derecho levantado 90°";
    s.descripcion      = "Brazo derecho en posicion horizontal (90 grados). "
                         "Simula alcance a un estante alto o panel de control. "
                         "Riesgo elevado en hombro derecho con exposicion prolongada.";

    s.anguloBrazoDer   = 90.0f;   // 90 grados = brazo horizontal
    s.tiempoExposicion = 5.0f;    // minutos de exposicion tipica
    s.pesoCarga        = 0.5f;    // herramienta ligera en la mano

    return s;
}
