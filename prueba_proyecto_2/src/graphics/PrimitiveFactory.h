#pragma once

#include "Mesh.h"

// Generador de primitivas.
// Para el MVP, construimos geometría en código para no depender de modelos externos.
class PrimitiveFactory
{
public:
    // ---- Sin UV (modo original) ----
    static Mesh crearCubo();
    static Mesh crearCilindro(float radio, float alto, int segmentos);
    static Mesh crearEsfera(float radio, int sectores, int anillos);

    // ---- Con UV (para texturas) ----
    // Las UVs cubren la malla completa con repetición para texturas de madera.
    static Mesh crearCuboUV();
    static Mesh crearCilindroUV(float radio, float alto, int segmentos);
    static Mesh crearEsferaUV(float radio, int sectores, int anillos);
};
