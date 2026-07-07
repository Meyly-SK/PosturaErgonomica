#pragma once

// Tipos compartidos entre módulos (contratos).
// La idea es que body/simulation/ui se comuniquen por estas estructuras simples.

enum class ZonaRiesgo
{
    Cuello,
    Lumbar,
    Hombros,
    Rodillas,
    Ninguna
};
