#include "Mesh.h"

#include <iostream>

static void intercambiar(GLuint& a, GLuint& b)
{
    const GLuint tmp = a;
    a = b;
    b = tmp;
}

Mesh::~Mesh()
{
    if (mVbo != 0) glDeleteBuffers(1, &mVbo);
    if (mVao != 0) glDeleteVertexArrays(1, &mVao);
    mVbo = 0;
    mVao = 0;
}

Mesh::Mesh(Mesh&& other) noexcept
{
    // Tomamos "posesión" de los recursos OpenGL del otro mesh
    mVao = other.mVao;
    mVbo = other.mVbo;
    mCantidadVertices = other.mCantidadVertices;

    other.mVao = 0;
    other.mVbo = 0;
    other.mCantidadVertices = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this == &other) return *this;

    // Liberamos lo nuestro
    if (mVbo != 0) glDeleteBuffers(1, &mVbo);
    if (mVao != 0) glDeleteVertexArrays(1, &mVao);

    // Tomamos lo del otro
    mVao = other.mVao;
    mVbo = other.mVbo;
    mCantidadVertices = other.mCantidadVertices;

    other.mVao = 0;
    other.mVbo = 0;
    other.mCantidadVertices = 0;

    return *this;
}

bool Mesh::crearSoloPosiciones(const float* datos, int cantidadFloats)
{
    if (!datos || cantidadFloats <= 0 || (cantidadFloats % 3) != 0)
    {
        std::cout << "Mesh: datos inválidos.\n";
        return false;
    }

    // Limpiar si existía
    if (mVbo != 0) glDeleteBuffers(1, &mVbo);
    if (mVao != 0) glDeleteVertexArrays(1, &mVao);
    mVbo = 0;
    mVao = 0;

    mCantidadVertices = cantidadFloats / 3;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cantidadFloats, datos, GL_STATIC_DRAW);

    // layout(location = 0) -> vec3 posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

bool Mesh::crearConUV(const float* datos, int cantidadFloats)
{
    // Cada vértice: x,y,z, u,v → 5 floats
    if (!datos || cantidadFloats <= 0 || (cantidadFloats % 5) != 0)
    {
        std::cout << "Mesh: datos UV inválidos (deben ser múltiplo de 5).\n";
        return false;
    }

    if (mVbo != 0) glDeleteBuffers(1, &mVbo);
    if (mVao != 0) glDeleteVertexArrays(1, &mVao);
    mVbo = 0; mVao = 0;

    mCantidadVertices = cantidadFloats / 5;
    mTieneUV = true;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cantidadFloats, datos, GL_STATIC_DRAW);

    // layout(location = 0) → vec3 posición (stride=5*sizeof(float), offset=0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout(location = 1) → vec2 UV (stride=5*sizeof(float), offset=3*sizeof(float))
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

bool Mesh::crearConUVNormal(const float* datos, int cantidadFloats)
{
    // Cada vértice: x,y,z, u,v, nx,ny,nz → 8 floats
    if (!datos || cantidadFloats <= 0 || (cantidadFloats % 8) != 0)
    {
        std::cout << "Mesh: datos UVNormal inválidos (deben ser múltiplo de 8).\n";
        return false;
    }

    if (mVbo != 0) glDeleteBuffers(1, &mVbo);
    if (mVao != 0) glDeleteVertexArrays(1, &mVao);
    mVbo = 0; mVao = 0;

    mCantidadVertices = cantidadFloats / 8;
    mTieneUV     = true;
    mTieneNormal = true;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cantidadFloats, datos, GL_STATIC_DRAW);

    const int stride = 8 * sizeof(float);

    // layout(location = 0) → vec3 posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // layout(location = 1) → vec2 UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // layout(location = 2) → vec3 normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Mesh::dibujar() const
{
    glBindVertexArray(mVao);
    glDrawArrays(GL_TRIANGLES, 0, mCantidadVertices);
    glBindVertexArray(0);
}
