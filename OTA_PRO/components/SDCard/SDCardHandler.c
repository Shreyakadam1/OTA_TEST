/**
 * @SDCard_Handler.c
  * @author 1)Kajal Mohite 
 *			2)Shreya Kadam 
 *			3)Uday Salokhe 
 * @brief This file contains function definitions for SD card interfacing using ESP32 in SPI mode.
 * @version 1.0
 * @date 2025-07-29
 */
 
#include <string.h>

#include "SDCardHandler.h"


sdmmc_card_t *card;

/**
 * @brief Initialize and mount the SD card using SPI.
 * 
 * Sets up SPI bus, configures the SD card slot, and mounts the filesystem.
 * This is required before any file read/write operations.
 * 
 * @return ESP_OK on success, or relevant error code from esp_err_t on failure.
 */
 
 
uint8_t InitSdCard(void)
{
    LogToSDCard(SDTAG, "Initializing SD card...");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PINMOSI,
        .miso_io_num = PINMISO,
        .sclk_io_num = PINCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
     };

    esp_err_t ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK)
    {
        LogToSDCard(SDTAG, "Failed to initialize SPI bus");
        return ret;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PINCS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };

    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
     {
        //LogToSDCard(TAG, "Failed to mount SD card: %s", esp_err_to_name(ret));
        spi_bus_free(host.slot);
        return RETURN_NUM_SDCARD_MOUNTFAIL ;
     }

    //LogToSDCard(TAG, "SD card mounted successfully.");
    sdmmc_card_print_info(stdout, card);
    return RETURN_NUM_SDCARD_MOUNTSUCCESSFULLY;
}


/**
 * @brief Append a log entry to the SD card file.
 *
 * Appends a formatted log message with a tag and message to the file located at
 * MOUNT_POINT/SDCARD.txt. If the file does not exist, the function will fail.
 *
 * @param tag     Log category or identifier to be written before the message.
 * @param message The message string to log.
 *
 * @return RETURN_NUM_SDCARD_APPENDSUCCESSFULLY on successful append,
 *         RETURN_NUM_SDCARD_APPENDFAIL if the file does not exist or cannot be opened.
 */


uint8_t LogToSDCard(const char *tag, const char *message)
{
    const char *filePath = MOUNT_POINT "/SDCARD.txt";

    if (!FileExists(filePath)) {
        if (!CreateFile(filePath)) {  // create if not found
            return RETURN_NUM_SDCARD_APPENDFAIL;
        }
    }

    FILE *PtrFile = fopen(filePath, "a");
    if (PtrFile == NULL) {
        return RETURN_NUM_SDCARD_APPENDFAIL;
    }

    fprintf(PtrFile, "%s: %s\n", tag, message);
    fclose(PtrFile);
    return RETURN_NUM_SDCARD_APPENDSUCCESSFULLY;
}




/**
 * @brief Check if a file exists on the SD card.
 *
 * Attempts to open the specified file in read mode to verify its existence.
 *
 * @param StringPath Full path to the file.
 *
 * @return true if the file exists, false if it does not.
 */
 
 
