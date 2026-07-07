#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

// Renderer mínimo: centraliza el dibujado.
// Importante: NO calcula riesgo, NO decide escenarios, solo dibuja.
class Renderer
{
public:
    // Inicializa recursos mínimos (shader base).
    bool inicializar();

    // Dibuja un mesh con una matriz de modelo y un color.
    void dibujar(const Mesh& mesh, const glm::mat4& modelo, const glm::vec3& color, const Camera& camara);

private:
    Shader mShader;
};
