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
// ==========================================================================
// Versiones con UV
// ==========================================================================

// Cubo con UV: cada cara tiene UVs 0-1 mapeadas por triángulo
Mesh PrimitiveFactory::crearCuboUV()
{
    // 6 caras * 2 triángulos * 3 vértices = 36 vértices, 5 floats cada uno (x,y,z,u,v)
    static const float v[] = {
        // Frontal (z=+0.5)
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,
        // Trasera
        -0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,
         0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,  1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,
        // Izquierda
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        -0.5f,-0.5f, 0.5f,  1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,
        // Derecha
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  0.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  0.0f,1.0f,
        // Superior
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        // Inferior
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,1.0f,
    };
    Mesh m;
    m.crearConUV(v, static_cast<int>(sizeof(v) / sizeof(float)));
    return m;
}

// Cilindro con UV: mapeado cilíndrico (u=angulo/2pi, v=altura)
Mesh PrimitiveFactory::crearCilindroUV(float radio, float alto, int segmentos)
{
    if (segmentos < 3) segmentos = 3;
    if (radio <= 0.0f) radio = 0.1f;
    if (alto <= 0.0f) alto = 0.1f;

    const float yA = alto * 0.5f;
    const float yB = -alto * 0.5f;
    const float paso = 2.0f * 3.14159265f / static_cast<float>(segmentos);

    std::vector<float> v;
    v.reserve(segmentos * 6 * 5);

    for (int i = 0; i < segmentos; i++)
    {
        const float a0 = paso * static_cast<float>(i);
        const float a1 = paso * static_cast<float>(i + 1);

        const float x0 = std::cos(a0) * radio;
        const float z0 = std::sin(a0) * radio;
        const float x1 = std::cos(a1) * radio;
        const float z1 = std::sin(a1) * radio;

        const float u0 = static_cast<float>(i)     / static_cast<float>(segmentos);
        const float u1 = static_cast<float>(i + 1) / static_cast<float>(segmentos);

        // Tri 1
        v.push_back(x0); v.push_back(yB); v.push_back(z0); v.push_back(u0); v.push_back(0.0f);
        v.push_back(x0); v.push_back(yA); v.push_back(z0); v.push_back(u0); v.push_back(1.0f);
        v.push_back(x1); v.push_back(yA); v.push_back(z1); v.push_back(u1); v.push_back(1.0f);
        // Tri 2
        v.push_back(x0); v.push_back(yB); v.push_back(z0); v.push_back(u0); v.push_back(0.0f);
        v.push_back(x1); v.push_back(yA); v.push_back(z1); v.push_back(u1); v.push_back(1.0f);
        v.push_back(x1); v.push_back(yB); v.push_back(z1); v.push_back(u1); v.push_back(0.0f);
    }

    Mesh m;
    m.crearConUV(v.data(), static_cast<int>(v.size()));
    return m;
}

// Esfera con UV: mapeado esférico estándar
Mesh PrimitiveFactory::crearEsferaUV(float radio, int sectores, int anillos)
{
    if (radio <= 0.0f) radio = 0.1f;
    if (sectores < 3) sectores = 3;
    if (anillos < 2)  anillos = 2;

    std::vector<float> v;
    v.reserve(sectores * anillos * 6 * 5);

    const float PI = 3.14159265f;

    for (int r = 0; r < anillos; r++)
    {
        const float v0 = static_cast<float>(r)     / static_cast<float>(anillos);
        const float v1 = static_cast<float>(r + 1) / static_cast<float>(anillos);

        const float phi0 = (v0 * PI) - (PI * 0.5f);
        const float phi1 = (v1 * PI) - (PI * 0.5f);

        const float y0  = std::sin(phi0) * radio;
        const float y1  = std::sin(phi1) * radio;
        const float rr0 = std::cos(phi0) * radio;
        const float rr1 = std::cos(phi1) * radio;

        for (int s = 0; s < sectores; s++)
        {
            const float u0_ = static_cast<float>(s)     / static_cast<float>(sectores);
            const float u1_ = static_cast<float>(s + 1) / static_cast<float>(sectores);

            const float theta0 = u0_ * 2.0f * PI;
            const float theta1 = u1_ * 2.0f * PI;

            const float x00 = std::cos(theta0) * rr0, z00 = std::sin(theta0) * rr0;
            const float x01 = std::cos(theta1) * rr0, z01 = std::sin(theta1) * rr0;
            const float x10 = std::cos(theta0) * rr1, z10 = std::sin(theta0) * rr1;
            const float x11 = std::cos(theta1) * rr1, z11 = std::sin(theta1) * rr1;

            v.push_back(x00); v.push_back(y0); v.push_back(z00); v.push_back(u0_); v.push_back(v0);
            v.push_back(x10); v.push_back(y1); v.push_back(z10); v.push_back(u0_); v.push_back(v1);
            v.push_back(x11); v.push_back(y1); v.push_back(z11); v.push_back(u1_); v.push_back(v1);

            v.push_back(x00); v.push_back(y0); v.push_back(z00); v.push_back(u0_); v.push_back(v0);
            v.push_back(x11); v.push_back(y1); v.push_back(z11); v.push_back(u1_); v.push_back(v1);
            v.push_back(x01); v.push_back(y0); v.push_back(z01); v.push_back(u1_); v.push_back(v0);
        }
    }

    Mesh m;
    m.crearConUV(v.data(), static_cast<int>(v.size()));
    return m;
}

