#include "Camera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

void Camera::setAspecto(float ancho, float alto)
{
    if (alto <= 0.0f) alto = 1.0f;
    mAspecto = ancho / alto;
}

glm::vec3 Camera::frente() const
{
    // Convertimos yaw/pitch a dirección
    const float yaw = glm::radians(yawGrados);
    const float pitch = glm::radians(pitchGrados);

    glm::vec3 f;
    f.x = std::cos(yaw) * std::cos(pitch);
    f.y = std::sin(pitch);
    f.z = std::sin(yaw) * std::cos(pitch);
    return glm::normalize(f);
}

glm::mat4 Camera::obtenerVista() const
{
    const glm::vec3 f = frente();
    return glm::lookAt(posicion, posicion + f, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::obtenerProyeccion() const
{
    return glm::perspective(glm::radians(fovGrados), mAspecto, cerca, lejos);
}

void Camera::procesarMouseDelta(float dx, float dy)
{
    yawGrados += dx * sensibilidad;
    pitchGrados -= dy * sensibilidad; // invertir para que mover mouse arriba mire arriba

    pitchGrados = std::clamp(pitchGrados, -89.0f, 89.0f);
}

void Camera::moverAdelante(float delta)
{
    posicion += frente() * delta;
}

void Camera::moverDerecha(float delta)
{
    const glm::vec3 f = frente();
    const glm::vec3 derecha = glm::normalize(glm::cross(f, glm::vec3(0.0f, 1.0f, 0.0f)));
    posicion += derecha * delta;
}

void Camera::moverArriba(float delta)
{
    posicion += glm::vec3(0.0f, 1.0f, 0.0f) * delta;
}
