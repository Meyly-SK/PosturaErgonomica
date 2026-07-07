#include "PrimitiveFactory.h"

#include <cmath>
#include <vector>

// Cubo con 12 triángulos (36 vértices) solo con posiciones.
// Nota: no incluye normales/UV todavía.
Mesh PrimitiveFactory::crearCubo()
{
    // 6 caras * 2 triángulos * 3 vertices = 36 vertices
    // Cada vertex: 3 floats (x, y, z)
    static const float vertices[] = {
        // Cara frontal (z = +0.5)
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Cara trasera (z = -0.5)
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,

        // Cara izquierda (x = -0.5)
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Cara derecha (x = +0.5)
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,

        // Cara superior (y = +0.5)
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

        // Cara inferior (y = -0.5)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
    };

    Mesh m;
    m.crearSoloPosiciones(vertices, static_cast<int>(sizeof(vertices) / sizeof(float)));
    return m;
}

// Cilindro simple (sin tapas), orientado al eje Y.
// Se construye como triángulos (2 por segmento).
Mesh PrimitiveFactory::crearCilindro(float radio, float alto, int segmentos)
{
    if (segmentos < 3) segmentos = 3;
    if (radio <= 0.0f) radio = 0.1f;
    if (alto <= 0.0f) alto = 0.1f;

    const float yArriba = alto * 0.5f;
    const float yAbajo = -alto * 0.5f;

    std::vector<float> v;
    v.reserve(segmentos * 6 * 3);

    const float paso = 2.0f * 3.14159265f / static_cast<float>(segmentos);

    for (int i = 0; i < segmentos; i++)
    {
        const float a0 = paso * static_cast<float>(i);
        const float a1 = paso * static_cast<float>(i + 1);

        const float x0 = std::cos(a0) * radio;
        const float z0 = std::sin(a0) * radio;
        const float x1 = std::cos(a1) * radio;
        const float z1 = std::sin(a1) * radio;

        // 1er triángulo
        v.push_back(x0); v.push_back(yAbajo); v.push_back(z0);
        v.push_back(x0); v.push_back(yArriba); v.push_back(z0);
        v.push_back(x1); v.push_back(yArriba); v.push_back(z1);

        // 2do triángulo
        v.push_back(x0); v.push_back(yAbajo); v.push_back(z0);
        v.push_back(x1); v.push_back(yArriba); v.push_back(z1);
        v.push_back(x1); v.push_back(yAbajo); v.push_back(z1);
    }

    Mesh m;
    m.crearSoloPosiciones(v.data(), static_cast<int>(v.size()));
    return m;
}

// Esfera por "stacks/slices" (anillos/sectores), solo posiciones.
Mesh PrimitiveFactory::crearEsfera(float radio, int sectores, int anillos)
{
    if (radio <= 0.0f) radio = 0.1f;
    if (sectores < 3) sectores = 3;
    if (anillos < 2) anillos = 2;

    std::vector<float> v;
    v.reserve(sectores * anillos * 6 * 3);

    const float PI = 3.14159265f;

    for (int r = 0; r < anillos; r++)
    {
        const float v0 = static_cast<float>(r) / static_cast<float>(anillos);
        const float v1 = static_cast<float>(r + 1) / static_cast<float>(anillos);

        const float phi0 = (v0 * PI) - (PI * 0.5f);
        const float phi1 = (v1 * PI) - (PI * 0.5f);

        const float y0 = std::sin(phi0) * radio;
        const float y1 = std::sin(phi1) * radio;

        const float rr0 = std::cos(phi0) * radio;
        const float rr1 = std::cos(phi1) * radio;

        for (int s = 0; s < sectores; s++)
        {
            const float u0 = static_cast<float>(s) / static_cast<float>(sectores);
            const float u1 = static_cast<float>(s + 1) / static_cast<float>(sectores);

            const float theta0 = u0 * 2.0f * PI;
            const float theta1 = u1 * 2.0f * PI;

            const float x00 = std::cos(theta0) * rr0;
            const float z00 = std::sin(theta0) * rr0;

            const float x01 = std::cos(theta1) * rr0;
            const float z01 = std::sin(theta1) * rr0;

            const float x10 = std::cos(theta0) * rr1;
            const float z10 = std::sin(theta0) * rr1;

            const float x11 = std::cos(theta1) * rr1;
            const float z11 = std::sin(theta1) * rr1;

            // 2 triángulos del quad entre anillos
            v.push_back(x00); v.push_back(y0); v.push_back(z00);
            v.push_back(x10); v.push_back(y1); v.push_back(z10);
            v.push_back(x11); v.push_back(y1); v.push_back(z11);

            v.push_back(x00); v.push_back(y0); v.push_back(z00);
            v.push_back(x11); v.push_back(y1); v.push_back(z11);
            v.push_back(x01); v.push_back(y0); v.push_back(z01);
        }
    }

    Mesh m;
    m.crearSoloPosiciones(v.data(), static_cast<int>(v.size()));
    return m;
}
