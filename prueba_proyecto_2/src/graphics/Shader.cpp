#include "Shader.h"

#include <iostream>

Shader::~Shader()
{
    if (mPrograma != 0)
    {
        glDeleteProgram(mPrograma);
        mPrograma = 0;
    }
}

GLuint Shader::compilarShader(GLenum tipo, const char* codigo)
{
    GLuint shader = glCreateShader(tipo);
    glShaderSource(shader, 1, &codigo, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cout << "Error compilando shader: " << log << "\n";
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::compilarDesdeCodigo(const char* codigoVertex, const char* codigoFragment)
{
    // Limpiar si ya existía uno
    if (mPrograma != 0)
    {
        glDeleteProgram(mPrograma);
        mPrograma = 0;
    }

    const GLuint vs = compilarShader(GL_VERTEX_SHADER, codigoVertex);
    if (vs == 0) return false;

    const GLuint fs = compilarShader(GL_FRAGMENT_SHADER, codigoFragment);
    if (fs == 0)
    {
        glDeleteShader(vs);
        return false;
    }

    mPrograma = glCreateProgram();
    glAttachShader(mPrograma, vs);
    glAttachShader(mPrograma, fs);
    glLinkProgram(mPrograma);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = 0;
    glGetProgramiv(mPrograma, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(mPrograma, sizeof(log), nullptr, log);
        std::cout << "Error linkeando programa: " << log << "\n";
        glDeleteProgram(mPrograma);
        mPrograma = 0;
        return false;
    }

    return true;
}

void Shader::usar() const
{
    glUseProgram(mPrograma);
}

void Shader::setMat4(const char* nombre, const float* ptrMat4) const
{
    const GLint loc = glGetUniformLocation(mPrograma, nombre);
    glUniformMatrix4fv(loc, 1, GL_FALSE, ptrMat4);
}

void Shader::setVec3(const char* nombre, float x, float y, float z) const
{
    const GLint loc = glGetUniformLocation(mPrograma, nombre);
    glUniform3f(loc, x, y, z);
}

void Shader::setFloat(const char* nombre, float valor) const
{
    const GLint loc = glGetUniformLocation(mPrograma, nombre);
    glUniform1f(loc, valor);
}

void Shader::setInt(const char* nombre, int valor) const
{
    const GLint loc = glGetUniformLocation(mPrograma, nombre);
    glUniform1i(loc, valor);
}
