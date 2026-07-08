#include "ScenarioManager.h"

// Incluir un archivo por escenario.
// Cada include es independiente: cada integrante solo toca su propio archivo.
#include "escenarios/Escenario_Neutro.h"
#include "escenarios/Escenario_BrazoDerecho90.h"
#include "escenarios/Escenario_LevantamientoIncorrecto.h"
// PUNTO DE EXTENSIÓN: agrega aquí el include de tu escenario nuevo:
// #include "escenarios/Escenario_MiPostura.h"

void ScenarioManager::inicializarEscenariosDefault()
{
    mEscenarios.clear();

    // Cada push_back registra un escenario en la lista de navegación.
    // El orden aquí define el orden al presionar N/P.
    mEscenarios.push_back(crearEscenarioNeutro());
    mEscenarios.push_back(crearEscenarioBrazoDerecho90());
    mEscenarios.push_back(crearEscenarioLevantamientoIncorrecto());
    // PUNTO DE EXTENSIÓN: agrega aquí tu escenario:
    // mEscenarios.push_back(crearEscenarioMiPostura());

    mIndiceActual = 0;
}

void ScenarioManager::siguiente()
{
    if (mEscenarios.empty()) return;
    mIndiceActual = (mIndiceActual + 1) % static_cast<int>(mEscenarios.size());
}

void ScenarioManager::anterior()
{
    if (mEscenarios.empty()) return;
    mIndiceActual = (mIndiceActual - 1 + static_cast<int>(mEscenarios.size()))
                    % static_cast<int>(mEscenarios.size());
}

const ScenarioData& ScenarioManager::getActual() const
{
    return mEscenarios[mIndiceActual];
}

int ScenarioManager::getIndiceActual() const
{
    return mIndiceActual;
}

int ScenarioManager::getTotalEscenarios() const
{
    return static_cast<int>(mEscenarios.size());
}
