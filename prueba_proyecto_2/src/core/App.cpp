#include "App.h"

#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../graphics/Camera.h"
#include "../graphics/PrimitiveFactory.h"
#include "../graphics/Renderer.h"
#include "../graphics/Textura.h"

#include "../body/HumanBody.h"
#include "../simulation/ScenarioManager.h"
#include "../simulation/RiskAnalyzer.h"
#include "../ui/UI.h"

// Ventana global al módulo
static GLFWwindow* gVentana = nullptr;

static bool gDemoInicializada = false;
static Renderer gRenderer;
static Camera gCamara;

// Meshes sin UV (color sólido, modo fallback)
static Mesh gCubo;
static Mesh gCilindro;
static Mesh gEsfera;

// Meshes con UV (para textura de madera)
static Mesh gCuboUV;
static Mesh gCilindroUV;
static Mesh gEsferaUV;

// Meshes con UV + Normales (para iluminación Phong)
static Mesh gCuboN;
static Mesh gCilindroN;
static Mesh gEsferaN;

// Posición de la luz (fija, espacio mundo)
static const glm::vec3 kPosLuz = {2.0f, 6.0f, 4.0f};

// Entorno visual: piso y pared trasera
static Mesh gPisoN;
static Mesh gParedN;

// Textura de madera
static Textura gTexturaMadera;

static HumanBody gCuerpo;
static ScenarioManager gEscenarios;
static RiskAnalyzer gAnalizador;

// ---------------------------------------------------------------------------
// Transición suave entre escenarios (H6-A)
// Al presionar N/P, los ángulos interpolan desde la postura actual a la nueva.
// ---------------------------------------------------------------------------
static ScenarioData gEscenarioOrigen;   // postura "desde donde venimos"
static ScenarioData gEscenarioDestino;  // postura "a donde vamos"
static float        gProgresoTransicion = 1.0f;  // 1.0 = completada (sin transición activa)
static const float  gDuracionTransicion = 0.85f; // segundos

// Modo editor de postura libre
static bool         gModoLibre    = false;
static ScenarioData gEscenarioLibre;   // postura editable en tiempo real

int App::ejecutar()
{
    if (!inicializar())
    {
        cerrar();
        return -1;
    }

    float ultimoTiempo = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(gVentana))
    {
        const float tiempoActual = static_cast<float>(glfwGetTime());
        const float deltaTiempo  = tiempoActual - ultimoTiempo;
        ultimoTiempo = tiempoActual;

        glfwPollEvents();
        procesarEntrada(deltaTiempo);
        actualizar(deltaTiempo);
        renderizar();
        glfwSwapBuffers(gVentana);
    }

    cerrar();
    return 0;
}

