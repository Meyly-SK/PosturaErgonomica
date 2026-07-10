#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Textura.h"

// Renderer: centraliza el dibujado.
// Soporta dos modos:
//   1. Sin textura (color sólido, modo original)
//   2. Con textura (textura de madera mezclada con color de riesgo)
class Renderer
{
public:
    // Inicializa shaders (sin textura y con textura).
    bool inicializar();

    // Modo 1: dibuja con color sólido (sin textura)
    void dibujar(const Mesh& mesh,
                 const glm::mat4& modelo,
                 const glm::vec3& color,
                 const Camera& camara);

    // Modo 2: dibuja con textura mezclada con el color de riesgo
    // mezcla: 0.0 = solo textura, 1.0 = solo color, 0.5 = mitad y mitad
    void dibujarConTextura(const Mesh& mesh,
                           const glm::mat4& modelo,
                           const glm::vec3& color,
                           const Camera& camara,
                           const Textura& textura,
                           float mezcla = 0.5f);

    // Modo 3: dibuja con textura + tinte de riesgo + iluminación Phong (H6-C)
    // Requiere mesh con normales (crearConUVNormal).
    // posLuz: posición de la fuente de luz en espacio mundo.
    void dibujarConPhong(const Mesh& mesh,
                         const glm::mat4& modelo,
                         const glm::vec3& color,
                         const Camera& camara,
                         const Textura& textura,
                         float mezcla,
                         const glm::vec3& posLuz);

private:
    Shader mShaderSinTex;   // shader original (solo color)
    Shader mShaderConTex;   // shader con textura + tinte
    Shader mShaderPhong;    // shader con textura + tinte + iluminación Phong
};
