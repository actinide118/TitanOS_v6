#include "alea.h"
#include <stdint.h>
#include "../cpu/timer.h"

uint32_t random(uint32_t *seed) {
    // Paramètres du LCG (conseillés pour uint32_t)
    const uint32_t a = 1664525;       // Multiplicateur
    const uint32_t c = 1013904223;    // Incrément
    const uint32_t m = 0xFFFFFFFF;    // Modulo (2^32 pour uint32_t)

    // Calcul de la prochaine valeur pseudo-aléatoire
    *seed = (a * (*seed) + c) & m;

    return *seed;
}