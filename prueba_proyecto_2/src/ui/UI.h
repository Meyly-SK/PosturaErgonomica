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
// ==========================================================================
class UI
{
public:
    // Inicializar ImGui (llamar una vez después de crear ventana/contexto OpenGL)
    static void inicializar(void* ventanaGLFW);

    // Liberar recursos de ImGui (llamar al cerrar)
    static void liberar();

    // Iniciar nuevo frame de ImGui (llamar al inicio de cada frame, antes de dibujar)
    static void nuevaFrame();

    // Dibujar el panel principal con información del escenario y riesgo.
    // Llamar después de nuevaFrame() y antes de renderizar().
    static void dibujarPanel(const ScenarioManager& escenarios,
                             const RiskData& riesgo);

    // Enviar comandos de renderizado de ImGui a OpenGL (llamar al final de renderizar())
    static void renderizar();
};
