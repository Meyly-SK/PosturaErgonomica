#include "UI.h"

// ImGui headers (backend OpenGL3 + GLFW)
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

// ---------------------------------------------------------------------------
// inicializar
// ---------------------------------------------------------------------------
void UI::inicializar(void* ventanaGLFW)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Tema oscuro (acorde con el fondo de la app)
    ImGui::StyleColorsDark();

    GLFWwindow* win = static_cast<GLFWwindow*>(ventanaGLFW);
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

// ---------------------------------------------------------------------------
// liberar
// ---------------------------------------------------------------------------
void UI::liberar()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// ---------------------------------------------------------------------------
// nuevaFrame
// ---------------------------------------------------------------------------
void UI::nuevaFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

// ---------------------------------------------------------------------------
// dibujarPanel
//
// Muestra un panel con:
//   - Nombre y descripción del escenario activo
//   - Índice / total de escenarios
//   - Barras de progreso con colores para cada zona de riesgo
//   - Controles disponibles
// ---------------------------------------------------------------------------
ResultadoUI UI::dibujarPanel(const ScenarioManager& escenarios, const RiskData& riesgo)
{
    ResultadoUI resultado;
    const ScenarioData& s = escenarios.getActual();

    // Posición y tamaño fijos (esquina superior izquierda)
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280.0f, 0.0f), ImGuiCond_Always); // ancho fijo, alto automático
    ImGui::SetNextWindowBgAlpha(0.80f);

    ImGui::Begin("ErgoSim 3D", nullptr,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove   |
                 ImGuiWindowFlags_NoCollapse);

    // ---- Escenario activo ----
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.2f, 1.0f), "Escenario:");
    ImGui::SameLine();
    ImGui::Text("[%d/%d]", escenarios.getIndiceActual() + 1, escenarios.getTotalEscenarios());

    ImGui::TextWrapped("%s", s.nombre.c_str());

    if (!s.descripcion.empty())
    {
        ImGui::Spacing();
        ImGui::TextDisabled("%s", s.descripcion.c_str());
    }

    ImGui::Separator();

    // ---- Niveles de riesgo por zona ----
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Riesgo ergonomico:");
    ImGui::Spacing();

    // Lambda local para dibujar una barra de riesgo con color
    auto barraRiesgo = [](const char* etiqueta, float valor)
    {
        // Color de la barra: verde / amarillo / rojo
        ImVec4 color;
        const char* nivel;
        if (valor < 30.0f)
        {
            color = ImVec4(0.15f, 0.80f, 0.15f, 1.0f);
            nivel = "BAJO";
        }
        else if (valor < 60.0f)
        {
            color = ImVec4(0.95f, 0.80f, 0.05f, 1.0f);
            nivel = "MODERADO";
        }
        else
        {
            color = ImVec4(0.90f, 0.15f, 0.15f, 1.0f);
            nivel = "CRITICO";
        }

        ImGui::TextUnformatted(etiqueta);
        ImGui::SameLine(90.0f);
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
        ImGui::ProgressBar(valor / 100.0f, ImVec2(-1.0f, 0.0f));
        ImGui::PopStyleColor();
        ImGui::SameLine();
        // Mostrar valor numérico + nivel
        ImGui::Text("%.0f  %s", valor, nivel);
    };

    barraRiesgo("Lumbar",   riesgo.riesgoLumbar);
    barraRiesgo("Hombros",  riesgo.riesgoHombros);
    barraRiesgo("Cuello",   riesgo.riesgoCuello);
    barraRiesgo("Rodillas", riesgo.riesgoRodillas);

    // ---- Variables del escenario (si hay carga) ----
    if (s.pesoCarga > 0.0f || s.tiempoExposicion > 0.0f)
    {
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Variables:");
        if (s.pesoCarga > 0.0f)
            ImGui::Text("  Peso carga:    %.1f kg", s.pesoCarga);
        if (s.distanciaCarga > 0.0f)
            ImGui::Text("  Distancia:     %.2f m", s.distanciaCarga);
        if (s.tiempoExposicion > 0.0f)
            ImGui::Text("  Exposicion:    %.0f min", s.tiempoExposicion);
    }

    ImGui::Separator();

    // ---- Botones de selección directa de escenario ----
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Escenarios:");
    ImGui::Spacing();

    const int total   = escenarios.getTotalEscenarios();
    const int actual  = escenarios.getIndiceActual();

    for (int i = 0; i < total; i++)
    {
        // Resaltar el botón del escenario activo con otro color
        const bool esActivo = (i == actual);
        if (esActivo)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.55f, 0.85f, 1.0f));

        // Etiqueta: "N. Nombre del escenario" — usa getNombreEscenario para todos
        char etiqueta[128];
        std::snprintf(etiqueta, sizeof(etiqueta), "%d. %s", i + 1,
                      escenarios.getNombreEscenario(i).c_str());

        if (ImGui::Button(etiqueta, ImVec2(-1.0f, 0.0f)))
        {
            if (!esActivo)
            {
                resultado.accion        = AccionUI::IrA;
                resultado.indiceDestino = i;
            }
        }

        if (esActivo)
            ImGui::PopStyleColor();
    }

    ImGui::Spacing();

    // ---- Botones de navegación secuencial ◀ / ▶ ----
    const float anchoBoton = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

    if (ImGui::Button("< Anterior", ImVec2(anchoBoton, 0.0f)))
        resultado.accion = AccionUI::Anterior;

    ImGui::SameLine();

    if (ImGui::Button("Siguiente >", ImVec2(anchoBoton, 0.0f)))
        resultado.accion = AccionUI::Siguiente;

    ImGui::Separator();

    // ---- Controles ----
    ImGui::TextDisabled("N/P = cambiar escenario");
    ImGui::TextDisabled("WASD + Mouse = camara");
    ImGui::TextDisabled("M = toggle mouse | ESC = salir");

    ImGui::End();
    return resultado;
}

// ---------------------------------------------------------------------------
// renderizar
// ---------------------------------------------------------------------------
void UI::renderizar()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
