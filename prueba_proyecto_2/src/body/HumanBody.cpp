#include "HumanBody.h"

#include <glm/glm.hpp>

// ---------------------------------------------------------------------------
// Helper interno
// ---------------------------------------------------------------------------
int HumanBody::agregarParte(const BodyPart& parte)
{
    mPartes.push_back(parte);
    return static_cast<int>(mPartes.size()) - 1;
}

// ---------------------------------------------------------------------------
// inicializar
// Construye el pseudo-cuerpo con COORDENADAS ABSOLUTAS (indicePadre = -1)
// para que las posiciones sean completamente predecibles sin pivotes complejos.
//
// Sistema de unidades: 1 unidad ≈ "módulo de cabeza"
// Altura total del cuerpo: ~7.5 unidades (convención artística clásica)
//   Y = 0   → suelo (planta del pie)
//   Y = 7.5 → tope de la cabeza
// Pivote del cuerpo: caderas ≈ Y 3.0
//
// Para posturas futuras: bastará con mover posicion + rotacion de cada parte.
// ---------------------------------------------------------------------------
void HumanBody::inicializar()
{
    mPartes.clear();

    // -----------------------------------------------------------------------
    // CABEZA  (esfera, radio ~0.6 unidades)
    // Centro de la cabeza en Y = 6.9
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "Cabeza";
        p.tipoMalla = BodyPart::TipoMalla::Esfera;
        p.indicePadre = -1;
        // Esfera crearEsfera(0.5, ...) → radio = 0.5 * escala.x
        p.escala    = {1.20f, 1.20f, 0.90f};   // diámetro ~1.2 unidades
        p.posicion  = {0.0f, 6.30f, 0.0f};     // centro de la esfera
        p.color     = {0.92f, 0.85f, 0.80f};   // piel
        agregarParte(p);
    }

    // -----------------------------------------------------------------------
    // CUELLO  (cilindro)
    // El cilindro crearCilindro(0.5, 1.0, 20) va de Y=-0.5 a Y=+0.5,
    // luego escalamos: alto real = 1.0 * escala.y, y lo posicionamos en su CENTRO.
    // Cuello: de Y=5.50 a Y=5.90  → centro Y=5.70, alto=0.40
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "Cuello";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.zona = ZonaRiesgo::Cuello;
        p.indicePadre = -1;
        p.escala   = {0.38f, 0.40f, 0.38f};    // radio ~0.19, alto ~0.40
        p.posicion = {0.0f, 5.70f, 0.0f};
        p.color    = {0.90f, 0.82f, 0.75f};
        agregarParte(p);
    }

    // -----------------------------------------------------------------------
    // TORSO SUPERIOR / PECHO
    // de Y=4.00 a Y=5.50 → centro Y=4.75, alto=1.50
    // ancho ≈ 1.20 (radio ~0.60)
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "Pecho";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.indicePadre = -1;
        p.escala   = {1.20f, 1.50f, 0.70f};
        p.posicion = {0.0f, 4.75f, 0.0f};
        p.color    = {0.55f, 0.70f, 0.95f};
        agregarParte(p);
    }

    // -----------------------------------------------------------------------
    // TORSO INFERIOR / ZONA LUMBAR
    // de Y=2.80 a Y=4.00 → centro Y=3.40, alto=1.20
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "ZonaLumbar";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.zona = ZonaRiesgo::Lumbar;
        p.indicePadre = -1;
        p.escala   = {1.10f, 1.20f, 0.65f};
        p.posicion = {0.0f, 3.40f, 0.0f};
        p.color    = {0.80f, 0.70f, 0.60f};
        agregarParte(p);
    }

    // -----------------------------------------------------------------------
    // CADERA (bloque ancho y corto que conecta torso con piernas)
    // de Y=2.40 a Y=2.80 → centro Y=2.60, alto=0.40
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "Cadera";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.indicePadre = -1;
        p.escala   = {1.05f, 0.40f, 0.60f};
        p.posicion = {0.0f, 2.60f, 0.0f};
        p.color    = {0.70f, 0.65f, 0.85f};
        agregarParte(p);
    }

    // -----------------------------------------------------------------------
    // BRAZOS SUPERIORES (UpperArm) - izquierdo y derecho
    // de Y=3.80 a Y=4.90 → centro Y=4.35, alto=1.10
    // X = ±0.90 (al lado del pecho)
    // Pequeña inclinación hacia afuera: rotZ = ±8°
    // -----------------------------------------------------------------------
    {
        // Izquierdo (X negativo)
        BodyPart p;
        p.nombre = "BrazoSupIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.zona = ZonaRiesgo::Hombros;
        p.indicePadre = -1;
        p.escala   = {0.34f, 1.10f, 0.34f};
        p.posicion = {-0.82f, 4.35f, 0.0f};
        p.rotacionEulerGrados = {0.0f, 0.0f, 8.0f};   // inclinación leve hacia afuera
        p.color    = {0.65f, 0.90f, 0.65f};
        agregarParte(p);

        // Derecho (X positivo)
        BodyPart pd = p;
        pd.nombre = "BrazoSupDer";
        pd.posicion.x = 0.82f;
        pd.rotacionEulerGrados.z = -8.0f;
        agregarParte(pd);
    }

    // -----------------------------------------------------------------------
    // ANTEBRAZO (Forearm) - izquierdo y derecho
    // de Y=2.80 a Y=3.80 → centro Y=3.30, alto=1.00
    // X = ±0.90 (misma columna que el brazo)
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "AntebrazoIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.indicePadre = -1;
        p.escala   = {0.30f, 1.00f, 0.30f};
        p.posicion = {-0.88f, 3.30f, 0.0f};
        p.rotacionEulerGrados = {0.0f, 0.0f, 5.0f};
        p.color    = {0.60f, 0.85f, 0.60f};
        agregarParte(p);

        BodyPart pd = p;
        pd.nombre = "AntebrazoDer";
        pd.posicion.x = 0.88f;
        pd.rotacionEulerGrados.z = -5.0f;
        agregarParte(pd);
    }

    // -----------------------------------------------------------------------
    // MANOS  (cubo aplanado)
    // centro Y=2.40, X=±0.92
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "ManoIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cubo;
        p.indicePadre = -1;
        p.escala   = {0.32f, 0.28f, 0.22f};
        p.posicion = {-0.94f, 2.40f, 0.0f};
        p.color    = {0.92f, 0.85f, 0.80f};
        agregarParte(p);

        BodyPart pd = p;
        pd.nombre = "ManoDer";
        pd.posicion.x = 0.94f;
        agregarParte(pd);
    }

    // -----------------------------------------------------------------------
    // MUSLOS (Thigh) - izquierdo y derecho
    // de Y=1.40 a Y=2.50 → centro Y=1.95, alto=1.10
    // X = ±0.30 (separados)
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "MusloIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.zona = ZonaRiesgo::Rodillas;
        p.indicePadre = -1;
        p.escala   = {0.40f, 1.10f, 0.40f};
        p.posicion = {-0.30f, 1.95f, 0.0f};
        p.color    = {0.90f, 0.70f, 0.70f};
        agregarParte(p);

        BodyPart pd = p;
        pd.nombre = "MusloDer";
        pd.posicion.x = 0.30f;
        agregarParte(pd);
    }

    // -----------------------------------------------------------------------
    // PANTORRILLAS (Shin) - izquierdo y derecho
    // de Y=0.40 a Y=1.40 → centro Y=0.90, alto=1.00
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "PiernaIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cilindro;
        p.zona = ZonaRiesgo::Rodillas;
        p.indicePadre = -1;
        p.escala   = {0.35f, 1.00f, 0.35f};
        p.posicion = {-0.28f, 0.90f, 0.0f};
        p.color    = {0.88f, 0.65f, 0.65f};
        agregarParte(p);

        BodyPart pd = p;
        pd.nombre = "PiernaDer";
        pd.posicion.x = 0.28f;
        agregarParte(pd);
    }

    // -----------------------------------------------------------------------
    // PIES  (cubo aplanado, ligeramente adelantado en Z)
    // centro Y=0.10, X=±0.28
    // -----------------------------------------------------------------------
    {
        BodyPart p;
        p.nombre = "PieIzq";
        p.tipoMalla = BodyPart::TipoMalla::Cubo;
        p.indicePadre = -1;
        p.escala   = {0.40f, 0.20f, 0.70f};
        p.posicion = {-0.28f, 0.10f, 0.15f};   // Z positivo = adelantado
        p.color    = {0.35f, 0.35f, 0.40f};
        agregarParte(p);

        BodyPart pd = p;
        pd.nombre = "PieDer";
        pd.posicion.x = 0.28f;
        agregarParte(pd);
    }

    // Calcular matrices mundo iniciales
    actualizarJerarquia();
}