bool App::inicializar()
{
    if (!glfwInit())
    {
        std::cout << "Error: no se pudo inicializar GLFW.\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gVentana = glfwCreateWindow(800, 600, "ErgoSim 3D", nullptr, nullptr);
    if (!gVentana)
    {
        std::cout << "Error: no se pudo crear la ventana.\n";
        return false;
    }

    glfwMakeContextCurrent(gVentana);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: no se pudo inicializar GLEW.\n";
        return false;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    if (!gDemoInicializada)
    {
        gCamara.setAspecto(800.0f, 600.0f);
        Input::setCamera(&gCamara);
        Input::setCapturaMouse(true);
        glfwSetInputMode(gVentana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gRenderer.inicializar())
        {
            std::cout << "Error: no se pudo inicializar Renderer/Shader.\n";
            return false;
        }

        // Meshes sin UV (fallback)
        gCubo     = PrimitiveFactory::crearCubo();
        gCilindro = PrimitiveFactory::crearCilindro(0.5f, 1.0f, 20);
        gEsfera   = PrimitiveFactory::crearEsfera(0.5f, 20, 12);

        // Meshes con UV (textura de madera)
        gCuboUV     = PrimitiveFactory::crearCuboUV();
        gCilindroUV = PrimitiveFactory::crearCilindroUV(0.5f, 1.0f, 20);
        gEsferaUV   = PrimitiveFactory::crearEsferaUV(0.5f, 20, 12);

        // Meshes con UV + Normales (iluminación Phong)
        gCuboN     = PrimitiveFactory::crearCuboUVNormal();
        gCilindroN = PrimitiveFactory::crearCilindroUVNormal(0.5f, 1.0f, 20);
        gEsferaN   = PrimitiveFactory::crearEsferaUVNormal(0.5f, 20, 12);

        // Entorno: piso (cubo muy aplanado) y pared trasera
        gPisoN  = PrimitiveFactory::crearCuboUVNormal();
        gParedN = PrimitiveFactory::crearCuboUVNormal();

        // Cargar textura de madera (ruta relativa al directorio de trabajo del exe)
        // El exe se genera en x64/Debug/, así que la ruta es relativa a ahí.
        // También intentamos desde el directorio del proyecto por si se ejecuta desde VS.
        if (!gTexturaMadera.cargarDesdeArchivo("assets/textures/textura_madera.jpg"))
        {
            // Intentar ruta alternativa (desde directorio de la solución)
            gTexturaMadera.cargarDesdeArchivo("prueba_proyecto_2/assets/textures/textura_madera.jpg");
        }

        // Inicializar cuerpo + escenarios
        gCuerpo.inicializar();
        gEscenarios.inicializarEscenariosDefault();
        gCuerpo.setScenario(gEscenarios.getActual());

        // Inicializar el sistema de transición con el escenario inicial
        // para evitar que la primera transición arranque desde postura vacía (ceros)
        gEscenarioOrigen  = gEscenarios.getActual();
        gEscenarioDestino = gEscenarios.getActual();

        // Inicializar panel UI (ImGui)
        UI::inicializar(gVentana);

        gDemoInicializada = true;
    }

    std::cout << "ErgoSim 3D iniciado.\n";
    std::cout << "  N/P     = siguiente/anterior escenario\n";
    std::cout << "  1-7     = filtros debug de partes\n";
    std::cout << "  WASD    = mover camara  |  Mouse = girar\n";
    std::cout << "  M       = toggle mouse  |  ESC = salir\n";
    std::cout << "Escenario actual: " << gEscenarios.getActual().nombre << "\n";
    return true;
}

void App::procesarEntrada(float deltaTiempo)
{
    Input::procesar(gVentana, deltaTiempo);

    // Debug: teclas 1-7 para ver partes por separado
    static bool tecla1Antes = false, tecla2Antes = false, tecla3Antes = false;
    static bool tecla4Antes = false, tecla5Antes = false;
    static bool tecla6Antes = false, tecla7Antes = false;

    const bool t1 = glfwGetKey(gVentana, GLFW_KEY_1) == GLFW_PRESS;
    const bool t2 = glfwGetKey(gVentana, GLFW_KEY_2) == GLFW_PRESS;
    const bool t3 = glfwGetKey(gVentana, GLFW_KEY_3) == GLFW_PRESS;
    const bool t4 = glfwGetKey(gVentana, GLFW_KEY_4) == GLFW_PRESS;
    const bool t5 = glfwGetKey(gVentana, GLFW_KEY_5) == GLFW_PRESS;
    const bool t6 = glfwGetKey(gVentana, GLFW_KEY_6) == GLFW_PRESS;
    const bool t7 = glfwGetKey(gVentana, GLFW_KEY_7) == GLFW_PRESS;

    if (t1 && !tecla1Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::Completo);
    if (t2 && !tecla2Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloCabezaCuello);
    if (t3 && !tecla3Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloHombros);
    if (t4 && !tecla4Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloBrazoIzq);
    if (t5 && !tecla5Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloBrazoDer);
    if (t6 && !tecla6Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloPiernaIzq);
    if (t7 && !tecla7Antes) gCuerpo.setModoDebug(HumanBody::ModoDebug::SoloPiernaDer);

    tecla1Antes = t1; tecla2Antes = t2; tecla3Antes = t3;
    tecla4Antes = t4; tecla5Antes = t5;
    tecla6Antes = t6; tecla7Antes = t7;

    // Cambio de escenario: N = siguiente, P = anterior
    static bool tNAntes = false;
    static bool tPAntes = false;
    const bool tN = glfwGetKey(gVentana, GLFW_KEY_N) == GLFW_PRESS;
    const bool tP = glfwGetKey(gVentana, GLFW_KEY_P) == GLFW_PRESS;

    // Helper: inicia la transición hacia el escenario actual del manager
    auto iniciarTransicion = [&]()
    {
        // Capturar postura actual interpolada (para que la transición arranque
        // desde donde el cuerpo está visualmente, no desde donde estaba antes)
        const float t = glm::smoothstep(0.0f, 1.0f, gProgresoTransicion);
        gEscenarioOrigen  = interpolar(gEscenarioOrigen, gEscenarioDestino, t);
        gEscenarioDestino = gEscenarios.getActual();
        gProgresoTransicion = 0.0f;   // reiniciar progreso
        std::cout << "Escenario ["
                  << (gEscenarios.getIndiceActual() + 1) << "/"
                  << gEscenarios.getTotalEscenarios() << "]: "
                  << gEscenarios.getActual().nombre << "\n";
    };

    if (tN && !tNAntes) { gEscenarios.siguiente(); iniciarTransicion(); }
    if (tP && !tPAntes) { gEscenarios.anterior();  iniciarTransicion(); }
    tNAntes = tN;
    tPAntes = tP;
}

void App::actualizar(float deltaTiempo)
{
    // H6-A: avanzar la transición suave entre escenarios
    ScenarioData escenarioVisible;
    if (gProgresoTransicion < 1.0f)
    {
        gProgresoTransicion += deltaTiempo / gDuracionTransicion;
        if (gProgresoTransicion > 1.0f) gProgresoTransicion = 1.0f;

        // glm::smoothstep produce curva S: empieza lento, acelera, termina lento
        const float t = glm::smoothstep(0.0f, 1.0f, gProgresoTransicion);
        escenarioVisible = interpolar(gEscenarioOrigen, gEscenarioDestino, t);
    }
    else
    {
        escenarioVisible = gEscenarios.getActual();
    }

    gCuerpo.setScenario(escenarioVisible);

    // H4: calcular riesgo y colorear el cuerpo
    const RiskData riesgo = gAnalizador.analizar(escenarioVisible);
    gCuerpo.applyRisk(riesgo);

    // Modo editor libre: usar postura editable en vez del escenario base
    if (gModoLibre)
    {
        gCuerpo.setScenario(gEscenarioLibre);
        const RiskData riesgoLibre = gAnalizador.analizar(gEscenarioLibre);
        gCuerpo.applyRisk(riesgoLibre);
    }

    // H5 + editor libre: preparar frame de UI y procesar botones
    UI::nuevaFrame();
    const ResultadoUI accionUI = UI::dibujarPanel(
        gEscenarios, riesgo, gModoLibre, gEscenarioLibre);

    // Toggle del modo editor libre
    if (accionUI.toggleModoLibre)
    {
        gModoLibre = !gModoLibre;
        if (gModoLibre)
        {
            // Al activar: copiar el escenario base actual como punto de partida
            gEscenarioLibre = gEscenarios.getActual();
        }
    }

    // Si el editor libre modificó la postura, actualizar gEscenarioLibre
    if (gModoLibre && accionUI.posturaModificada)
    {
        gEscenarioLibre = accionUI.posturaLibre;
        gCuerpo.setScenario(gEscenarioLibre);
        const RiskData riesgoLibre = gAnalizador.analizar(gEscenarioLibre);
        gCuerpo.applyRisk(riesgoLibre);
    }

    // Procesar la acción solicitada por los botones de la UI
    if (accionUI.accion == AccionUI::Anterior)
    {
        gEscenarios.anterior();
        const float t = glm::smoothstep(0.0f, 1.0f, gProgresoTransicion);
        gEscenarioOrigen  = interpolar(gEscenarioOrigen, gEscenarioDestino, t);
        gEscenarioDestino = gEscenarios.getActual();
        gProgresoTransicion = 0.0f;
        // Si el editor libre está activo, resetear al nuevo escenario base
        if (gModoLibre) gEscenarioLibre = gEscenarios.getActual();
    }
    else if (accionUI.accion == AccionUI::Siguiente)
    {
        gEscenarios.siguiente();
        const float t = glm::smoothstep(0.0f, 1.0f, gProgresoTransicion);
        gEscenarioOrigen  = interpolar(gEscenarioOrigen, gEscenarioDestino, t);
        gEscenarioDestino = gEscenarios.getActual();
        gProgresoTransicion = 0.0f;
        if (gModoLibre) gEscenarioLibre = gEscenarios.getActual();
    }
    else if (accionUI.accion == AccionUI::IrA)
    {
        gEscenarios.irA(accionUI.indiceDestino);
        const float t = glm::smoothstep(0.0f, 1.0f, gProgresoTransicion);
        gEscenarioOrigen  = interpolar(gEscenarioOrigen, gEscenarioDestino, t);
        gEscenarioDestino = gEscenarios.getActual();
        gProgresoTransicion = 0.0f;
        if (gModoLibre) gEscenarioLibre = gEscenarios.getActual();
    }
}

void App::renderizar()
{
    glClearColor(0.12f, 0.15f, 0.22f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCuerpo.actualizarJerarquia();

    // ---- Dibujar entorno (piso y pared trasera) ----
    if (gTexturaMadera.estaLista())
    {
        // Piso: cubo muy aplanado, centrado en Y=0, tamaño 6x0.1x6
        glm::mat4 modeloPiso = glm::mat4(1.0f);
        modeloPiso = glm::translate(modeloPiso, {0.0f, -0.05f, 0.5f});
        modeloPiso = glm::scale(modeloPiso, {6.0f, 0.10f, 6.0f});
        gRenderer.dibujarConPhong(gPisoN, modeloPiso, {0.55f, 0.50f, 0.45f},
                                  gCamara, gTexturaMadera, 0.05f, kPosLuz);

        // Pared trasera: cubo delgado detrás del muñeco
        glm::mat4 modeloPared = glm::mat4(1.0f);
        modeloPared = glm::translate(modeloPared, {0.0f, 3.5f, -3.5f});
        modeloPared = glm::scale(modeloPared, {8.0f, 8.0f, 0.15f});
        gRenderer.dibujarConPhong(gParedN, modeloPared, {0.20f, 0.22f, 0.28f},
                                  gCamara, gTexturaMadera, 0.02f, kPosLuz);
    }

    // Usar Phong si textura y normales están listas, de lo contrario fallback
    if (gTexturaMadera.estaLista())
    {
        gCuerpo.dibujarConPhong(gRenderer, gCuboN, gCilindroN, gEsferaN,
                                gCamara, gTexturaMadera, kPosLuz);
    }
    else
    {
        gCuerpo.dibujar(gRenderer, gCubo, gCilindro, gEsfera, gCamara);
    }

    // H5: renderizar panel UI encima de la escena 3D
    UI::renderizar();
}

void App::dibujarDemoCubo()
{
    const float t = static_cast<float>(glfwGetTime());
    glm::mat4 modelo(1.0f);
    modelo = glm::rotate(modelo, t * 0.6f, glm::vec3(0.3f, 1.0f, 0.0f));
    gRenderer.dibujar(gCubo, modelo, {0.2f, 0.8f, 0.9f}, gCamara);
}

void App::cerrar()
{
    UI::liberar();
    gTexturaMadera.liberar();

    if (gVentana)
    {
        glfwDestroyWindow(gVentana);
        gVentana = nullptr;
    }
    glfwTerminate();
}
