#include "./ports.h"
#include <stdint.h>

#define ATA_IRQ0	32+14
#define ATA_IRQ1	32+15
#define ATA_IRQ2	32+11
#define ATA_IRQ3	32+9

#define ATA_BASE0	0x1F0
#define ATA_BASE1	0x170
#define ATA_BASE2	0x1E8
#define ATA_BASE3	0x168

#define ATA_TIMEOUT 5000
#define ATA_IDENTIFY_TIMEOUT 1000

#define ATA_DATA	0	/* data register */
#define ATA_ERROR	1	/* error register */
#define ATA_COUNT	2	/* sectors to transfer */
#define ATA_SECTOR	3	/* sector number */
#define ATA_CYL_LO	4	/* low byte of cylinder number */
#define ATA_CYL_HI	5	/* high byte of cylinder number */
#define ATA_FDH		6	/* flags, drive and head */
#define ATA_STATUS	7
#define ATA_COMMAND	7
#define ATA_CONTROL	0x206

#define ATA_FLAGS_ECC	0x80	/* enable error correction */
#define ATA_FLAGS_LBA	0x40	/* enable linear addressing */
#define ATA_FLAGS_SEC	0x20	/* enable 512-byte sectors */
#define ATA_FLAGS_SLV	0x10	/* address the slave drive */

#define ATA_STATUS_BSY	0x80	/* controller busy */
#define ATA_STATUS_RDY	0x40	/* drive ready */
#define ATA_STATUS_WF	0x20	/* write fault */
#define ATA_STATUS_SC	0x10	/* seek complete (obsolete) */
#define ATA_STATUS_DRQ	0x08	/* data transfer request */
#define ATA_STATUS_CRD	0x04	/* corrected data */
#define ATA_STATUS_IDX	0x02	/* index pulse */
#define ATA_STATUS_ERR	0x01	/* error */

#define ATA_COMMAND_IDLE		0x00
#define ATA_COMMAND_READ		0x20	/* read data */
#define ATA_COMMAND_WRITE		0x30	/* write data */
#define ATA_COMMAND_IDENTIFY		0xec

#define ATAPI_COMMAND_IDENTIFY 0xa1
#define ATAPI_COMMAND_PACKET   0xa0

#define ATAPI_FEATURE	1
#define ATAPI_IRR 2
#define ATAPI_SAMTAG 3
#define ATAPI_COUNT_LO 4
#define ATAPI_COUNT_HI 5
#define ATAPI_DRIVE 6

#define SCSI_READ10            0x28
#define SCSI_SENSE             0x03

#define ATA_CONTROL_RESET	0x04
#define ATA_CONTROL_DISABLEINT	0x02
#define FAT32_START_LBA 100

static inline void insw(uint16_t port, void *addr, uint32_t count){
    __asm__ volatile ("rep insw": "+D"(addr),"+c"(count):"d"(port): "memory");
}

void read_sector(uint32_t lba, uint16_t *buffer ){
    const uint16_t DATA_PORT = 0x1F0;
    const uint16_t ERROR_PORT= 0x1F1;
    const uint16_t SECTOR_COUNT_PORT= 0x1F2;
    const uint16_t LBA_LOW_PORT= 0x1F3;
    const uint16_t LBA_MID_PORT= 0x1F4;
    const uint16_t LBA_HIGH_PORT= 0x1F5;
    const uint16_t DRIVE_PORT= 0x1F6;
    const uint16_t COMMAND_PORT= 0x1F7;
    const uint16_t STATUS_PORT= 0x1F8;

    port_byte_out(DRIVE_PORT, 0xE0| ((lba >> 24)&0x0F));
    port_byte_out(SECTOR_COUNT_PORT, 1);

    port_byte_out(LBA_LOW_PORT,(uint8_t)(lba & 0xFF));
    port_byte_out(LBA_MID_PORT, (uint8_t)((lba >> 8)& 0xFF));
    port_byte_out(LBA_HIGH_PORT, (uint8_t)((lba >> 16)& 0xFF));

    port_byte_out(COMMAND_PORT, 0x20);

    while(!(port_byte_in(STATUS_PORT) & 0x08));
    insw(DATA_PORT, buffer, 256);
}

void ata_write_sector(uint32_t lba, const uint16_t *buffer) {
    const uint16_t DATA_PORT = 0x1F0;
    const uint16_t SECTOR_COUNT_PORT = 0x1F2;
    const uint16_t LBA_LOW_PORT = 0x1F3;
    const uint16_t LBA_MID_PORT = 0x1F4;
    const uint16_t LBA_HIGH_PORT = 0x1F5;
    const uint16_t DRIVE_PORT = 0x1F6;
    const uint16_t COMMAND_PORT = 0x1F7;
    const uint16_t STATUS_PORT = 0x1F7;

    // Sélection du disque et LBA
    port_byte_out(DRIVE_PORT, 0xE0 | ((lba >> 24) & 0x0F));

    // Nombre de secteurs à écrire (1 secteur)
    port_byte_out(SECTOR_COUNT_PORT, 1);

    // Spécification de l'adresse LBA
    port_byte_out(LBA_LOW_PORT, (uint8_t)(lba & 0xFF));
    port_byte_out(LBA_MID_PORT, (uint8_t)((lba >> 8) & 0xFF));
    port_byte_out(LBA_HIGH_PORT, (uint8_t)((lba >> 16) & 0xFF));

    // Envoyer la commande d'écriture (0x30)
    port_byte_out(COMMAND_PORT, 0x30);

    // Attendre que le disque soit prêt à recevoir les données
    while (!(port_byte_in(STATUS_PORT) & 0x08));

    // Écrire les 512 octets (256 mots de 16 bits)
    for (int i = 0; i < 256; i++) {
        port_word_out(DATA_PORT, buffer[i]);
    }

    // Attendre la fin de l'opération
    while (port_byte_in(STATUS_PORT) & 0x80);
}

void fat32_format() {
    uint16_t boot_sector[256] = {0};

    // Remplir les champs nécessaires du secteur de boot
    boot_sector[0] = 0xEB58;  // JMP instruction + NOP
    boot_sector[1] = 0x90;
    boot_sector[3] = 0x4641;  // "FAT32"
    boot_sector[4] = 0x0200;  // Taille d'un secteur = 512 octets
    boot_sector[5] = 0x0008;  // Taille du cluster (4 Ko)
    boot_sector[14] = 2;      // Nombre de FATs
    boot_sector[16] = 0x2000; // Taille totale du volume (exemple)
    boot_sector[17] = 0x0001; // Taille d'une FAT en secteurs

    // Écrire le secteur de boot sur le disque
    ata_write_sector(FAT32_START_LBA, boot_sector);
}

void fat32_write_file(uint32_t cluster, const uint16_t *data) {
    uint32_t lba = FAT32_START_LBA +2+ cluster * 8; // Conversion cluster -> LBA (exemple)
    
    ata_write_sector(lba, data);

    // Mettre à jour la FAT (simplifié)
    uint16_t fat[256] = {0};
    fat[cluster] = 0xFFFF; // Marquer la fin du fichier
    ata_write_sector(FAT32_START_LBA + 1, fat); // Écrire la FAT sur le disque
}