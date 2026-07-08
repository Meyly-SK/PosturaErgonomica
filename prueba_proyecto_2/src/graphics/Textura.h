#pragma once

#include <GL/glew.h>
#include <string>

// ==========================================================================
// Textura — carga una imagen (jpg/png) y la sube a OpenGL como textura 2D.
// Usa stb_image (ya disponible via vcpkg).
// ==========================================================================
class Textura
{
public:
    ~Textura();

    // Carga la imagen desde disco y sube a GPU.
    // Devuelve true si tuvo éxito.
    bool cargarDesdeArchivo(const std::string& ruta);

    // Bind de la textura a la unidad indicada (0, 1, 2, ...)
    void usar(int unidad = 0) const;

    // Liberar recursos de OpenGL
    void liberar();

    bool estaLista() const { return mId != 0; }

private:
    GLuint mId = 0;
};