bool FileExists(const char *StringPath)
{
    FILE *file = fopen(StringPath, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}


/**
 * @brief Create a file on the SD card if it does not exist.
 *
 * Opens the specified file in append mode. If the file does not exist, it will be created.
 * Does not modify the contents if the file already exists.
 *
 * @param StringPath Full path to the file.
 *
 * @return true if the file was successfully created or already exists, false if file creation failed.
 */

bool CreateFile(const char *StringPath)
{
    FILE *PtrFile = fopen(StringPath, "w");  // "w" mode: only to create file
    if (PtrFile == NULL) {
        return false;
    }
    fclose(PtrFile);
    return true;
}

/**
 * @brief Write default content to the specified file on SD card.
 * 
 * Opens the file in write mode and writes a sample log. Overwrites if the file exists.
 * 
 * @param StringPath Full path to the file.
 *
 * @return Integer code: RETURN_NUM_SDCARD_WRITESUCCESSFULL on success, RETURN_NUM_SDCARD_WRITEFAIL on write failure.
 */
 
   
uint8_t WriteFile(const char *StringPath)
{
    FILE *PtrFile = fopen(StringPath, "w");
    if (PtrFile == NULL) {
        return RETURN_NUM_SDCARD_WRITEFAIL ;
    }
    fprintf(PtrFile, "sectr!\n");
    fprintf(PtrFile, "CONSOLE.\n");
    fclose(PtrFile);
    return RETURN_NUM_SDCARD_WRITESUCCESSFULL;
}

/**
 * @brief Read and print contents of the specified file.
 * 
 * Opens the file in read mode and prints all lines to terminal.
 * 
 * @param StringPath Full path to the file.
 *@return Integer code: RETURN_NUM_SDCARD_READSUCCESSFULL on success, RETURN_NUM_SDCARD_READFAIL on Read failure.
 */
 
 
 uint8_t ReadFile(const char *StringPath)
{
    FILE *PtrFile = fopen(StringPath, "r");
    if (PtrFile == NULL) {
        return RETURN_NUM_SDCARD_READFAIL;
    }
    //LogToSDCard(SDTAG, " Reading from %s file:", StringPath);
    char line[128];
    while (fgets(line, sizeof(line), PtrFile)) {
        printf("%s", line);
    }
    fclose(PtrFile);
    return RETURN_NUM_SDCARD_READSUCCESSFULL;
}


/**
 * @brief Append text to the specified file.
 * 
 * Opens the file in append mode and adds a new line of content.
 * 
 * @param StringPath Full path to the file.
 *
 *@return Integer code: RETURN_NUM_SDCARD_APPENDSUCCESSFULLY on success, ETURN_NUM_SDCARD_APPENDFAIL on Append failure.
 */
 
 
 /*uint8_t AppendFile(const char *StringPath)
{
    FILE *PtrFile = fopen(StringPath, "a");
    if (PtrFile == NULL) {
        return RETURN_NUM_SDCARD_APPENDFAIL;
    }
    fprintf(PtrFile, "Appended this line without erasing previous content.\n");
    fclose(PtrFile);
    return RETURN_NUM_SDCARD_APPENDSUCCESSFULLY;
}*/

uint8_t AppendFile(const char *StringPath, const char *Data)
{
    FILE *PtrFile = fopen(StringPath, "a");   // Open file in append mode
    if (PtrFile == NULL) {
        return RETURN_NUM_SDCARD_APPENDFAIL;
    }

    fprintf(PtrFile, "%s\n", Data);  // Write the passed string
    fclose(PtrFile);

    return RETURN_NUM_SDCARD_APPENDSUCCESSFULLY;
}



/**
 * @brief Display total and free space of the mounted SD card.
 * 
 * Queries and logs available storage information of the SD card.
 */
void GetSdCardSpace(void)
{
    uint64_t Num_TotalBytes, Num_FreeBytes;
    char str[64];  

    esp_err_t ret = esp_vfs_fat_info(MOUNT_POINT, &Num_TotalBytes, &Num_FreeBytes);
    if (ret == ESP_OK) {
        float a = (uint32_t)(Num_TotalBytes / (1024 * 1024));  
        sprintf(str, "Total Space: %.2f MB", a); 
        LogToSDCard(SDTAG, str);

        float b = (uint32_t)(Num_FreeBytes / (1024 * 1024));
        sprintf(str, "Free Space: %.2f MB", b);
        LogToSDCard(SDTAG, str);
    } else {
        LogToSDCard(SDTAG, "Failed to get SD card info");
    }
}



/**
 * @brief Delete the specified file from the SD card.
 * 
 * Uses unlink() to remove the file.
 * 
 * @param StringPath Full path to the file.
 *
 * @return Integer code: RETURN_NUM_SDCARD_DELETEFILESUCCESSFULLY  on success, RETURN_NUM_SDCARD_DELETEFILEFAI on Delete failure.
 */
 
 
 uint8_t DeleteFile(const char *StringPath)
{
    if (unlink(StringPath) == 0) {
        return  RETURN_NUM_SDCARD_DELETEFILESUCCESSFULLY;
    } else {
	return  RETURN_NUM_SDCARD_DELETEFILEFAIL;
    }
}


/**
 * @brief Unmount the SD card and free the SPI bus.
 * 
 * This function unmounts the SD card and releases the SPI bus.
 * It returns status codes that can be logged from main().
 * 
 * @return Integer code:
 * - RETURN_NUM_SDCARD_UNMOUNT_SUCCESS on successful unmount and SPI free.
 * - RETURN_NUM_SDCARD_UNMOUNT_FAIL on unmount failure.
 * - RETURN_NUM_SDCARD_SPIFREE_FAIL if SPI bus free fails.
 */
void SdCardUnmount(void)
{
    esp_err_t status = esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    if (status == ESP_OK) {
        status = spi_bus_free(SPI2_HOST);  
        if (status == ESP_OK) {
            printf("SD Card unmounted successfully.");
        } 
        else
         {
             printf("SPI bus release failed after unmount.");
        }
    } else {
        printf("SD Card unmount failed.");
    }
}
