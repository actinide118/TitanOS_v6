/*
 * =====================================================================================
 *
 *       Filename:  alea.c
 *
 *    Description:  This file implement a simple PNRG. Maybe it's not a good idea to use this one for cryptography purposes but cryptography is actually not a thing 
 *
 *        Version:  1.0
 *        Created:  27/05/2026 21:12:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "alea.h"
#include <stdint.h>
#include "../cpu/timer.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random
 *  Description:  Return a number generated pseudo-randomly regarding a seed 
 * =====================================================================================
 */
uint32_t random(uint32_t seed) {
    // Paramètres du LCG (conseillés pour uint32_t)
    const uint32_t a = 1664525;       // Multiplicateur
    const uint32_t c = 1013904223;    // Incrément
    const uint32_t m = 0xFFFFFFFF;    // Modulo (2^32 pour uint32_t)

    // Calcul de la prochaine valeur pseudo-aléatoire
    seed = (a * (seed) + c) & m;

    return seed;
}
