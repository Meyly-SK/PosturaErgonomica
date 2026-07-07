#include "Input.h"

#include <GLFW/glfw3.h>

#include "../graphics/Camera.h"

static Camera* gCamara = nullptr;

static bool gMouseCapturado = false;
static bool gPrimerMouse = true;
static double gUltX = 0.0;
static double gUltY = 0.0;

void Input::setCamera(Camera* camara)
{
    gCamara = camara;
}

void Input::setCapturaMouse(bool activar)
{
    gMouseCapturado = activar;
    gPrimerMouse = true; // reiniciar acumulación al cambiar modo
}

static void procesarMouse(GLFWwindow* ventana)
{
    if (!gCamara || !gMouseCapturado) return;

    double x, y;
    glfwGetCursorPos(ventana, &x, &y);

    if (gPrimerMouse)
    {
        gUltX = x;
        gUltY = y;
        gPrimerMouse = false;
        return;
    }

    const float dx = static_cast<float>(x - gUltX);
    const float dy = static_cast<float>(y - gUltY);

    gUltX = x;
    gUltY = y;

    gCamara->procesarMouseDelta(dx, dy);
}

void Input::procesar(GLFWwindow* ventana, float deltaTiempo)
{
    if (!ventana) return;

    // Salir
    if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ventana, GLFW_TRUE);
    }

    // Nota: el cambio de modo debug del cuerpo se manejará desde App (donde está el HumanBody).

    // Toggle captura de mouse con tecla M
    static bool mAntes = false;
    const bool mAhora = glfwGetKey(ventana, GLFW_KEY_M) == GLFW_PRESS;
    if (mAhora && !mAntes)
    {
        gMouseCapturado = !gMouseCapturado;
        gPrimerMouse = true;

        glfwSetInputMode(ventana, GLFW_CURSOR, gMouseCapturado ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    mAntes = mAhora;

    // Si no hay cámara conectada, no movemos nada
    if (!gCamara) return;

    const float vel = gCamara->velocidad * deltaTiempo;

    // Movimiento FPS clásico
    if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) gCamara->moverAdelante(vel);
    if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) gCamara->moverAdelante(-vel);
    if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) gCamara->moverDerecha(vel);
    if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) gCamara->moverDerecha(-vel);

    // Subir/bajar (útil para inspeccionar)
    if (glfwGetKey(ventana, GLFW_KEY_E) == GLFW_PRESS) gCamara->moverArriba(vel);
    if (glfwGetKey(ventana, GLFW_KEY_Q) == GLFW_PRESS) gCamara->moverArriba(-vel);

    // Mouse look
    procesarMouse(ventana);
}