// ---------------------------------------------------------------------------
// actualizarJerarquia
// ---------------------------------------------------------------------------
void HumanBody::actualizarJerarquia()
{
    for (size_t i = 0; i < mPartes.size(); i++)
    {
        BodyPart& p = mPartes[i];
        const glm::mat4 local = p.matrizLocal();

        if (p.indicePadre < 0)
        {
            p.matrizMundo = local;
        }
        else
        {
            p.matrizMundo = mPartes[p.indicePadre].matrizMundo * local;
        }
    }
}

// ---------------------------------------------------------------------------
// Modo debug
// ---------------------------------------------------------------------------
void HumanBody::setModoDebug(ModoDebug modo)
{
    mModoDebug = modo;
}

HumanBody::ModoDebug HumanBody::getModoDebug() const
{
    return mModoDebug;
}

bool HumanBody::debeDibujarse(const BodyPart& parte) const
{
    if (mModoDebug == ModoDebug::Completo) return true;

    const std::string& n = parte.nombre;

    switch (mModoDebug)
    {
    case ModoDebug::SoloCabezaCuello:
        return (n == "Cabeza" || n == "Cuello");

    case ModoDebug::SoloHombros:
        return (n == "BrazoSupIzq" || n == "BrazoSupDer");

    case ModoDebug::SoloBrazoIzq:
        return (n == "BrazoSupIzq" || n == "AntebrazoIzq" || n == "ManoIzq");

    case ModoDebug::SoloBrazoDer:
        return (n == "BrazoSupDer" || n == "AntebrazoDer" || n == "ManoDer");

    case ModoDebug::SoloPiernaIzq:
        return (n == "MusloIzq" || n == "PiernaIzq" || n == "PieIzq");

    case ModoDebug::SoloPiernaDer:
        return (n == "MusloDer" || n == "PiernaDer" || n == "PieDer");

    case ModoDebug::Completo:
    default:
        return true;
    }
}

// ---------------------------------------------------------------------------
// dibujar
// ---------------------------------------------------------------------------
void HumanBody::dibujar(Renderer& renderer,
                        const Mesh& meshCubo,
                        const Mesh& meshCilindro,
                        const Mesh& meshEsfera,
                        const Camera& camara) const
{
    for (const BodyPart& p : mPartes)
    {
        if (!debeDibujarse(p))
            continue;

        const Mesh* mesh = &meshCubo;
        switch (p.tipoMalla)
        {
        case BodyPart::TipoMalla::Cilindro:
            mesh = &meshCilindro;
            break;
        case BodyPart::TipoMalla::Esfera:
            mesh = &meshEsfera;
            break;
        case BodyPart::TipoMalla::Cubo:
        default:
            mesh = &meshCubo;
            break;
        }

        renderer.dibujar(*mesh, p.matrizMundo, p.color, camara);
    }
}
