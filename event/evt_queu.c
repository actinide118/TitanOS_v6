#include <stddef.h>
#include <stdint.h>
#include "evt_queu.h"
#include "../driver/vga.h"

// Définition du type de fonction callback

// Variable globale pour stocker le callback
static callback_t callback = NULL;

// Fonction pour définir le callback
void set_callback_keyboard(callback_t cb) {
    callback = cb;
}

// Fonction pour émettre un événement
void emit_keyboard(struct keymap km, uint8_t arg2) {
    if (callback) {
        callback(km, arg2); // Appelle le callback si défini
    } 
}

static callback_t callback_down = NULL;

// Fonction pour définir le callback
void set_callback_keyboard_up(callback_t cb) {
    callback_down = cb;
}

// Fonction pour émettre un événement
void emit_keyboard_up(struct keymap km, uint8_t arg2) {
    if (callback_down) {
        callback_down(km, arg2); // Appelle le callback si défini
    } 
}