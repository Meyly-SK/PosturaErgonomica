#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "BodyPart.h"
#include "../graphics/Mesh.h"
#include "../graphics/Renderer.h"
#include "../graphics/Camera.h"
#include "../simulation/Types.h"

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

    // Aplica una postura: rota partes alrededor de sus pivotes usando GLM.
    void setScenario(const ScenarioData& escenario);

    // Colorea zonas según nivel de riesgo (verde/amarillo/rojo).
    void applyRisk(const RiskData& riesgo);

    // Configuración de debug
    void setModoDebug(ModoDebug modo);
    ModoDebug getModoDebug() const;

    // Dibuja con color sólido (modo original, sin textura)
    void dibujar(Renderer& renderer,
                 const Mesh& meshCubo,
                 const Mesh& meshCilindro,
                 const Mesh& meshEsfera,
                 const Camera& camara) const;

    // Dibuja con textura de madera mezclada con el color de riesgo.
    // Las partes con ZonaRiesgo asignada muestran tinte; las demás muestran solo madera.
    void dibujarConTextura(Renderer& renderer,
                           const Mesh& meshCuboUV,
                           const Mesh& meshCilindroUV,
                           const Mesh& meshEsferaUV,
                           const Camera& camara,
                           const Textura& textura) const;

    // Dibuja con iluminación Phong (textura + tinte de riesgo + luz difusa/especular).
    // Requiere meshes con normales (crearConUVNormal).
    void dibujarConPhong(Renderer& renderer,
                         const Mesh& meshCuboN,
                         const Mesh& meshCilindroN,
                         const Mesh& meshEsferaN,
                         const Camera& camara,
                         const Textura& textura,
                         const glm::vec3& posLuz) const;

private:
    std::vector<BodyPart> mPartes;
    ModoDebug mModoDebug = ModoDebug::Completo;

    // Helpers
    int agregarParte(const BodyPart& parte);
    bool debeDibujarse(const BodyPart& parte) const;
};
