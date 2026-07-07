#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "BodyPart.h"
#include "../graphics/Mesh.h"
#include "../graphics/Renderer.h"
#include "../graphics/Camera.h"

// Cuerpo humano simplificado (pseudo-cuerpo) hecho por partes.
// Se construye como una jerarquía de BodyPart usando parentIndex.
class HumanBody
{
public:
    // Modo de depuración: permite dibujar solo un conjunto de partes para ajustar proporciones.
    enum class ModoDebug
    {
        Completo = 0,
        SoloCabezaCuello,
        SoloHombros,
        SoloBrazoIzq,
        SoloBrazoDer,
        SoloPiernaIzq,
        SoloPiernaDer
    };

    // Crea las partes base (torso, cabeza, brazos, piernas).
    void inicializar();

    // Recalcula matrices mundo en base a la jerarquía.
    void actualizarJerarquia();

    // Configuración de debug
    void setModoDebug(ModoDebug modo);
    ModoDebug getModoDebug() const;

    // Dibuja cada parte eligiendo la malla según el TipoMalla del BodyPart.
    void dibujar(Renderer& renderer,
                 const Mesh& meshCubo,
                 const Mesh& meshCilindro,
                 const Mesh& meshEsfera,
                 const Camera& camara) const;

private:
    std::vector<BodyPart> mPartes;
    ModoDebug mModoDebug = ModoDebug::Completo;

    // Helpers
    int agregarParte(const BodyPart& parte);
    bool debeDibujarse(const BodyPart& parte) const;
};
