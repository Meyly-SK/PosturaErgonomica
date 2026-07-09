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

    // Pose base (postura neutra) — se guarda en inicializar() y NO se modifica.
    // setScenario() calcula la posicion/rotacion nueva a partir de estos valores.
    glm::vec3 posicionBase{0.0f};
    glm::vec3 rotacionBase{0.0f};

    // Color base (se reemplazará por color según riesgo)
    glm::vec3 color{0.8f, 0.8f, 0.8f};
    glm::vec3 colorBase{0.8f, 0.8f, 0.8f}; // color original (para restaurar)

    // Matriz calculada por HumanBody::actualizarJerarquia()
    glm::mat4 matrizMundo{1.0f};

    // Offset visual: desplazamiento de la malla DENTRO del espacio del nodo,
    // aplicado DESPUÉS de rotar pero ANTES de escalar.
    //
    // PARA QUÉ SIRVE:
    //   Permite que el nodo de articulación tenga escala {1,1,1} (sin distorsión
    //   para los hijos) mientras la malla visual se desplaza al lugar correcto.
    //
    //   Ejemplo: un cilindro de brazo que "cuelga" desde el hombro:
    //     offsetVisual = {0, -0.5, 0}  →  la malla baja al hombro,
    //     el nodo de rotación queda en el pivot exacto.
    //
    //   Orden de la matrizLocal:
    //     translate(posicion) * rotate(euler) * translate(offsetVisual) * scale(escala)
    glm::vec3 offsetVisual{0.0f};

    // Construye la matriz local a partir de posición/rotación/offsetVisual/escala.
    glm::mat4 matrizLocal() const;
};
