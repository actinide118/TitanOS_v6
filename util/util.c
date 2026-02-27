#include "util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
       int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    s32 tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}
bool backspace(char s[]) {
    int len = strlen(s);
    if (len > 0) {
        s[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}
void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void uint16_to_hex(uint16_t value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    //buffer[0] = '0';
    //buffer[1] = 'x';
    for (int i = 0; i < 4; i++) {
        buffer[3 - i] = hex_digits[value & 0xF];
        value >>= 4;
    }
    buffer[4] = ' ';  // Ajoute un espace après la représentation hexadécimale
    buffer[5] = '\0'; // Termine la chaîne de caractères
}

// Fonction pour diviser une chaîne `str` en sous-chaînes selon le séparateur `delimiter`
// et stocker les pointeurs vers ces sous-chaînes dans `result`.
// Retourne le nombre de sous-chaînes trouvées.
int split(const char* str, char delimiter, char* result[], int max_splits) {
    int count = 0;
    const char* start = str;
    
    while (*str) {
        if (*str == delimiter) {
            // Terminer la sous-chaîne actuelle
            if (str != start) {
                result[count] = (char*)start;
                ((char*)str)[0] = '\0';  // Remplace le délimiteur par '\0'
                count++;
                
                // Arrêter si le nombre maximal de divisions est atteint
                if (count >= max_splits) {
                    break;
                }
            }
            start = str + 1;  // Déplacer le début à après le délimiteur
        }
        str++;
    }

    // Ajouter la dernière sous-chaîne si elle existe
    if (*start && count < max_splits) {
        result[count] = (char*)start;
        count++;
    }

    return count;
}
uint32_t string_to_uint32(const char* str) {
    uint32_t num = 0;

    // Parcours de chaque caractère de la chaîne jusqu'à la fin ('\0')
    while (*str) {
        // Vérifie si le caractère est un chiffre
        if (*str >= '0' && *str <= '9') {
            num = num * 10 + (*str - '0');  // Conversion du caractère en chiffre et ajout au nombre
        } else {
            // Arrêter si un caractère non numérique est rencontré
            break;
        }
        str++;
    }

    return num;
}
void uint32_to_string(uint32_t num, char* buffer) {
    int index = 0;
    uint32_t temp = num;
    char temp_buffer[12];  // Assez grand pour contenir un uint32_t et le caractère nul

    // Gère le cas où le nombre est zéro
    if (num == 0) {
        buffer[index++] = '0';
        buffer[index] = '\0';
        return;
    }

    // Convertit chaque chiffre en commençant par la fin (remplissage de temp_buffer)
    while (temp > 0) {
        temp_buffer[index++] = (temp % 10) + '0';  // Prend le dernier chiffre et le convertit en caractère
        temp /= 10;  // Réduit le nombre
    }

    // Ajoute le caractère nul de fin
    temp_buffer[index] = '\0';

    // Inverse temp_buffer pour mettre les chiffres dans le bon ordre dans buffer
    for (int i = 0; i < index; i++) {
        buffer[i] = temp_buffer[index - i - 1];
    }

    buffer[index] = '\0';  // Termine la chaîne par un caractère nul
}

void uint8_to_string(uint8_t num, char* buffer) {
    int index = 0;
    uint8_t temp = num;
    char temp_buffer[3];  // Assez grand pour contenir un uint32_t et le caractère nul

    // Gère le cas où le nombre est zéro
    if (num == 0) {
        buffer[index++] = '0';
        buffer[index] = '\0';
        return;
    }

    // Convertit chaque chiffre en commençant par la fin (remplissage de temp_buffer)
    while (temp > 0) {
        temp_buffer[index++] = (temp % 10) + '0';  // Prend le dernier chiffre et le convertit en caractère
        temp /= 10;  // Réduit le nombre
    }

    // Ajoute le caractère nul de fin
    temp_buffer[index] = '\0';

    // Inverse temp_buffer pour mettre les chiffres dans le bon ordre dans buffer
    for (int i = 0; i < index; i++) {
        buffer[i] = temp_buffer[index - i - 1];
    }

    buffer[index] = '\0';  // Termine la chaîne par un caractère nul
}

void uint16_array_to_string(uint16_t* array, size_t length, char* result_buffer) {
    char* current_position = result_buffer;

    for (size_t i = 0; i < length; i++) {
        // Extraction des deux caractères d'un uint16_t
        *current_position++ = (char)(((array[i] >> 8) & 0xFF)& 0x80);  // Premier caractère (octet supérieur)
        *current_position++ = (char)((array[i] & 0xFF)& 0x80);         // Deuxième caractère (octet inférieur)
    }

    // Ajoute le caractère nul de fin
    *current_position = '\0';
}
uint8_t string_to_uint8(const char* str) {
    uint8_t num = 0;

    // Parcourt chaque caractère de la chaîne jusqu'à la fin ('\0')
    while (*str) {
        // Vérifie si le caractère est un chiffre
        if (*str >= '0' && *str <= '9') {
            // Calcule la nouvelle valeur et vérifie qu'elle reste dans la plage de uint8_t
            uint8_t new_num = num * 10 + (*str - '0');
            if (new_num < num) {
                // Détection d'un dépassement
                return 0;  // Retourne 0 en cas de dépassement (peut être adapté selon les besoins)
            }
            num = new_num;
        } else {
            // Arrête si un caractère non numérique est rencontré
            break;
        }
        str++;
    }

    return num;
}

u32 free_mem_addr = 0x10000;
/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 kmalloc(u32 size, int align, u32 *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}