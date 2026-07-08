#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

// ---------------------------------------------------------------------------
// Shader sin textura (modo original — solo color sólido)
// ---------------------------------------------------------------------------
static const char* kVertexSinTex = R"(
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

static const char* kFragmentSinTex = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

// ---------------------------------------------------------------------------
// Shader con textura + tinte de riesgo
//
// Mezcla la textura de madera con el color de riesgo:
//   - uMezcla = 0.0 → solo textura (partes sin riesgo asignado)
//   - uMezcla = 0.6 → 40% textura + 60% color riesgo (zonas con riesgo)
//   - uMezcla = 1.0 → solo color
// ---------------------------------------------------------------------------
static const char* kVertexConTex = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uModelo;
uniform mat4 uVista;
uniform mat4 uProyeccion;

out vec2 vTexCoord;

void main()
{
    gl_Position = uProyeccion * uVista * uModelo * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
}
)";

static const char* kFragmentConTex = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTextura;
uniform vec3      uColor;
uniform float     uMezcla;   // 0 = solo textura, 1 = solo color

void main()
{
    vec3 texColor = texture(uTextura, vTexCoord).rgb;
    // Mezcla: la textura se tinta con el color de riesgo
    vec3 resultado = mix(texColor, uColor, uMezcla);
    FragColor = vec4(resultado, 1.0);
}
)";

// ---------------------------------------------------------------------------
// inicializar
// ---------------------------------------------------------------------------
bool Renderer::inicializar()
{
    if (!mShaderSinTex.compilarDesdeCodigo(kVertexSinTex, kFragmentSinTex))
        return false;
    if (!mShaderConTex.compilarDesdeCodigo(kVertexConTex, kFragmentConTex))
        return false;
    return true;
}

// ---------------------------------------------------------------------------
// dibujar (modo original — sin textura)
// ---------------------------------------------------------------------------
void Renderer::dibujar(const Mesh& mesh,
                       const glm::mat4& modelo,
                       const glm::vec3& color,
                       const Camera& camara)
{
    mShaderSinTex.usar();

    mShaderSinTex.setMat4("uModelo",      glm::value_ptr(modelo));
    mShaderSinTex.setMat4("uVista",       glm::value_ptr(camara.obtenerVista()));
    mShaderSinTex.setMat4("uProyeccion",  glm::value_ptr(camara.obtenerProyeccion()));
    mShaderSinTex.setVec3("uColor", color.x, color.y, color.z);

    mesh.dibujar();
}

// ---------------------------------------------------------------------------
// dibujarConTextura
//
// Dibuja la malla usando la textura de madera mezclada con el color de riesgo.
// Si el color de riesgo es el color base (sin peligro), la mezcla es baja
// y predomina la textura. Si hay riesgo (verde/amarillo/rojo), el color tinta.
// ---------------------------------------------------------------------------
void Renderer::dibujarConTextura(const Mesh& mesh,
                                 const glm::mat4& modelo,
                                 const glm::vec3& color,
                                 const Camera& camara,
                                 const Textura& textura,
                                 float mezcla)
{
    textura.usar(0);  // bind textura en unidad 0

    mShaderConTex.usar();

    mShaderConTex.setMat4("uModelo",     glm::value_ptr(modelo));
    mShaderConTex.setMat4("uVista",      glm::value_ptr(camara.obtenerVista()));
    mShaderConTex.setMat4("uProyeccion", glm::value_ptr(camara.obtenerProyeccion()));
    mShaderConTex.setVec3("uColor", color.x, color.y, color.z);
    mShaderConTex.setFloat("uMezcla", mezcla);
    mShaderConTex.setInt("uTextura", 0);

    mesh.dibujar();
}
