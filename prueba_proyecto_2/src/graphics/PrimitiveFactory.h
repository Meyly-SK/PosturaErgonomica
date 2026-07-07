#pragma once

#include "Mesh.h"

// Generador de primitivas.
// Para el MVP, construimos geometría en código para no depender de modelos externos.
class PrimitiveFactory
{
public:
    // Crea un cubo centrado en el origen, tamaño 1 (de -0.5 a +0.5).
    static Mesh crearCubo();

    // Crea un cilindro "de pie" (eje Y), centrado en el origen.
    // - radio: tamaño en X/Z
    // - alto: tamaño en Y
    // - segmentos: calidad (más segmentos = más redondo)
    static Mesh crearCilindro(float radio, float alto, int segmentos);

    // Crea una esfera centrada en el origen.
    // - radio: tamaño
    // - sectores/anillos: calidad
    static Mesh crearEsfera(float radio, int sectores, int anillos);
};
