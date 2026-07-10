#pragma once

#include "../simulation/Types.h"
#include "../simulation/ScenarioManager.h"
#include "../simulation/RiskAnalyzer.h"

// ==========================================================================
// UI — panel informativo del simulador (ImGui)
//
// Responsabilidad: mostrar en pantalla el estado del escenario activo y
//                  los niveles de riesgo por zona.
//
// Regla (README): la UI NO dibuja geometría 3D ni calcula riesgo.
//                 Solo recibe ScenarioData y RiskData y los presenta.
//
// Comunicación de vuelta a App:
//   dibujarPanel() devuelve ResultadoUI con la acción que el usuario pidió
//   (botón pulsado). App.cpp actúa en consecuencia.
// ==========================================================================

// Acción solicitada por el usuario a través de la UI
enum class AccionUI
{
    Ninguna,    // el usuario no pulsó ningún botón de escenario
    Anterior,   // botón "◀ Anterior"
    Siguiente,  // botón "Siguiente ▶"
    IrA         // botón de un escenario específico; ver indiceDestino
};

struct ResultadoUI
{
    AccionUI accion       = AccionUI::Ninguna;
    int      indiceDestino = -1;   // válido solo cuando accion == IrA
};

class UI
{
public:
    // Inicializar ImGui (llamar una vez después de crear ventana/contexto OpenGL)
    static void inicializar(void* ventanaGLFW);

    // Liberar recursos de ImGui (llamar al cerrar)
    static void liberar();

    // Iniciar nuevo frame de ImGui (llamar al inicio de cada frame, antes de dibujar)
    static void nuevaFrame();

    // Dibujar el panel principal.
    // Devuelve ResultadoUI indicando si el usuario pulsó algún botón de navegación.
    static ResultadoUI dibujarPanel(const ScenarioManager& escenarios,
                                    const RiskData& riesgo);

    // Enviar comandos de renderizado de ImGui a OpenGL (llamar al final de renderizar())
    static void renderizar();
};
