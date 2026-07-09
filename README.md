# ErgoSim 3D — Guía técnica (referencia viva)

Este `README.md` consolida **el análisis técnico del plan modular**, las **correcciones recomendadas**, los **pasos/hitos**, y las **reglas de organización** para mantener el proyecto ordenado y extensible.

> Objetivo: que cada vez que hablemos del proyecto, tengamos un marco común (arquitectura, contratos, límites de módulos y criterios de avance).

---

## 1) Resumen del proyecto

**ErgoSim 3D** es un **simulador educativo/demostrativo** de postura ergonómica y fuerza física (no simulación médica exacta).

- Enfoque inicial: **escenarios predefinidos** con valores controlados (ángulos, peso, distancia, tiempo, riesgo).
- Stack técnico base: **C++ + OpenGL + GLFW + GLEW + GLM**
- Opcionales: `stb` (texturas), `ImGui` (UI rápida)
- Assimp: **fuera del MVP** (posible investigación futura)

---

## 2) Alcance del MVP (lo mínimo que debe hacer)

El MVP debe:

1. Compilar y abrir una ventana OpenGL.
2. Mostrar un **pseudo-cuerpo humano 3D** simplificado (primitivas/mallas básicas).
3. Aplicar **posturas predefinidas** (al menos 2–3 escenarios).
4. Colorear zonas del cuerpo según **nivel de riesgo** (verde/amarillo/rojo).
5. Mostrar información básica del escenario (en UI o texto).
6. Permitir cambiar escenario con teclado (mínimo) o UI (opcional).

**Variables candidatas por escenario:**
- `neckAngle`, `torsoAngle`, `leftArmAngle`, `rightArmAngle`, `kneeAngle`
- `objectWeight`, `objectDistance`, `exposureTime`

---

## 3) Principios de organización (reglas no negociables)

### 3.1 Separación de responsabilidades
- El analizador **no dibuja**.
- El renderer **no calcula** riesgo.
- El cuerpo **no lee input**.
- La UI **no contiene** reglas de riesgo ni drawcalls low-level.

### 3.2 Contratos de datos (comunicación entre módulos)
La comunicación debe hacerse con estructuras simples y estables:
- `ScenarioData`
- `RiskData`
- (opcional) `TransformData` / `RenderItem`

### 3.3 Integración sin romper
- El proyecto base **debe compilar siempre**.
- Integrar por etapas: primero local/branch, luego merge cuando compila y muestra la demo estable.

---

## 4) Estructura de carpetas recomendada (objetivo)

```
ErgoSim3D/
|-- vcpkg.json
|-- README.md
|-- docs/
|-- src/
|   |-- main.cpp
|   |-- core/
|   |   |-- App.h / App.cpp
|   |   |-- Input.h / Input.cpp
|   |-- graphics/
|   |   |-- Shader.h / Shader.cpp
|   |   |-- Camera.h / Camera.cpp
|   |   |-- Mesh.h / Mesh.cpp
|   |   |-- PrimitiveFactory.h / PrimitiveFactory.cpp
|   |   |-- Renderer.h / Renderer.cpp
|   |-- simulation/
|   |   |-- Types.h                <-- recomendado (ScenarioData/RiskData/RiskZone)
|   |   |-- ScenarioManager.h/.cpp
|   |   |-- RiskAnalyzer.h/.cpp
|   |-- body/
|   |   |-- BodyPart.h / BodyPart.cpp
|   |   |-- HumanBody.h / HumanBody.cpp
|   |-- ui/
|       |-- UI.h / UI.cpp
|-- assets/
    |-- shaders/
    |-- textures/
    |-- data/
```

> Nota: en el repo actual hay una estructura distinta (Visual Studio project). Esta guía describe el **target modular**; se puede migrar gradualmente.

---

## 5) Módulos y responsabilidades (división por integrante)

### Integrante A — Base OpenGL / Render
- Ventana y loop
- Shaders
- Cámara
- Mesh/primitivas
- Renderer (API limpia)

### Integrante B — Cuerpo humano modular
- `HumanBody`, `BodyPart`
- Jerarquía (torso-cuello-cabeza, brazo-antebrazo-mano, etc.)
- Zonas de riesgo por parte

### Integrante C — Escenarios y posturas
- `ScenarioData`
- `ScenarioManager` (posturas predefinidas + transiciones)
- Controles para cambiar escenario

### Integrante D — Riesgo y UI
- `RiskAnalyzer` (heurístico o tabla)
- Mapeo a colores y recomendaciones
- UI (mínimo: texto/ImGui)

