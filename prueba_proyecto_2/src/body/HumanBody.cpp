#include "HumanBody.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
//
// Arquitectura de jerarquía real con pivotes neutros (Opción B):
//
//   Partes del torso y cabeza: absolutas (indicePadre = -1), sin cambio.
//
//   Brazos y piernas: jerarquía real con PIVOTES DE ESCALA NEUTRA {1,1,1}:
//     PivoteHombro (escala {1,1,1}, posicion absoluta del hombro)
//       └─ BrazoSup (indicePadre = iPivoteHombro, offsetVisual = {0,-0.55,0})
//            └─ PivoteCodo (escala {1,1,1}, pos relativa = extremo inferior del brazo)
//                 └─ Antebrazo (offsetVisual = {0,-0.50,0})
//                      └─ PivoteMuñeca
//                           └─ Mano
//
//   El nodo pivote tiene escala {1,1,1} → los hijos NO heredan escala distorsionada.
//   La malla visual usa offsetVisual para posicionarse correctamente.
//
//   Ventaja: setScenario() solo cambia rotacionEulerGrados del PIVOTE.
//            Los hijos siguen automáticamente por actualizarJerarquia().
// ---------------------------------------------------------------------------
void HumanBody::inicializar()
{
    mPartes.clear();

    // ---- Helper: crea parte visible con offset visual ----
    // offsetV = desplazamiento de la malla desde el pivote (en espacio local rotado)
    // Para cilindro centrado: offsetV.y = -escala.y/2 hace que cuelgue desde el pivote.

    // ---- CABEZA ----
    { BodyPart p; p.nombre="Cabeza"; p.tipoMalla=BodyPart::TipoMalla::Esfera;
      p.indicePadre=-1; p.escala={1.20f,1.20f,0.90f}; p.posicion={0.0f,6.30f,0.0f};
      p.color={0.92f,0.85f,0.80f}; agregarParte(p); }

    // ---- CUELLO ----
    { BodyPart p; p.nombre="Cuello"; p.tipoMalla=BodyPart::TipoMalla::Cilindro;
      p.zona=ZonaRiesgo::Cuello; p.indicePadre=-1;
      p.escala={0.38f,0.40f,0.38f}; p.posicion={0.0f,5.70f,0.0f};
      p.color={0.90f,0.82f,0.75f}; agregarParte(p); }

    // ---- PECHO ----
    { BodyPart p; p.nombre="Pecho"; p.tipoMalla=BodyPart::TipoMalla::Cilindro;
      p.indicePadre=-1; p.escala={1.20f,1.50f,0.70f}; p.posicion={0.0f,4.75f,0.0f};
      p.color={0.55f,0.70f,0.95f}; agregarParte(p); }

    // ---- ZONA LUMBAR ----
    { BodyPart p; p.nombre="ZonaLumbar"; p.tipoMalla=BodyPart::TipoMalla::Cilindro;
      p.zona=ZonaRiesgo::Lumbar; p.indicePadre=-1;
      p.escala={1.10f,1.20f,0.65f}; p.posicion={0.0f,3.40f,0.0f};
      p.color={0.80f,0.70f,0.60f}; agregarParte(p); }

    // ---- CADERA ----
    { BodyPart p; p.nombre="Cadera"; p.tipoMalla=BodyPart::TipoMalla::Cilindro;
      p.indicePadre=-1; p.escala={1.05f,0.40f,0.60f}; p.posicion={0.0f,2.60f,0.0f};
      p.color={0.70f,0.65f,0.85f}; agregarParte(p); }

    // ====================================================================
    // BRAZOS con jerarquía real + pivotes de escala neutra
    //
    // Estructura:
    //   PivHombro (escala {1,1,1}) ← nodo de rotación, NO hereda escala
    //     └─ BrazoSup  (offsetVisual {0,-0.55,0} ← cuelga desde hombro)
    //          └─ PivCodo (escala {1,1,1}, pos relativa {0,-1.10,0})
    //               └─ Antebrazo (offsetVisual {0,-0.50,0})
    //                    └─ PivMuñeca (escala {1,1,1}, pos {0,-1.00,0})
    //                         └─ Mano (offsetVisual {0,-0.14,0})
    // ====================================================================
    auto crearBrazo = [&](float xSign)
    {
        const std::string lado = (xSign < 0) ? "Izq" : "Der";
        const float xH = xSign * 0.82f; // X del hombro

        // Pivote hombro: escala {1,1,1} → hijos NO heredan distorsión
        // No se dibuja porque debeDibujarse() filtra nombres que empiezan con "Piv"
        BodyPart pivH;
        pivH.nombre = "PivHombro" + lado;
        pivH.tipoMalla = BodyPart::TipoMalla::Esfera;
        pivH.indicePadre = -1;
        pivH.escala = {1.0f, 1.0f, 1.0f}; // NEUTRO: hijos heredan escala 1 (sin distorsión)
        pivH.posicion = {xH, 4.90f, 0.0f}; // exactamente en la articulación del hombro
        pivH.zona = ZonaRiesgo::Hombros;
        pivH.color = {0.65f, 0.90f, 0.65f};
        const int iPivH = agregarParte(pivH);

        // Brazo superior: cuelga desde el pivote del hombro
        BodyPart bs;
        bs.nombre = "BrazoSup" + lado;
        bs.tipoMalla = BodyPart::TipoMalla::Cilindro;
        bs.zona = ZonaRiesgo::Hombros;
        bs.indicePadre = iPivH;
        bs.escala = {0.34f, 1.10f, 0.34f};
        bs.posicion = {0.0f, 0.0f, 0.0f}; // relativo al pivote
        bs.offsetVisual = {0.0f, -0.55f, 0.0f}; // la malla cuelga 0.55 hacia abajo
        bs.rotacionEulerGrados = {0.0f, 0.0f, xSign * -8.0f}; // leve inclinación
        bs.color = {0.65f, 0.90f, 0.65f};
        agregarParte(bs);

        // Pivote codo
        BodyPart pivC;
        pivC.nombre = "PivCodo" + lado;
        pivC.tipoMalla = BodyPart::TipoMalla::Esfera;
        pivC.indicePadre = iPivH;
        pivC.escala = {1.0f, 1.0f, 1.0f}; // neutro
        pivC.posicion = {0.0f, -1.10f, 0.0f}; // relativo al hombro = exactamente en el codo
        pivC.color = {0.60f, 0.85f, 0.60f};
        const int iPivC = agregarParte(pivC);

        // Antebrazo: cuelga desde el pivote del codo
        BodyPart ab;
        ab.nombre = "Antebrazo" + lado;
        ab.tipoMalla = BodyPart::TipoMalla::Cilindro;
        ab.indicePadre = iPivC;
        ab.escala = {0.30f, 1.00f, 0.30f};
        ab.posicion = {0.0f, 0.0f, 0.0f};
        ab.offsetVisual = {0.0f, -0.50f, 0.0f};
        ab.rotacionEulerGrados = {0.0f, 0.0f, xSign * -3.0f};
        ab.color = {0.60f, 0.85f, 0.60f};
        agregarParte(ab);

        // Pivote muñeca
        BodyPart pivM;
        pivM.nombre = "PivMuneca" + lado;
        pivM.tipoMalla = BodyPart::TipoMalla::Cubo;
        pivM.indicePadre = iPivC;
        pivM.escala = {1.0f, 1.0f, 1.0f}; // neutro
        pivM.posicion = {0.0f, -1.00f, 0.0f};
        pivM.color = {0.92f, 0.85f, 0.80f};
        const int iPivM = agregarParte(pivM);

        // Mano
        BodyPart mn;
        mn.nombre = "Mano" + lado;
        mn.tipoMalla = BodyPart::TipoMalla::Cubo;
        mn.indicePadre = iPivM;
        mn.escala = {0.32f, 0.28f, 0.22f};
        mn.posicion = {0.0f, 0.0f, 0.0f};
        mn.offsetVisual = {0.0f, -0.14f, 0.0f};
        mn.color = {0.92f, 0.85f, 0.80f};
        agregarParte(mn);
    };

    crearBrazo(-1.0f); // izquierdo
    crearBrazo( 1.0f); // derecho

    // ====================================================================
    // PIERNAS con jerarquía real
    //   PivCadera → Muslo → PivRodilla → Pierna → PivTobillo → Pie
    // ====================================================================
    auto crearPierna = [&](float xSign)
    {
        const std::string lado = (xSign < 0) ? "Izq" : "Der";
        const float xC = xSign * 0.30f;

        // Pivote cadera: separación aumentada a ±0.38 para postura más natural
        BodyPart pivCad;
        pivCad.nombre = "PivCadera" + lado;
        pivCad.tipoMalla = BodyPart::TipoMalla::Cilindro;
        pivCad.zona = ZonaRiesgo::Rodillas;
        pivCad.indicePadre = -1;
        pivCad.escala = {1.0f, 1.0f, 1.0f}; // neutro
        const float xCad = xSign * 0.38f; // separación aumentada (antes 0.30)
        pivCad.posicion = {xCad, 2.50f, 0.0f};
        pivCad.color = {0.90f, 0.70f, 0.70f};
        const int iPivCad = agregarParte(pivCad);

        // Muslo
        BodyPart ms;
        ms.nombre = "Muslo" + lado;
        ms.tipoMalla = BodyPart::TipoMalla::Cilindro;
        ms.zona = ZonaRiesgo::Rodillas;
        ms.indicePadre = iPivCad;
        ms.escala = {0.40f, 1.10f, 0.40f};
        ms.posicion = {0.0f, 0.0f, 0.0f};
        ms.offsetVisual = {0.0f, -0.55f, 0.0f};
        ms.color = {0.90f, 0.70f, 0.70f};
        agregarParte(ms);

        // Pivote rodilla
        BodyPart pivRod;
        pivRod.nombre = "PivRodilla" + lado;
        pivRod.tipoMalla = BodyPart::TipoMalla::Cilindro;
        pivRod.zona = ZonaRiesgo::Rodillas;
        pivRod.indicePadre = iPivCad;
        pivRod.escala = {1.0f, 1.0f, 1.0f}; // neutro
        pivRod.posicion = {0.0f, -1.10f, 0.0f};
        pivRod.color = {0.88f, 0.65f, 0.65f};
        const int iPivRod = agregarParte(pivRod);

        // Pantorrilla
        BodyPart pr;
        pr.nombre = "Pierna" + lado;
        pr.tipoMalla = BodyPart::TipoMalla::Cilindro;
        pr.zona = ZonaRiesgo::Rodillas;
        pr.indicePadre = iPivRod;
        pr.escala = {0.35f, 1.00f, 0.35f};
        pr.posicion = {0.0f, 0.0f, 0.0f};
        pr.offsetVisual = {0.0f, -0.50f, 0.0f};
        pr.color = {0.88f, 0.65f, 0.65f};
        agregarParte(pr);

        // Pivote tobillo
        BodyPart pivTob;
        pivTob.nombre = "PivTobillo" + lado;
        pivTob.tipoMalla = BodyPart::TipoMalla::Cubo;
        pivTob.indicePadre = iPivRod;
        pivTob.escala = {1.0f, 1.0f, 1.0f}; // neutro
        pivTob.posicion = {0.0f, -1.00f, 0.0f};
        pivTob.color = {0.35f, 0.35f, 0.40f};
        const int iPivTob = agregarParte(pivTob);

        // Pie: ligeramente separado hacia afuera (X) para naturalidad visual
        BodyPart pi;
        pi.nombre = "Pie" + lado;
        pi.tipoMalla = BodyPart::TipoMalla::Cubo;
        pi.indicePadre = iPivTob;
        pi.escala = {0.40f, 0.20f, 0.70f};
        pi.posicion = {0.0f, 0.0f, 0.0f};
        pi.offsetVisual = {xSign * 0.06f, -0.10f, 0.15f}; // pie separado hacia afuera
        pi.color = {0.35f, 0.35f, 0.40f};
        agregarParte(pi);
    };

    crearPierna(-1.0f); // izquierda
    crearPierna( 1.0f); // derecha

    // Guardar pose base
    for (BodyPart& p : mPartes)
    {
        p.posicionBase  = p.posicion;
        p.rotacionBase  = p.rotacionEulerGrados;
        p.colorBase     = p.color;
    }
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
// setScenario
//
// Para rotar un segmento del cuerpo desde su articulación (sin jerarquía padre/hijo),
// usamos la fórmula:
//   posNueva = pivot + Rot(angulo) * (posBase - pivot)
//   rotNueva = rotBase + angulo (solo en el eje correspondiente)
//
// El pivot de cada grupo de partes:
//   Brazo derecho: tope del BrazoSupDer = posBase.y + escala.y/2
//   Brazo izquierdo: idem lado izquierdo
// ---------------------------------------------------------------------------
void HumanBody::setScenario(const ScenarioData& escenario)
{
    // Restaurar pose base primero
    for (BodyPart& p : mPartes)
    {
        p.posicion            = p.posicionBase;
        p.rotacionEulerGrados = p.rotacionBase;
    }

    // Helper lambda: rota grupo de partes alrededor de un pivot en el eje dado
    auto rotarGrupoEje = [&](
        const std::vector<std::string>& nombres,
        const glm::vec3& pivot,
        float angGrados,
        const glm::vec3& eje,
        bool actualizarRotZ = false,
        float deltaZ = 0.0f)
    {
        if (angGrados == 0.0f) return;
        const glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angGrados), eje);
        for (BodyPart& p : mPartes)
        {
            for (const auto& nm : nombres)
            {
                if (p.nombre == nm)
                {
                    const glm::vec4 off = {
                        p.posicion.x - pivot.x,
                        p.posicion.y - pivot.y,
                        p.posicion.z - pivot.z, 0.0f
                    };
                    const glm::vec4 r = rot * off;
                    p.posicion.x = pivot.x + r.x;
                    p.posicion.y = pivot.y + r.y;
                    p.posicion.z = pivot.z + r.z;
                    if (actualizarRotZ && p.nombre == nm)
                        p.rotacionEulerGrados.z = p.rotacionBase.z + deltaZ;
                    break;
                }
            }
        }
    };

    // ---- TORSO PRIMERO (mueve torso + pivotes de hombro + pivotes de cadera) ----
    // Con jerarquía real, mover el torso = mover los pivotes de hombro (absolutos).
    // Los brazos siguen automáticamente por herencia de jerarquía.
    if (escenario.anguloTorso != 0.0f)
    {
        const glm::vec3 pivotTorso = {0.0f, 2.80f, 0.0f};
        const float angT = glm::radians(escenario.anguloTorso);
        const glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angT, glm::vec3(1.0f, 0.0f, 0.0f));
        // Mueve las partes absolutas del torso + los pivotes de hombro
        const std::vector<std::string> partesTorso = {
            "Pecho","ZonaLumbar","Cuello","Cabeza",
            "PivHombroIzq","PivHombroDer"   // pivotes de hombro se mueven con el torso
        };
        for (BodyPart& p : mPartes)
        {
            for (const auto& nombre : partesTorso)
            {
                if (p.nombre == nombre)
                {
                    const glm::vec4 off = {
                        p.posicion.x - pivotTorso.x,
                        p.posicion.y - pivotTorso.y,
                        p.posicion.z - pivotTorso.z, 0.0f
                    };
                    const glm::vec4 r2 = rot * off;
                    p.posicion.x = pivotTorso.x + r2.x;
                    p.posicion.y = pivotTorso.y + r2.y;
                    p.posicion.z = pivotTorso.z + r2.z;
                    p.rotacionEulerGrados.x = p.rotacionBase.x + escenario.anguloTorso;
                    break;
                }
            }
        }
    }

    // ---- Hombro derecho: rota el PIVOTE → brazo/antebrazo/mano siguen automáticamente ----
    // + = arriba (eje Z), - = abajo
    // X negado: + = adelante del muñeco
    {
        auto setRot = [&](const std::string& nm, const glm::vec3& r) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm) { p.rotacionEulerGrados = p.rotacionBase + r; break; }
        };
        setRot("PivHombroDer", {0.0f,  0.0f,  escenario.anguloBrazoDer});
        // Eje X sagital: negar para que + = adelante
        // Se aplica como rotX adicional en el pivote (primero Z, luego X)
        if (escenario.anguloBrazoDerX != 0.0f)
        {
            for (BodyPart& p : mPartes)
                if (p.nombre == "PivHombroDer")
                    { p.rotacionEulerGrados.x = p.rotacionBase.x - escenario.anguloBrazoDerX; break; }
        }

        setRot("PivHombroIzq", {0.0f, 0.0f, -escenario.anguloBrazoIzq});
        if (escenario.anguloBrazoIzqX != 0.0f)
        {
            for (BodyPart& p : mPartes)
                if (p.nombre == "PivHombroIzq")
                    { p.rotacionEulerGrados.x = p.rotacionBase.x - escenario.anguloBrazoIzqX; break; }
        }
    }

    // ---- Codo: rota el PivCodo relativo al hombro ----
    {
        auto setRotCodo = [&](const std::string& nm, float z, float x) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm)
                    { p.rotacionEulerGrados = {p.rotacionBase.x + x, p.rotacionBase.y, p.rotacionBase.z + z}; break; }
        };
        setRotCodo("PivCodoDer",  escenario.anguloCodoDer,  -escenario.anguloCodoDerX);
        setRotCodo("PivCodoIzq", -escenario.anguloCodoIzq,  -escenario.anguloCodoIzqX);
    }

    // ---- Cadera: rota PivCadera → muslo/pierna/pie siguen ----
    {
        auto setRotCad = [&](const std::string& nm, float z, float x) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm)
                    { p.rotacionEulerGrados = {p.rotacionBase.x + x, p.rotacionBase.y, p.rotacionBase.z + z}; break; }
        };
        setRotCad("PivCaderaDer", -escenario.anguloMusloDer, -escenario.anguloMusloDerX);
        setRotCad("PivCaderaIzq",  escenario.anguloMusloIzq, -escenario.anguloMusloIzqX);
    }

    // ---- Rodilla: rota PivRodilla → pierna/pie siguen ----
    {
        auto setRotRod = [&](const std::string& nm, float z, float x) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm)
                    { p.rotacionEulerGrados = {p.rotacionBase.x + x, p.rotacionBase.y, p.rotacionBase.z + z}; break; }
        };
        setRotRod("PivRodillaDer", -escenario.anguloRodilla, -escenario.anguloRodillaX);
        setRotRod("PivRodillaIzq",  escenario.anguloRodilla, -escenario.anguloRodillaX);
    }

    // ---- Tobillo / Pie: rotación independiente del pie (sin mover la pantorrilla) ----
    // PivTobillo ya existe en la jerarquía: PivRodilla → Pierna → PivTobillo → Pie
    // Rotar PivTobillo gira solo el pie, sin afectar la pantorrilla.
    // Eje Y = rotación axial (dedo del pie apunta a izq/der)
    // Eje X = dorsiflexión (punta arriba/abajo)
    {
        auto setRotTob = [&](const std::string& nm, float x, float y) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm)
                    { p.rotacionEulerGrados = {p.rotacionBase.x + x, p.rotacionBase.y + y, p.rotacionBase.z}; break; }
        };
        setRotTob("PivTobilloDer", escenario.anguloPieDerX,  escenario.anguloPieDerY);
        setRotTob("PivTobilloIzq", escenario.anguloPieIzqX,  escenario.anguloPieIzqY);
    }

    // ---- Muñeca / Mano: rotación independiente de la mano (sin mover el antebrazo) ----
    // PivMuneca ya existe: PivCodo → Antebrazo → PivMuneca → Mano
    // Eje Y = pronación/supinación (palma arriba/abajo)
    // Eje X = flexión de muñeca
    // Eje Z = desviación cubital/radial
    {
        auto setRotMun = [&](const std::string& nm, float x, float y, float z) {
            for (BodyPart& p : mPartes)
                if (p.nombre == nm)
                    { p.rotacionEulerGrados = {p.rotacionBase.x + x, p.rotacionBase.y + y, p.rotacionBase.z + z}; break; }
        };
        setRotMun("PivMunecaDer", escenario.anguloManoDerX, escenario.anguloManoDerY, escenario.anguloManoDerZ);
        setRotMun("PivMunecaIzq", escenario.anguloManoIzqX, escenario.anguloManoIzqY, escenario.anguloManoIzqZ);
    }

    // Recalcular jerarquía completa
    actualizarJerarquia();
}