// ==========================================================================
// Versiones con UV + Normales (para iluminación Phong)
// Stride = 8 floats: x,y,z, u,v, nx,ny,nz
// ==========================================================================

Mesh PrimitiveFactory::crearCuboUVNormal()
{
    // Normal por cara: constante para todos los vértices de la misma cara
    // Frontal: nz=+1, Trasera: nz=-1, Izq: nx=-1, Der: nx=+1, Sup: ny=+1, Inf: ny=-1
    static const float v[] = {
        // Frontal (z=+0.5)  normal=(0,0,1)
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,  0,0,1,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,  0,0,1,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,  0,0,1,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,  0,0,1,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,  0,0,1,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,  0,0,1,
        // Trasera (z=-0.5)  normal=(0,0,-1)
        -0.5f,-0.5f,-0.5f,  1.0f,0.0f,  0,0,-1,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,  0,0,-1,
         0.5f,-0.5f,-0.5f,  0.0f,0.0f,  0,0,-1,
        -0.5f,-0.5f,-0.5f,  1.0f,0.0f,  0,0,-1,
        -0.5f, 0.5f,-0.5f,  1.0f,1.0f,  0,0,-1,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,  0,0,-1,
        // Izquierda (x=-0.5) normal=(-1,0,0)
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,  -1,0,0,
        -0.5f,-0.5f, 0.5f,  1.0f,0.0f,  -1,0,0,
        -0.5f, 0.5f, 0.5f,  1.0f,1.0f,  -1,0,0,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,  -1,0,0,
        -0.5f, 0.5f, 0.5f,  1.0f,1.0f,  -1,0,0,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,  -1,0,0,
        // Derecha (x=+0.5) normal=(1,0,0)
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,  1,0,0,
         0.5f, 0.5f, 0.5f,  0.0f,1.0f,  1,0,0,
         0.5f,-0.5f, 0.5f,  0.0f,0.0f,  1,0,0,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,  1,0,0,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,  1,0,0,
         0.5f, 0.5f, 0.5f,  0.0f,1.0f,  1,0,0,
        // Superior (y=+0.5) normal=(0,1,0)
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,  0,1,0,
        -0.5f, 0.5f, 0.5f,  0.0f,0.0f,  0,1,0,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,  0,1,0,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,  0,1,0,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,  0,1,0,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,  0,1,0,
        // Inferior (y=-0.5) normal=(0,-1,0)
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,  0,-1,0,
         0.5f,-0.5f, 0.5f,  1.0f,1.0f,  0,-1,0,
        -0.5f,-0.5f, 0.5f,  0.0f,1.0f,  0,-1,0,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,  0,-1,0,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,  0,-1,0,
         0.5f,-0.5f, 0.5f,  1.0f,1.0f,  0,-1,0,
    };
    Mesh m;
    m.crearConUVNormal(v, static_cast<int>(sizeof(v) / sizeof(float)));
    return m;
}

Mesh PrimitiveFactory::crearCilindroUVNormal(float radio, float alto, int segmentos)
{
    if (segmentos < 3) segmentos = 3;
    if (radio <= 0.0f) radio = 0.1f;
    if (alto <= 0.0f) alto = 0.1f;

    const float yA   = alto * 0.5f;
    const float yB   = -alto * 0.5f;
    const float paso = 2.0f * 3.14159265f / static_cast<float>(segmentos);

    std::vector<float> v;
    v.reserve(segmentos * 6 * 8);

    for (int i = 0; i < segmentos; i++)
    {
        const float a0 = paso * static_cast<float>(i);
        const float a1 = paso * static_cast<float>(i + 1);
        const float aMid = (a0 + a1) * 0.5f;

        // Posiciones
        const float x0 = std::cos(a0) * radio, z0 = std::sin(a0) * radio;
        const float x1 = std::cos(a1) * radio, z1 = std::sin(a1) * radio;

        // Normales radiales (apuntan hacia afuera del cilindro)
        const float nx0 = std::cos(a0), nz0 = std::sin(a0);
        const float nx1 = std::cos(a1), nz1 = std::sin(a1);

        // UVs
        const float u0 = static_cast<float>(i)     / static_cast<float>(segmentos);
        const float u1 = static_cast<float>(i + 1) / static_cast<float>(segmentos);

        // Tri 1: (x0,yB) -> (x0,yA) -> (x1,yA)
        v.push_back(x0); v.push_back(yB); v.push_back(z0); v.push_back(u0); v.push_back(0.0f); v.push_back(nx0); v.push_back(0); v.push_back(nz0);
        v.push_back(x0); v.push_back(yA); v.push_back(z0); v.push_back(u0); v.push_back(1.0f); v.push_back(nx0); v.push_back(0); v.push_back(nz0);
        v.push_back(x1); v.push_back(yA); v.push_back(z1); v.push_back(u1); v.push_back(1.0f); v.push_back(nx1); v.push_back(0); v.push_back(nz1);
        // Tri 2: (x0,yB) -> (x1,yA) -> (x1,yB)
        v.push_back(x0); v.push_back(yB); v.push_back(z0); v.push_back(u0); v.push_back(0.0f); v.push_back(nx0); v.push_back(0); v.push_back(nz0);
        v.push_back(x1); v.push_back(yA); v.push_back(z1); v.push_back(u1); v.push_back(1.0f); v.push_back(nx1); v.push_back(0); v.push_back(nz1);
        v.push_back(x1); v.push_back(yB); v.push_back(z1); v.push_back(u1); v.push_back(0.0f); v.push_back(nx1); v.push_back(0); v.push_back(nz1);
    }

    Mesh m;
    m.crearConUVNormal(v.data(), static_cast<int>(v.size()));
    return m;
}

