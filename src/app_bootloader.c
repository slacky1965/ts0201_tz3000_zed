#include "app_main.h"

#define BUFF_SIZE   256

void bootloader_check() {

    uint8_t  marker[11] = BOOTLOAD_MARKER;
    uint8_t  buff[10] = {0};
    uint8_t  bootloader_buff[BUFF_SIZE] = {0};
    uint32_t bootloader_offset = 0;
    uint32_t bootloader_addr = BOOTLOADER_ADDR;
    uint32_t image_ota_addr = IMAGE_OTA_ADDR1;


    tl_header_t fw_header, boot_header;

    for (uint8_t i = 0; i < 5; i++) {
        flash_read_page(BOOTLOAD_MARKER_ADDR, marker[0], buff);

        if (memcmp(buff, marker+1, marker[0]) == 0) {
#if UART_PRINTF_MODE
            printf("SDK bootloader\r\n");
#endif
            return;
        }
    }

    for (uint8_t i = 0; i < 5; i++) {
        flash_read_page(bootloader_addr, sizeof(tl_header_t), (uint8_t*)&boot_header);

        if (boot_header.manuf_code == SLACKY_MANUF_CODE) {
    #if UART_PRINTF_MODE
            printf("SDK bootloader\r\n");
    #endif
            return;
        }
    }


#if UART_PRINTF_MODE
    printf("Tuya bootloader\r\n");
#endif

    flash_read_page(image_ota_addr, sizeof(tl_header_t), (uint8_t*)&fw_header);

    if (fw_header.bin_size > IMAGE_OTA_SIZE || memcmp(fw_header.magic, TL_MAGIC, 4) || fw_header.sig != TL_SIG) {
#if UART_PRINTF_MODE
        printf("No fimware on address: 0x%x\r\n", image_ota_addr);
#endif
        image_ota_addr = IMAGE_OTA_ADDR2;

        flash_read_page(image_ota_addr, sizeof(tl_header_t), (uint8_t*)&fw_header);

        if (fw_header.bin_size > IMAGE_OTA_SIZE || memcmp(fw_header.magic, TL_MAGIC, 4) || fw_header.sig != TL_SIG) {
#if UART_PRINTF_MODE
            printf("No fimware on address: 0x%x\r\n", image_ota_addr);
#endif
            return;
        }
    }

    flash_read_page(image_ota_addr+fw_header.bin_size, sizeof(tl_header_t), (uint8_t*)&boot_header);

//    printf("fw_size: 0x%x, boot_size: 0x%x, boot_size: %d\r\n", fw_header.bin_size, boot_header.bin_size, BOOT_SIZE);

    if (memcmp(boot_header.magic, TL_MAGIC, 4) || boot_header.bin_size != BOOT_SIZE || boot_header.sig != TL_SIG) {
#if UART_PRINTF_MODE
        printf("Bootloader not found\r\n");
#endif
        return;
    }


    flash_unlock();

    bootloader_offset = image_ota_addr + fw_header.bin_size;

    uint8_t read_buff[BUFF_SIZE];

    for (uint32_t i = 0; i < boot_header.bin_size; i += BUFF_SIZE) {
        if (i % FLASH_SECTOR_SIZE == 0) {
//            printf("erase_addr: 0x%x\r\n", i);
            flash_erase(bootloader_addr + i);
        }
//        printf("read_addr: 0x%x\r\n", bootloader_offset+i);
        flash_read_page(bootloader_offset+i, BUFF_SIZE, bootloader_buff);
//        printf("write_addr: 0x%x\r\n", bootloader_addr+i);
        flash_write(bootloader_addr+i, BUFF_SIZE, bootloader_buff);

        flash_read_page(bootloader_addr+i, BUFF_SIZE, read_buff);

        if(memcmp(read_buff, bootloader_buff, BUFF_SIZE)){
            SYSTEM_RESET();
        }
    }

    for (uint8_t i = 0; i < 10; i++) {
        flash_erase(BOOTLOAD_MARKER_SECTOR);
        flash_read(0, BUFF_SIZE, bootloader_buff);

        flash_write(BOOTLOAD_MARKER_ADDR, marker[0], marker+1);

        memset(bootloader_buff, 0, BUFF_SIZE);

        flash_read_page(BOOTLOAD_MARKER_ADDR, marker[0], buff);

        if (memcmp(buff, marker+1, marker[0]) == 0) {
//            printf("i: %d\r\n", i);
            break;
        }
    }

#if UART_PRINTF_MODE
    printf("Bootloader is overwritten. Reset\r\n");
#endif

    uint32_t erase_size = IMAGE_OTA_ADDR_END - image_ota_addr;

    for (uint32_t i = 0; i < erase_size; i += FLASH_SECTOR_SIZE) {
//        printf("erase_addr: 0x%x\r\n", image_ota_addr + i);
        flash_erase(image_ota_addr + i);
    }
    SYSTEM_RESET();
}
