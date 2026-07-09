#include "BodyPart.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 BodyPart::matrizLocal() const
{
    glm::mat4 m(1.0f);

    // Orden: trasladar → rotar → offsetVisual → escalar
    //
    // El offsetVisual permite que el nodo de articulación tenga escala {1,1,1}
    // (sin herencia de distorsión para los hijos) mientras la malla visual se
    // posiciona correctamente. Se aplica DESPUÉS de la rotación y ANTES de escalar,
    // por lo que está en el espacio ya rotado del nodo.
    m = glm::translate(m, posicion);

    m = glm::rotate(m, glm::radians(rotacionEulerGrados.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, glm::radians(rotacionEulerGrados.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, glm::radians(rotacionEulerGrados.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Aplicar offset visual (desplazamiento de malla en espacio local rotado)
    if (offsetVisual.x != 0.0f || offsetVisual.y != 0.0f || offsetVisual.z != 0.0f)
        m = glm::translate(m, offsetVisual);

    m = glm::scale(m, escala);

    return m;
}
