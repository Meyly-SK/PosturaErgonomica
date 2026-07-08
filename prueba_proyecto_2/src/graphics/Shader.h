#pragma once

#include <string>

#include <GL/glew.h>

// Clase sencilla para compilar/usar shaders.
// Mantiene el OpenGL "encapsulado" y evita repetir código.
class Shader
{
public:
    Shader() = default;
    ~Shader();

    // Compila vertex/fragment desde strings en memoria.
    bool compilarDesdeCodigo(const char* codigoVertex, const char* codigoFragment);

    // Activa el programa.
    void usar() const;

    // Setters de uniforms
    void setMat4 (const char* nombre, const float* ptrMat4) const;
    void setVec3 (const char* nombre, float x, float y, float z) const;
    void setFloat(const char* nombre, float valor) const;
    void setInt  (const char* nombre, int valor) const;

    GLuint idPrograma() const { return mPrograma; }

private:
    GLuint mPrograma = 0;

    static GLuint compilarShader(GLenum tipo, const char* codigo);
};