// ---------------------------------------------------------------------------
// applyRisk — colorea partes según el nivel de riesgo por zona
// ---------------------------------------------------------------------------
void HumanBody::applyRisk(const RiskData& riesgo)
{
    auto colorRiesgo = [](float r) -> glm::vec3 {
        if (r < 30.0f) return {0.20f, 0.85f, 0.20f};
        if (r < 60.0f) return {1.00f, 0.85f, 0.10f};
        return               {0.90f, 0.15f, 0.15f};
    };

    for (BodyPart& p : mPartes)
    {
        switch (p.zona)
        {
        case ZonaRiesgo::Cuello:   p.color = colorRiesgo(riesgo.riesgoCuello);   break;
        case ZonaRiesgo::Lumbar:   p.color = colorRiesgo(riesgo.riesgoLumbar);   break;
        case ZonaRiesgo::Hombros:  p.color = colorRiesgo(riesgo.riesgoHombros);  break;
        case ZonaRiesgo::Rodillas: p.color = colorRiesgo(riesgo.riesgoRodillas); break;
        case ZonaRiesgo::Ninguna:
        default: break;
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
    // Los nodos pivote (nombres que empiezan con "Piv") NUNCA se dibujan.
    // Tienen escala {1,1,1} para no distorsionar hijos, pero no son mallas visibles.
    if (parte.nombre.substr(0, 3) == "Piv") return false;

    if (mModoDebug == ModoDebug::Completo) return true;

    const std::string& n = parte.nombre;

    switch (mModoDebug)
    {
    case ModoDebug::SoloCabezaCuello:
        return (n == "Cabeza" || n == "Cuello");

    case ModoDebug::SoloHombros:
        return (n == "BrazoSupIzq" || n == "BrazoSupDer"
             || n == "AntebrazoIzq" || n == "AntebrazoDer"
             || n == "ManoIzq" || n == "ManoDer");

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
// dibujarConPhong — igual que dibujarConTextura pero usa shader Phong
// ---------------------------------------------------------------------------
void HumanBody::dibujarConPhong(Renderer& renderer,
                                const Mesh& meshCuboN,
                                const Mesh& meshCilindroN,
                                const Mesh& meshEsferaN,
                                const Camera& camara,
                                const Textura& textura,
                                const glm::vec3& posLuz) const
{
    for (const BodyPart& p : mPartes)
    {
        if (!debeDibujarse(p)) continue;
        const Mesh* mesh = &meshCuboN;
        switch (p.tipoMalla)
        {
        case BodyPart::TipoMalla::Cilindro: mesh = &meshCilindroN; break;
        case BodyPart::TipoMalla::Esfera:   mesh = &meshEsferaN;   break;
        default:                             mesh = &meshCuboN;     break;
        }
        const float mezcla = (p.zona == ZonaRiesgo::Ninguna) ? 0.05f : 0.55f;
        renderer.dibujarConPhong(*mesh, p.matrizMundo, p.color, camara, textura, mezcla, posLuz);
    }
}

// ---------------------------------------------------------------------------
// dibujarConTextura
//
// Lógica de mezcla:
//   - Partes con ZonaRiesgo::Ninguna (pecho, cadera, etc.) → mezcla 0.0 (solo madera)
//   - Partes con zona asignada → mezcla 0.55 (textura tintada con color de riesgo)
// Esto hace que las zonas seguras se vean como madera pura y las de riesgo
// muestren el tinte verde/amarillo/rojo sobre la textura.
// ---------------------------------------------------------------------------
void HumanBody::dibujarConTextura(Renderer& renderer,
                                  const Mesh& meshCuboUV,
                                  const Mesh& meshCilindroUV,
                                  const Mesh& meshEsferaUV,
                                  const Camera& camara,
                                  const Textura& textura) const
{
    for (const BodyPart& p : mPartes)
    {
        if (!debeDibujarse(p)) continue;

        const Mesh* mesh = &meshCuboUV;
        switch (p.tipoMalla)
        {
        case BodyPart::TipoMalla::Cilindro: mesh = &meshCilindroUV; break;
        case BodyPart::TipoMalla::Esfera:   mesh = &meshEsferaUV;   break;
        default:                             mesh = &meshCuboUV;     break;
        }

        // Calcular mezcla: zona sin riesgo = solo textura, con riesgo = tintada
        const float mezcla = (p.zona == ZonaRiesgo::Ninguna) ? 0.05f : 0.55f;

        renderer.dibujarConTextura(*mesh, p.matrizMundo, p.color, camara, textura, mezcla);
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
