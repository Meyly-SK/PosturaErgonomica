#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../simulation/Types.h"

// Parte del cuerpo: solo datos de transform y color.
// No debe tener OpenGL directo.
struct BodyPart
{
    enum class TipoMalla
    {
        Cubo,
        Cilindro,
        Esfera
    };

    std::string nombre;

    // Qué malla usar para representar esta parte (detalle visual).
    TipoMalla tipoMalla = TipoMalla::Cubo;

    // Zona de riesgo asociada a esta parte (para pintarla luego).
    ZonaRiesgo zona = ZonaRiesgo::Ninguna;

    // Jerarquía: -1 = sin padre (raíz / absoluto en mundo)
    int indicePadre = -1;

    // Transformación local (respecto al padre, o absoluta si indicePadre == -1)
    glm::vec3 posicion{0.0f};
    glm::vec3 rotacionEulerGrados{0.0f}; // (x,y,z) en grados
    glm::vec3 escala{1.0f};

    // Color base (se reemplazará por color según riesgo)
    glm::vec3 color{0.8f, 0.8f, 0.8f};

    // Matriz calculada por HumanBody::actualizarJerarquia()
    glm::mat4 matrizMundo{1.0f};

    // Construye la matriz local a partir de posición/rotación/escala.
    glm::mat4 matrizLocal() const;
};
