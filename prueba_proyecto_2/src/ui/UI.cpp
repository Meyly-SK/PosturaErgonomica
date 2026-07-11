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
ResultadoUI UI::dibujarPanel(const ScenarioManager& escenarios,
                              const RiskData& riesgo,
                              bool modoLibreActivo,
                              const ScenarioData& posturaModoLibre)
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
    barraRiesgo("Hombro D", riesgo.riesgoHombroDer);
    barraRiesgo("Hombro I", riesgo.riesgoHombroIzq);
    barraRiesgo("Cuello",   riesgo.riesgoCuello);
    barraRiesgo("Rodilla D",riesgo.riesgoRodillaDer);
    barraRiesgo("Rodilla I",riesgo.riesgoRodillaIzq);

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

    // ---- Toggle modo editor de postura libre ----
    ImGui::Separator();
    {
        bool activo = modoLibreActivo;
        if (ImGui::Checkbox(" Postura libre (editor)", &activo))
            resultado.toggleModoLibre = true;
    }

    // ---- Panel del editor (solo visible cuando modoLibreActivo) ----
    if (modoLibreActivo)
    {
        // Estado estático para los sliders: se inicializa desde posturaModoLibre
        // cuando el modo se activa (gestionado externamente vía posturaModoLibre).
        static ScenarioData sEditable;
        static bool sInicializado = false;

        // Reinicializar cuando:
        //   1. El modo se activa (false → true)
        //   2. El escenario activo cambió mientras el modo estaba activo
        static bool sModoAnterior   = false;
        static int  sIndiceAnterior = -1;
        const  int  indiceActual    = escenarios.getIndiceActual();

        const bool activandoModo      = (!sModoAnterior && modoLibreActivo);
        const bool cambioDeEscenario  = (modoLibreActivo && sIndiceAnterior != indiceActual);

        if (activandoModo || cambioDeEscenario)
        {
            sEditable       = posturaModoLibre;
            sInicializado   = true;
            sIndiceAnterior = indiceActual;
        }
        sModoAnterior = modoLibreActivo;

        if (!sInicializado) sEditable = posturaModoLibre;

        bool cambiado = false;

        // Helper macro para un slider de ángulo
        auto sliderAng = [&](const char* label, float& val, float vmin, float vmax) {
            if (ImGui::SliderFloat(label, &val, vmin, vmax, "%.0f deg"))
                cambiado = true;
        };
        auto sliderM = [&](const char* label, float& val, float vmin, float vmax, const char* fmt) {
            if (ImGui::SliderFloat(label, &val, vmin, vmax, fmt))
                cambiado = true;
        };

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Torso"))
        {
            sliderAng("Flex. adelante/atras", sEditable.anguloTorso, -90.0f, 90.0f);
        }

        if (ImGui::CollapsingHeader("Cuello"))
        {
            sliderAng("Flexion",   sEditable.anguloCuello,        -45.0f, 45.0f);
            sliderAng("Lateral",   sEditable.anguloLateralCuello, -30.0f, 30.0f);
        }

        if (ImGui::CollapsingHeader("Brazo derecho"))
        {
            ImGui::PushID("brazo_der");
            // Nota: +Z = brazo sube/va hacia afuera, -Z = baja/va hacia adentro
            sliderAng("Elev Z (afuera+)", sEditable.anguloBrazoDer,  -90.0f, 90.0f);
            sliderAng("Sagital X",        sEditable.anguloBrazoDerX, -90.0f, 90.0f);
            sliderAng("Codo Z",      sEditable.anguloCodoDer,    0.0f, 150.0f);
            sliderAng("Codo X",      sEditable.anguloCodoDerX, -90.0f, 90.0f);
            sliderAng("Muneca Y",    sEditable.anguloManoDerY,  -90.0f, 90.0f);
            sliderAng("Muneca X",    sEditable.anguloManoDerX,  -60.0f, 60.0f);
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Brazo izquierdo"))
        {
            ImGui::PushID("brazo_izq");
            sliderAng("Elev Z (afuera+)", sEditable.anguloBrazoIzq,  -90.0f, 90.0f);
            sliderAng("Sagital X",        sEditable.anguloBrazoIzqX, -90.0f, 90.0f);
            sliderAng("Codo Z",      sEditable.anguloCodoIzq,    0.0f, 150.0f);
            sliderAng("Codo X",      sEditable.anguloCodoIzqX, -90.0f, 90.0f);
            sliderAng("Muneca Y",    sEditable.anguloManoIzqY,  -90.0f, 90.0f);
            sliderAng("Muneca X",    sEditable.anguloManoIzqX,  -60.0f, 60.0f);
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Pierna derecha"))
        {
            ImGui::PushID("pierna_der");
            sliderAng("Lateral Z",      sEditable.anguloMusloDer,     -60.0f,  60.0f);
            sliderAng("Sagital X",      sEditable.anguloMusloDerX,    -90.0f,  90.0f);
            sliderAng("Rodilla Z",      sEditable.anguloRodillaDer,    0.0f,   90.0f);
            sliderAng("Rodilla X",      sEditable.anguloRodillaDerX,  -90.0f,  90.0f);
            sliderAng("Pie Y (axial)",  sEditable.anguloPieDerY,      -45.0f,  45.0f);
            sliderAng("Pie X (flex)",   sEditable.anguloPieDerX,      -30.0f,  30.0f);
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Pierna izquierda"))
        {
            ImGui::PushID("pierna_izq");
            sliderAng("Lateral Z",      sEditable.anguloMusloIzq,     -60.0f,  60.0f);
            sliderAng("Sagital X",      sEditable.anguloMusloIzqX,    -90.0f,  90.0f);
            sliderAng("Rodilla Z",      sEditable.anguloRodillaIzq,    0.0f,   90.0f);
            sliderAng("Rodilla X",      sEditable.anguloRodillaIzqX,  -90.0f,  90.0f);
            sliderAng("Pie Y (axial)",  sEditable.anguloPieIzqY,      -45.0f,  45.0f);
            sliderAng("Pie X (flex)",   sEditable.anguloPieIzqX,      -30.0f,  30.0f);
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Carga ergonomica"))
        {
            sliderM("Peso (kg)",    sEditable.pesoCarga,       0.0f, 50.0f, "%.1f kg");
            sliderM("Distancia (m)",sEditable.distanciaCarga,  0.0f,  1.0f, "%.2f m");
            sliderM("Tiempo (min)", sEditable.tiempoExposicion, 0.0f, 60.0f, "%.0f min");
        }

        if (cambiado)
        {
            resultado.posturaModificada = true;
            resultado.posturaLibre      = sEditable;
        }

        ImGui::Spacing();
        ImGui::Separator();
    }

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
