#pragma once

#include <glm/glm.hpp>

// Cámara con modo FPS simple (WASD + mouse).
// Mantiene yaw/pitch y calcula vista/proyección.
class Camera
{
public:
    void setAspecto(float ancho, float alto);

    // Matrices para el shader
    glm::mat4 obtenerVista() const;
    glm::mat4 obtenerProyeccion() const;

    // Movimiento / rotación
    void procesarMouseDelta(float dx, float dy);
    void moverAdelante(float delta);
    void moverDerecha(float delta);
    void moverArriba(float delta);

    // Parámetros
    glm::vec3 posicion{0.0f, 0.0f, 3.0f};

    float yawGrados = -90.0f;  // mirando hacia -Z
    float pitchGrados = 0.0f;

    float velocidad = 2.5f;       // unidades/segundo
    float sensibilidad = 0.10f;   // grados por pixel aprox

    float fovGrados = 60.0f;
    float cerca = 0.1f;
    float lejos = 100.0f;

private:
    float mAspecto = 800.0f / 600.0f;

    glm::vec3 frente() const;  // vector forward desde yaw/pitch
};
