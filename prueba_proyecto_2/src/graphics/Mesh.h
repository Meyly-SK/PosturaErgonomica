#pragma once

#include <GL/glew.h>

// Mesh mínimo: solo posiciones 3D.
// Encapsula VAO/VBO para no repetir OpenGL en todo el proyecto.
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

    // Crea un mesh usando un arreglo de floats (x,y,z) y cantidad de vértices.
    bool crearSoloPosiciones(const float* datos, int cantidadFloats);

    // Dibuja el mesh con glDrawArrays (triángulos).
    void dibujar() const;

private:
    GLuint mVao = 0;
    GLuint mVbo = 0;
    int mCantidadVertices = 0;
};