---

## 6) Contratos (estructuras compartidas)

### 6.1 ScenarioData / RiskData / RiskZone
Recomendación: colocarlas en `simulation/Types.h` (o `shared/Types.h`).

```cpp
struct ScenarioData {
    std::string name;
    std::string description;

    float neckAngle;
    float torsoAngle;
    float leftArmAngle;
    float rightArmAngle;
    float kneeAngle;

    float objectWeight;
    float objectDistance;
    float exposureTime;
};

struct RiskData {
    float neckRisk;
    float lumbarRisk;
    float shoulderRisk;
    float kneeRisk;
};

enum class RiskZone {
    Neck,
    Lumbar,
    Shoulders,
    Knees,
    None
};
```

### 6.2 Corrección clave: agregar jerarquía real en BodyPart
El boceto original con `std::vector<BodyPart> parts` **no modela jerarquía** por sí solo.

**Recomendación mínima (robusta y fácil de integrar):**
- `parentIndex` para definir árbol
- `localTransform` y `worldTransform` para cálculo

Ejemplo conceptual:

```cpp
struct BodyPart {
    std::string name;
    RiskZone zone = RiskZone::None;

    int parentIndex = -1;         // -1 = root
    glm::vec3 position{0};
    glm::vec3 rotationEuler{0};   // ver sección de convención
    glm::vec3 scale{1};
    glm::vec3 color{1};

    glm::mat4 localMatrix() const;
    glm::mat4 worldMatrix = glm::mat4(1.0f); // calculado por HumanBody
};
```

Luego `HumanBody` hace un recorrido y calcula:

```
world(child) = world(parent) * local(child)
```

Esto asegura que:
- cabeza sigue al cuello
- antebrazo sigue al brazo
- mano sigue al antebrazo, etc.

---

## 7) Convención de rotaciones (evitar ambigüedad)

Si se usan Euler (`glm::vec3`), definir desde el inicio:

- **Unidades**: preferible **grados** en `ScenarioData` (más amigable) y convertir a radianes internamente.
- **Ejes** (ejemplo típico):
  - X: flexión/extensión
  - Y: rotación axial
  - Z: inclinación lateral

Además:
- Definir rangos/clamps por articulación.
- A mediano plazo: considerar `glm::quat` internamente para evitar gimbal lock.

---

## 8) API Renderer ↔ Body (corrección para evitar OpenGL en HumanBody)

Objetivo: HumanBody produce “qué dibujar” y Renderer hace drawcalls.

Dos enfoques válidos:

### Opción A: RenderItem
```cpp
struct RenderItem {
    Mesh* mesh;
    glm::mat4 model;
    glm::vec3 color;
};
```

`HumanBody::gatherRenderItems(std::vector<RenderItem>& out)`

`Renderer::draw(const std::vector<RenderItem>& items)`

### Opción B: drawMesh parametrizado
`Renderer::drawMesh(const Mesh&, const glm::mat4& model, const glm::vec3& color)`

---

## 9) Flujo del programa (loop ordenado)

```
main.cpp
  -> App::run()
      -> processInput()
      -> ScenarioManager::getCurrentScenario()
      -> RiskAnalyzer::analyze(scenario)
      -> HumanBody::setScenario(scenario)
      -> HumanBody::applyRisk(risk)
      -> Renderer::draw(humanBody)
      -> UI::draw(scenario, risk)
```

**Recomendación de organización interna:**
- `update(dt)`:
  - input
  - scenario/risk
  - transforms
- `render()`:
  - draw scene
  - UI

---

## 10) Diseño del pseudo-cuerpo (árbol recomendado)

```
HumanBody
|-- Torso (root)
|   |-- Chest
|   |-- LumbarZone
|   |-- Neck
|       |-- Head
|-- LeftArm
|   |-- LeftUpperArm
|   |-- LeftForearm
|   |-- LeftHand
|-- RightArm
|   |-- RightUpperArm
|   |-- RightForearm
|   |-- RightHand
|-- LeftLeg
|   |-- LeftThigh
|   |-- LeftShin
|   |-- LeftFoot
|-- RightLeg
    |-- RightThigh
    |-- RightShin
    |-- RightFoot
```

Para MVP, se puede empezar con:
- Torso + Head + 2 brazos + 2 piernas (sin manos/pies)
y luego subdividir.

---

## 11) Modelo simple de riesgo (heurístico)

El riesgo demostrativo debe ser:
- determinista
- estable (0..100)
- fácil de ajustar

Ejemplo base (lumbar):

