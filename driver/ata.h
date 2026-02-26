#include <stdint.h>

void read_sector(uint32_t lba, uint16_t *buffer);
void ata_write_sector(uint32_t lba, const uint16_t *buffer);
void fat32_format();
void fat32_write_file(uint32_t cluster, const uint16_t *data);