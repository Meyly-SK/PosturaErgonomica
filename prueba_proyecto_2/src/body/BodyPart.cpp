#include "BodyPart.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 BodyPart::matrizLocal() const
{
    glm::mat4 m(1.0f);

    // Orden: trasladar -> rotar -> escalar
    // Usamos posicion como coordenada directa (puede ser absoluta o relativa al padre).
    m = glm::translate(m, posicion);

    m = glm::rotate(m, glm::radians(rotacionEulerGrados.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, glm::radians(rotacionEulerGrados.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, glm::radians(rotacionEulerGrados.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m = glm::scale(m, escala);

    return m;
}