```cpp
float calculateLumbarRisk(float torsoAngle, float weight, float distance, float time) {
    float risk = 0.0f;
    risk += std::abs(torsoAngle) * 1.1f;
    risk += weight * 2.0f;
    risk += distance * 15.0f;
    risk += time * 0.3f;
    return glm::clamp(risk, 0.0f, 100.0f);
}
```

Mapeo a color:

```cpp
glm::vec3 getRiskColor(float risk) {
    if (risk < 30.0f) return {0,1,0};
    if (risk < 60.0f) return {1,1,0};
    return {1,0,0};
}
```

**Corrección recomendada:** separar por zona:
- `neckRisk = f(neckAngle, time)`
- `lumbarRisk = f(torsoAngle, weight, distance, time)`
- `shoulderRisk = f(armAngle, weight, time)`
- `kneeRisk = f(kneeAngle, weight, time)`

Y documentar rangos esperados de inputs.

---

## 12) UI (mínima vs ImGui)

### MVP mínimo
- Teclas para cambiar escenario (ej. `1/2/3`, `N/P`)
- Mostrar texto (título escenario + riesgos) en overlay simple o consola

### Con ImGui (si el equipo lo domina)
- Sliders: ángulos/peso/distancia/tiempo
- Botones: siguiente/anterior escenario
- Panel: riesgos por zona + recomendación

**Regla:** UI no dibuja geometría ni calcula riesgo.

---

## 13) Pasos para construir el proyecto base (ruta recomendada)

1. Ventana base (GLFW + GLEW) + loop principal
2. Shader + Camera (orbital/fija)
3. Dibujar 1 cubo (Mesh + Renderer)
4. BodyPart (transform + color + zone)
5. HumanBody (con jerarquía real)
6. Scenarios (3 posturas: correcto/encorvado/levantamiento incorrecto)
7. RiskAnalyzer (heurístico simple)
8. Pintar zonas (verde/amarillo/rojo)
9. Controles (teclas/ImGui)
10. Pulir demo (texto, recomendaciones, transiciones)

---

## 14) Hitos con criterios de “Done” (para integrar sin fricción)

- **H1 Base gráfica**
  - Ventana + shader + cámara
  - Se ve un cubo, sin warnings críticos

- **H2 Pseudo cuerpo**
  - Partes dibujan con colores individuales
  - Transformaciones funcionan

- **H3 Posturas**
  - 3 escenarios aplicados sin reconstruir VAOs/shaders
  - Cambio de escenario en runtime

- **H4 Riesgo visual**
  - Zonas cambian color según `RiskData` en runtime

- **H5 Interacción**
  - Teclado o UI para cambiar escenario y variar peso/distancia

- **H6 Demo final**
  - Panel informativo + recomendaciones
  - Escena estable “presentable”

---

## 15) Próximo objetivo inmediato (checklist base)

La primera versión compartida debe:
- Compilar al menos en la máquina de un integrante
- Abrir ventana OpenGL
- Cámara 3D básica
- Pseudo-cuerpo visible (aunque sean cubos)
- Cambiar entre al menos 2 posturas con teclado
- Tener archivos separados (App, Renderer, HumanBody, ScenarioManager, RiskAnalyzer)

---

## 16) Riesgos principales detectados y mitigación

1. **Jerarquía incompleta del cuerpo**
   - Mitigar: `parentIndex` + cálculo `world = parent * local`

2. **Rotaciones ambiguas**
   - Mitigar: convención ejes/unidades + clamps; considerar quats internos

3. **Acoplamiento UI–Simulación–Render**
   - Mitigar: contratos `ScenarioData/RiskData` + dependencias unidireccionales

4. **Scope creep (texturas, importación, músculos, física)**
   - Mitigar: mantenerlo como “enhancements” post-MVP, con toggles

---

## 17) Mejoras H6 — Demo final y pulido (post-MVP)

Estado actual: **Hitos H1–H5 completados**. El proyecto tiene cuerpo con jerarquía real, texturas, 4 escenarios, panel ImGui, colores de riesgo y ejes X/Y/Z por articulación.

Las siguientes mejoras corresponden al **Hito H6 (Demo final)**:

### H6-A: Transición suave entre escenarios ← PRIORITARIA
Al presionar N/P, los ángulos interpolan suavemente desde la postura actual hasta la nueva (~1 segundo).

