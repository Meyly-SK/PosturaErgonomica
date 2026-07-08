#include "Textura.h"

#include <iostream>

// stb_image: librería header-only para carga de imágenes.
// Solo definir STB_IMAGE_IMPLEMENTATION UNA VEZ en todo el proyecto.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Textura::~Textura()
{
    liberar();
}

bool Textura::cargarDesdeArchivo(const std::string& ruta)
{
    liberar();

    // stb_image carga la imagen con el eje Y invertido respecto a OpenGL.
    // flip vertical = true para que las UVs queden correctas.
    stbi_set_flip_vertically_on_load(true);

    int ancho = 0, alto = 0, canales = 0;
    unsigned char* datos = stbi_load(ruta.c_str(), &ancho, &alto, &canales, 0);

    if (!datos)
    {
        std::cout << "Textura: no se pudo cargar '" << ruta << "'\n";
        std::cout << "  Razon: " << stbi_failure_reason() << "\n";
        return false;
    }

    // Determinar formato según número de canales
    GLenum formato = GL_RGB;
    if (canales == 1) formato = GL_RED;
    else if (canales == 3) formato = GL_RGB;
    else if (canales == 4) formato = GL_RGBA;

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    // Parámetros de wrapping y filtro
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(formato),
                 ancho, alto, 0, formato, GL_UNSIGNED_BYTE, datos);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(datos);

    std::cout << "Textura cargada: " << ruta
              << " (" << ancho << "x" << alto << ", " << canales << " canales)\n";
    return true;
}

void Textura::usar(int unidad) const
{
    glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(unidad));
    glBindTexture(GL_TEXTURE_2D, mId);
}

void Textura::liberar()
{
    if (mId != 0)
    {
        glDeleteTextures(1, &mId);
        mId = 0;
    }
}
