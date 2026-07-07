#pragma once

// Núcleo de la aplicación (ventana + loop principal).
// Mantiene la inicialización/cierre del sistema gráfico.
// NOTA: Este módulo no debería contener lógica de simulación ni del cuerpo.

class App
{
public:
    // Ejecuta la aplicación. Retorna 0 si todo sale bien.
    int ejecutar();

private:
    // Secciones del ciclo de vida.
    bool inicializar();
    void procesarEntrada(float deltaTiempo);
    void actualizar(float deltaTiempo);
    void renderizar();

    // Dibujo específico de la demo del Hito 1 (cubo).
    void dibujarDemoCubo();

    void cerrar();
};
