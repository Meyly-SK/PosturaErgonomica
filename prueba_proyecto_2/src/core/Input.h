#pragma once

struct GLFWwindow;
class Camera;

// Entrada de usuario (teclado / mouse).
// Se mantiene separado para que App no se llene de código de input.
class Input
{
public:
    // Procesa input por frame.
    // - deltaTiempo: tiempo entre frames para movimientos consistentes
    static void procesar(GLFWwindow* ventana, float deltaTiempo);

    // Conecta/desconecta la cámara a los controles FPS.
    static void setCamera(Camera* camara);

    // Captura o libera el mouse (cursor oculto) para mirar con mouse.
    static void setCapturaMouse(bool activar);
};