Mesh PrimitiveFactory::crearEsferaUVNormal(float radio, int sectores, int anillos)
{
    if (radio <= 0.0f) radio = 0.1f;
    if (sectores < 3) sectores = 3;
    if (anillos < 2)  anillos = 2;

    std::vector<float> v;
    v.reserve(sectores * anillos * 6 * 8);

    const float PI = 3.14159265f;

    for (int r = 0; r < anillos; r++)
    {
        const float vv0 = static_cast<float>(r)     / static_cast<float>(anillos);
        const float vv1 = static_cast<float>(r + 1) / static_cast<float>(anillos);

        const float phi0 = (vv0 * PI) - (PI * 0.5f);
        const float phi1 = (vv1 * PI) - (PI * 0.5f);

        const float y0  = std::sin(phi0) * radio, rr0 = std::cos(phi0) * radio;
        const float y1  = std::sin(phi1) * radio, rr1 = std::cos(phi1) * radio;

        // Normales Y para la esfera: sin(phi)/radio = sin(phi) (radio normalizado)
        const float ny0 = std::sin(phi0), ny1 = std::sin(phi1);
        const float nr0 = std::cos(phi0), nr1 = std::cos(phi1);

        for (int s = 0; s < sectores; s++)
        {
            const float uu0 = static_cast<float>(s)     / static_cast<float>(sectores);
            const float uu1 = static_cast<float>(s + 1) / static_cast<float>(sectores);

            const float t0 = uu0 * 2.0f * PI, t1 = uu1 * 2.0f * PI;

            const float x00 = std::cos(t0)*rr0, z00 = std::sin(t0)*rr0;
            const float x01 = std::cos(t1)*rr0, z01 = std::sin(t1)*rr0;
            const float x10 = std::cos(t0)*rr1, z10 = std::sin(t0)*rr1;
            const float x11 = std::cos(t1)*rr1, z11 = std::sin(t1)*rr1;

            // Normales: dirección radial = pos/radio
            const float nx00=std::cos(t0)*nr0, nz00=std::sin(t0)*nr0;
            const float nx01=std::cos(t1)*nr0, nz01=std::sin(t1)*nr0;
            const float nx10=std::cos(t0)*nr1, nz10=std::sin(t0)*nr1;
            const float nx11=std::cos(t1)*nr1, nz11=std::sin(t1)*nr1;

            // Tri 1
            v.push_back(x00); v.push_back(y0); v.push_back(z00); v.push_back(uu0); v.push_back(vv0); v.push_back(nx00); v.push_back(ny0); v.push_back(nz00);
            v.push_back(x10); v.push_back(y1); v.push_back(z10); v.push_back(uu0); v.push_back(vv1); v.push_back(nx10); v.push_back(ny1); v.push_back(nz10);
            v.push_back(x11); v.push_back(y1); v.push_back(z11); v.push_back(uu1); v.push_back(vv1); v.push_back(nx11); v.push_back(ny1); v.push_back(nz11);
            // Tri 2
            v.push_back(x00); v.push_back(y0); v.push_back(z00); v.push_back(uu0); v.push_back(vv0); v.push_back(nx00); v.push_back(ny0); v.push_back(nz00);
            v.push_back(x11); v.push_back(y1); v.push_back(z11); v.push_back(uu1); v.push_back(vv1); v.push_back(nx11); v.push_back(ny1); v.push_back(nz11);
            v.push_back(x01); v.push_back(y0); v.push_back(z01); v.push_back(uu1); v.push_back(vv0); v.push_back(nx01); v.push_back(ny0); v.push_back(nz01);
        }
    }

    Mesh m;
    m.crearConUVNormal(v.data(), static_cast<int>(v.size()));
    return m;
}

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
