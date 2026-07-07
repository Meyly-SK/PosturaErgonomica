#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

bool Renderer::inicializar()
{
    // Shader mínimo (solo posiciones + color sólido)
    const char* codigoVertex = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uModelo;
uniform mat4 uVista;
uniform mat4 uProyeccion;

void main()
{
    gl_Position = uProyeccion * uVista * uModelo * vec4(aPos, 1.0);
}
)";

    const char* codigoFragment = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

    return mShader.compilarDesdeCodigo(codigoVertex, codigoFragment);
}

void Renderer::dibujar(const Mesh& mesh, const glm::mat4& modelo, const glm::vec3& color, const Camera& camara)
{
    mShader.usar();

    const glm::mat4 vista = camara.obtenerVista();
    const glm::mat4 proy = camara.obtenerProyeccion();

    mShader.setMat4("uModelo", glm::value_ptr(modelo));
    mShader.setMat4("uVista", glm::value_ptr(vista));
    mShader.setMat4("uProyeccion", glm::value_ptr(proy));
    mShader.setVec3("uColor", color.x, color.y, color.z);

    mesh.dibujar();
}
