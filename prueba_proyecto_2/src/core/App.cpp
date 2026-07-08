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

#include "../body/HumanBody.h"
#include "../simulation/ScenarioManager.h"

// Ventana global al módulo (simple por ahora)
static GLFWwindow* gVentana = nullptr;

static bool gDemoInicializada = false;
static Renderer gRenderer;
static Camera gCamara;
static Mesh gCubo;
static Mesh gCilindro;
static Mesh gEsfera;
static HumanBody gCuerpo;
static ScenarioManager gEscenarios;

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

        gCubo     = PrimitiveFactory::crearCubo();
        gCilindro = PrimitiveFactory::crearCilindro(0.5f, 1.0f, 20);
        gEsfera   = PrimitiveFactory::crearEsfera(0.5f, 20, 12);

        // Inicializar cuerpo + escenarios
        gCuerpo.inicializar();
        gEscenarios.inicializarEscenariosDefault();
        // Aplicar postura inicial (postura neutra)
        gCuerpo.setScenario(gEscenarios.getActual());

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

    // ------------------------------------------------------------------
    // Debug: teclas 1-7 para ver partes por separado
    // ------------------------------------------------------------------
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

    // ------------------------------------------------------------------
    // Cambio de escenario: N = siguiente, P = anterior
    // ------------------------------------------------------------------
    static bool tNAntes = false;
    static bool tPAntes = false;
    const bool tN = glfwGetKey(gVentana, GLFW_KEY_N) == GLFW_PRESS;
    const bool tP = glfwGetKey(gVentana, GLFW_KEY_P) == GLFW_PRESS;

    if (tN && !tNAntes)
    {
        gEscenarios.siguiente();
        gCuerpo.setScenario(gEscenarios.getActual());
        std::cout << "Escenario ["
                  << (gEscenarios.getIndiceActual() + 1) << "/"
                  << gEscenarios.getTotalEscenarios() << "]: "
                  << gEscenarios.getActual().nombre << "\n";
    }
    if (tP && !tPAntes)
    {
        gEscenarios.anterior();
        gCuerpo.setScenario(gEscenarios.getActual());
        std::cout << "Escenario ["
                  << (gEscenarios.getIndiceActual() + 1) << "/"
                  << gEscenarios.getTotalEscenarios() << "]: "
                  << gEscenarios.getActual().nombre << "\n";
    }
    tNAntes = tN;
    tPAntes = tP;
}

void App::actualizar(float /*deltaTiempo*/)
{
    // Aqui se integrara RiskAnalyzer en el Hito 4.
}

void App::renderizar()
{
    glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCuerpo.actualizarJerarquia();
    gCuerpo.dibujar(gRenderer, gCubo, gCilindro, gEsfera, gCamara);
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
    if (gVentana)
    {
        glfwDestroyWindow(gVentana);
        gVentana = nullptr;
    }
    glfwTerminate();
}
