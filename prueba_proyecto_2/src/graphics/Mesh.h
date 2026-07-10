#pragma once

#include <GL/glew.h>

// Mesh: encapsula VAO/VBO para geometría 3D.
// Soporta dos modos:
//   1. Solo posiciones (layout 0 = vec3 pos)
//   2. Posiciones + UV (layout 0 = vec3 pos, layout 1 = vec2 uv)
class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    // Permitimos mover (move) pero no copiar.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Modo 1: solo posiciones (x,y,z) por vértice — interleaved stride=3.
    bool crearSoloPosiciones(const float* datos, int cantidadFloats);

    // Modo 2: posiciones + coordenadas UV (x,y,z, u,v) por vértice — interleaved stride=5.
    bool crearConUV(const float* datos, int cantidadFloats);

    // Modo 3: posiciones + UV + normales (x,y,z, u,v, nx,ny,nz) — interleaved stride=8.
    // Layouts:  0=vec3 pos,  1=vec2 uv,  2=vec3 normal
    // Usado por el shader Phong para calcular iluminación.
    bool crearConUVNormal(const float* datos, int cantidadFloats);

    bool tieneNormal() const { return mTieneNormal; }

    // Dibuja el mesh con glDrawArrays (triángulos).
    void dibujar() const;

    bool tieneUV() const { return mTieneUV; }

private:
    GLuint mVao = 0;
    GLuint mVbo = 0;
    int    mCantidadVertices = 0;
    bool   mTieneUV     = false;
    bool   mTieneNormal = false;
};
