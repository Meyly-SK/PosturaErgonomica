#pragma once

#include <vector>

#include "Types.h"

// ==========================================================================
// ScenarioManager — gestiona la lista de escenarios/posturas predefinidos
//
// Responsabilidad: almacenar ScenarioData y navegar entre ellos.
// NO dibuja, NO calcula riesgo, NO lee input.
//
// Para agregar un escenario nuevo:
//   Ver instrucciones en src/simulation/escenarios/README_ESCENARIOS.md
// ==========================================================================
class ScenarioManager
{
public:
    // Carga todos los escenarios predefinidos (incluye los de la carpeta escenarios/).
    void inicializarEscenariosDefault();

    // Navegar entre escenarios (circulares)
    void siguiente();
    void anterior();
    void irA(int indice);   // saltar directamente a un escenario por índice

    // Obtener el escenario activo
    const ScenarioData& getActual() const;

    // Información para UI
    int getIndiceActual()     const;
    int getTotalEscenarios()  const;
    const std::string& getNombreEscenario(int indice) const;  // nombre sin cambiar estado

private:
    std::vector<ScenarioData> mEscenarios;
    int mIndiceActual = 0;
};