**Implementación:**
- Variables en `App`: `mEscenarioActual`, `mEscenarioDestino`, `mTiempoTransicion`, `mEnTransicion`
- Función `interpolar(ScenarioData a, ScenarioData b, float t)` en `Types.h`
- En `App::update(dt)`: `t += dt / duracion`, aplicar `glm::smoothstep`, llamar `setScenario(interpolado)`
- `glm::smoothstep(0,1,t)`: transición suave (empieza lento, acelera, termina lento)
- **No modifica** HumanBody, Renderer ni los archivos de escenario.

### H6-B: Recomendaciones automáticas en UI
Texto contextual según zona de mayor riesgo:
- Lumbar > 60: "⚠ Dobla las rodillas, no la espalda"
- Hombros > 60: "⚠ Mantén los brazos cerca del cuerpo"
- Cuello > 60: "⚠ Mantén la cabeza alineada con la columna"
- Rodillas > 60: "⚠ Evita hiperextender las rodillas"

### H6-C: Iluminación básica Phong
Agregar normal a los vértices y cálculo de luz en el shader.
- Hace que los cilindros muestren profundidad real (sombra/luz)
- Posición de luz fija: `{2, 4, 3}`

### H6-D: Sliders ImGui en tiempo real
- Sliders para `pesoCarga` y `distanciaCarga` que recalculan riesgo en cada frame
- Permite explorar el impacto ergonómico sin cambiar de escenario

---

## 18) Convenciones de repo (recomendado)
- `assets/` solo recursos necesarios (no outputs).
- No subir binarios/outputs de VS (`x64/Debug`, `*.pdb`, etc.)
- Si usan `vcpkg` con manifest: **no subir `vcpkg_installed/`** (se regenera).

---

## 19) Trazabilidad (Word → README)

Esta sección asegura que el contenido del documento **“Guía técnica de organización modular”** quedó reflejado en este README, incluyendo las **optimizaciones** discutidas.

| Sección en Word | Idea principal | Dónde está en este README |
|---|---|---|
| Decisión consolidada (tipo de proyecto, enfoque, núcleo técnico, modelo humano, Assimp) | Definir alcance realista y stack | **1) Resumen** + notas en **2) MVP** |
| 1. Objetivo del documento | Trabajo paralelo sin interferencia, base estable | **3) Principios** + **5) Módulos** |
| 2. Alcance técnico inicial | MVP: cuerpo simplificado + posturas + colorear riesgo | **2) Alcance del MVP** |
| 3. Principio de organización del equipo | Separar responsabilidades; usar estructuras compartidas; compilar siempre | **3) Principios** + **6) Contratos** |
| 4. División sugerida por responsabilidades | Asignación por módulos | **5) Módulos y responsabilidades** |
| 5. Estructura de carpetas | Layout recomendado | **4) Estructura de carpetas** |
| 6. Archivos y propósito | Rol de cada archivo/clase | **4) Estructura** + enfoque de módulos en **5** (si quieren, se puede añadir un listado de archivos 1:1) |
| 7. Estructuras compartidas | ScenarioData/RiskData/RiskZone | **6.1) ScenarioData / RiskData / RiskZone** |
| 8. Flujo general del programa | Orden del loop | **9) Flujo del programa** |
| 9. Pasos para construir base | Pasos 1..10 | **13) Pasos** |
| 10. Diseño del pseudo cuerpo | Jerarquía de partes | **10) Diseño del pseudo-cuerpo** + corrección en **6.2** |
| 11. Interfaces iniciales recomendadas | Bocetos de clases | **6.2 / 8 / 9** (adaptado a arquitectura limpia) |
| 12. Modelo simple de riesgo | Fórmula y color | **11) Modelo simple de riesgo** |
| 13. Dependencias recomendadas | vcpkg deps | **1) Resumen** (y se mantiene `vcpkg.json` en el repo) |
| 14. Trabajo en paralelo sin romper | reglas prácticas | **3) Principios** + **14) Hitos** |
| 15. Hitos de avance | roadmap | **14) Hitos** |
| 16. Próximo objetivo inmediato | checklist mínimo | **15) Próximo objetivo inmediato** |
| 17. Recomendación final | construir demo estable y modular | Reflejado en **3) Principios** + **14/15** |

### Optimizaciones añadidas (no explícitas en Word, pero necesarias para implementar)
- **Jerarquía real en el cuerpo**: `parentIndex` + `world = parent * local` (**6.2**).
- **Convención de rotaciones**: ejes/unidades/clamps (y opción de quaternions) (**7**).
- **API Renderer↔Body**: evitar OpenGL low-level dentro de `HumanBody` (**8**).
- **Criterios de “Done” por hitos**: para integrar sin fricción (**14**).
