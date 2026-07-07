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

// Ventana global al módulo (simple por ahora)
// Más adelante se puede encapsular mejor.
static GLFWwindow* gVentana = nullptr;

// Recursos de demo (Hito 1)
static bool gDemoInicializada = false;
static Renderer gRenderer;
static Camera gCamara;
static Mesh gCubo;
static Mesh gCilindro;
static Mesh gEsfera;
static HumanBody gCuerpo;

int App::ejecutar()
{
    if (!inicializar())
    {
        cerrar();
        return -1;
    }

    // Tiempo para deltaTime
    float ultimoTiempo = static_cast<float>(glfwGetTime());

    // Loop principal
    while (!glfwWindowShouldClose(gVentana))
    {
        const float tiempoActual = static_cast<float>(glfwGetTime());
        const float deltaTiempo = tiempoActual - ultimoTiempo;
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

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gVentana = glfwCreateWindow(800, 600, "ErgoSim 3D (base)", nullptr, nullptr);
    if (!gVentana)
    {
        std::cout << "Error: no se pudo crear la ventana.\n";
        return false;
    }

    glfwMakeContextCurrent(gVentana);

    // Inicializar GLEW (requiere contexto activo)
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: no se pudo inicializar GLEW.\n";
        return false;
    }

    // Estado inicial
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Inicializar demo del cubo (graphics)
    if (!gDemoInicializada)
    {
        gCamara.setAspecto(800.0f, 600.0f);

        // Conectar input a cámara (modo FPS)
        Input::setCamera(&gCamara);
        // Opcional: capturar mouse al inicio (se puede alternar con tecla M)
        Input::setCapturaMouse(true);
        glfwSetInputMode(gVentana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gRenderer.inicializar())
        {
            std::cout << "Error: no se pudo inicializar Renderer/Shader.\n";
            return false;
        }

        gCubo = PrimitiveFactory::crearCubo();
        gCilindro = PrimitiveFactory::crearCilindro(0.5f, 1.0f, 20);
        gEsfera = PrimitiveFactory::crearEsfera(0.5f, 20, 12);

        // Inicializar cuerpo modular
        gCuerpo.inicializar();

        gDemoInicializada = true;
    }

    std::cout << "App inicializada correctamente.\n";
    return true;
}

void App::procesarEntrada(float deltaTiempo)
{
    Input::procesar(gVentana, deltaTiempo);

    // -------------------------------------------------------------
    // Debug del cuerpo: teclas 1..6 para ver partes por separado.
    // Esto ayuda a ajustar proporciones "parte por parte".
    // 1: Completo
    // 2: Cabeza + Cuello
    // 3: Hombros
    // 4: Brazo izquierdo
    // 5: Brazo derecho
    // 6: Piernas (izq/der alternando con 6 y 7)
    // -------------------------------------------------------------
    static bool tecla1Antes = false;
    static bool tecla2Antes = false;
    static bool tecla3Antes = false;
    static bool tecla4Antes = false;
    static bool tecla5Antes = false;
    static bool tecla6Antes = false;
    static bool tecla7Antes = false;

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

    tecla1Antes = t1;
    tecla2Antes = t2;
    tecla3Antes = t3;
    tecla4Antes = t4;
    tecla5Antes = t5;
    tecla6Antes = t6;
    tecla7Antes = t7;
}

void App::actualizar(float /*deltaTiempo*/)
{
    // Por ahora no hay simulación.
    // Aquí más adelante se actualizarán escenario, riesgo y transforms.
}

void App::renderizar()
{
    // Render mínimo: limpiar pantalla.
    glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Demo Hito 2: pseudo-cuerpo
    gCuerpo.actualizarJerarquia();
    gCuerpo.dibujar(gRenderer, gCubo, gCilindro, gEsfera, gCamara);
}

void App::dibujarDemoCubo()
{
    // Mantengo este método por si quieren volver a la demo del cubo,
    // pero ya no se usa en renderizar() porque ahora mostramos el pseudo-cuerpo.
    const float t = static_cast<float>(glfwGetTime());
    glm::mat4 modelo(1.0f);
    modelo = glm::rotate(modelo, t * 0.6f, glm::vec3(0.3f, 1.0f, 0.0f));

    const glm::vec3 color(0.2f, 0.8f, 0.9f);

    gRenderer.dibujar(gCubo, modelo, color, gCamara);
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
