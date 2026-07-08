# Guía de instalación — ErgoSim 3D

Instrucciones para clonar el proyecto y compilarlo desde cero en Windows con Visual Studio y vcpkg.

---

## Requisitos previos

### 1. Visual Studio 2022 (Community, Professional o Enterprise)
Descarga desde: https://visualstudio.microsoft.com/

Durante la instalación, asegúrate de incluir la carga de trabajo:
- **Desarrollo para el escritorio con C++**

Componentes individuales que deben estar marcados:
- MSVC v143 (o v142) — compilador C++
- Windows 10/11 SDK
- C++ CMake Tools (opcional pero recomendado)

### 2. Git
Descarga desde: https://git-scm.com/

---

## Clonar el repositorio

Abre una terminal (Git Bash, PowerShell o cmd) y ejecuta:

```bash
git clone https://github.com/Meyly-SK/PosturaErgonomica.git
cd PosturaErgonomica
```

---

## Abrir el proyecto en Visual Studio

1. Abre Visual Studio 2022.
2. Selecciona **"Abrir una solución o archivo de proyecto"**.
3. Navega a la carpeta clonada y abre el archivo:
   ```
   prueba_proyecto_2.slnx
   ```
4. Visual Studio abrirá la solución con todos los archivos fuente.

---

## Dependencias: vcpkg (automático)

El proyecto usa **vcpkg en modo manifest** (`vcpkg.json` en la raíz).

**No necesitas instalar nada manualmente.**
Al compilar por primera vez, Visual Studio detecta el `vcpkg.json` y descarga automáticamente:

| Librería | Versión | Propósito |
|---|---|---|
| `glfw3` | 3.4 | Ventana y eventos |
| `glew` | 2.3 | Carga de extensiones OpenGL |
| `glm` | 1.0.3 | Matemáticas 3D (vectores, matrices) |
| `stb` | latest | Carga de texturas |

> **Nota:** La primera compilación puede tardar varios minutos mientras vcpkg descarga y compila las dependencias. Las siguientes compilaciones son rápidas.

### Requisito: vcpkg integrado con Visual Studio

Si es tu primera vez usando vcpkg con Visual Studio, necesitas integrarlo una vez:

1. Descarga vcpkg (si no lo tienes):
   ```bash
   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
   cd C:\vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```
   
2. El comando `integrate install` configura Visual Studio para usar vcpkg automáticamente en todos los proyectos.

> Si ya tienes vcpkg instalado y ejecutaste `integrate install` antes, no necesitas hacer nada más.

---

## Compilar el proyecto

### Opción A: Desde Visual Studio (recomendado)
1. Selecciona la configuración: **Debug | x64** (barra superior).
2. Presiona **Ctrl + Shift + B** para compilar (Build Solution).
3. Presiona **F5** para compilar y ejecutar.

### Opción B: Desde la línea de comandos (MSBuild)
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" prueba_proyecto_2.slnx /t:Build /p:Configuration=Debug /p:Platform=x64
```
El ejecutable generado estará en:
```
x64\Debug\prueba_proyecto_2.exe
```

---

## Controles de la aplicación

| Tecla | Acción |
|---|---|
| `WASD` | Mover la cámara |
| `Q / E` | Bajar / subir cámara |
| `Mouse` | Girar la cámara (requiere captura activa) |
| `M` | Activar / desactivar captura del mouse |
| `N` | Siguiente escenario / postura |
| `P` | Anterior escenario / postura |
| `1` | Ver cuerpo completo |
| `2` | Ver solo cabeza + cuello |
| `3` | Ver solo hombros |
| `4` | Ver solo brazo izquierdo |
| `5` | Ver solo brazo derecho |
| `6` | Ver solo pierna izquierda |
| `7` | Ver solo pierna derecha |
| `ESC` | Cerrar la aplicación |

---

## Estructura del proyecto

```
prueba_proyecto_2/
├── src/
│   ├── core/           → App (loop principal), Input (teclado/mouse)
│   ├── graphics/       → Shader, Camera, Mesh, PrimitiveFactory, Renderer
│   ├── body/           → BodyPart, HumanBody (cuerpo 3D modular)
│   ├── simulation/
│   │   ├── Types.h                  → Contratos: ScenarioData, RiskData, ZonaRiesgo
│   │   ├── ScenarioManager.h/.cpp   → Gestión de escenarios
│   │   └── escenarios/              → Un archivo .h por postura (agregar aquí los nuevos)
│   └── ui/             → (pendiente: panel informativo)
├── assets/
│   ├── shaders/        → Vertex y fragment shaders
│   └── textures/       → (pendiente)
├── vcpkg.json          → Dependencias del proyecto
└── prueba_proyecto_2.slnx  → Solución de Visual Studio
```

---

## Agregar un escenario nuevo

Ver instrucciones detalladas en:
```
prueba_proyecto_2/src/simulation/escenarios/README_ESCENARIOS.md
```

Resumen de 2 pasos:
1. Crear `src/simulation/escenarios/Escenario_MiPostura.h`
2. Registrar en `src/simulation/ScenarioManager.cpp` (2 líneas)

---

## Problemas comunes

### Error: "no se puede abrir el archivo de origen glfw3.h"
**Causa:** vcpkg no está integrado con Visual Studio.
**Solución:** Ejecutar `.\vcpkg integrate install` en la carpeta de vcpkg.

### Error al compilar: "vcpkg_installed no encontrado"
**Causa:** vcpkg no se ejecutó correctamente.
**Solución:** 
```bash
cd ruta/al/proyecto
vcpkg install --triplet x64-windows
```

### La ventana no abre / crash al inicio
**Causa probable:** Drivers de GPU desactualizados o GPU sin soporte OpenGL 3.3.
**Solución:** Actualizar drivers de GPU (NVIDIA, AMD o Intel).

### El mouse no responde al mover la cámara
**Solución:** Presionar `M` para activar la captura del mouse. Con mouse capturado, el cursor desaparece y el mouse controla la cámara.
