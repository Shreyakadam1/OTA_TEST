/*
 * @ SDCardHandler.h
 * @author 
 *   1) Kajal Mohite  
 *   2) Shreya Kadam  
 *   3) Uday Salokhe  
 * @brief This header defines GPIO macros and function prototypes for SD card operations using SPI.
 * @version 1.0
 * @date 2025-07-29
 */

#ifndef MAIN_FILE_H_
#define MAIN_FILE_H_

#include <errno.h>
#include <sys/unistd.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <driver/spi_common.h>
#include <driver/sdspi_host.h>
#include <sdmmc_cmd.h>

/** @brief Mount point for the SD card */
#define MOUNT_POINT "/sdcard"

/** @brief Tag used for ESP logging */
#define SDTAG "SD_CARD"

// SPI Pin Definitions
/** @brief MISO pin number */
#define PINMISO  13
/** @brief MOSI pin number */
#define PINMOSI  11
/** @brief CLK (SCK) pin number */
#define PINCLK   12
/** @brief Chip Select (CS) pin number */
#define PINCS    10

// Return codes for SD card operations

/** @brief SD card mount successful */
#define RETURN_NUM_SDCARD_MOUNTSUCCESSFULLY      0

/** @brief SD card mount failed */
#define RETURN_NUM_SDCARD_MOUNTFAIL              1

/** @brief File write successful */
#define RETURN_NUM_SDCARD_WRITESUCCESSFULL       2

/** @brief File write failed */
#define RETURN_NUM_SDCARD_WRITEFAIL              3

/** @brief File read successful */
#define RETURN_NUM_SDCARD_READSUCCESSFULL        4

/** @brief File read failed */
#define RETURN_NUM_SDCARD_READFAIL               5

/** @brief File append successful */
#define RETURN_NUM_SDCARD_APPENDSUCCESSFULLY     6

/** @brief File append failed */

#define RETURN_NUM_SDCARD_APPENDFAIL             7
/** @brief File delete successful */
#define RETURN_NUM_SDCARD_DELETEFILESUCCESSFULLY 8

/** @brief File delete failed */
#define RETURN_NUM_SDCARD_DELETEFILEFAIL         9



/**
 * @brief Initializes the SD card over SPI interface.
 * @return 0 if success, 1 if failed.
 */
 
uint8_t InitSdCard(void);


/**
 * @brief Writes a new file to the SD card.
 * @param StringPath Path to the file to write.
 * @return 2 if success, 3 if failed.
 */
 
uint8_t WriteFile(const char *StringPath);


/**
 * @brief Reads the contents of a file from the SD card.
 * @param StringPath Path to the file to read.
 * @return 4 if success, 5 if failed.
 */
 
uint8_t ReadFile(const char *StringPath);

/**
 * @brief Appends data to an existing file on the SD card.
 * @param StringPath Path to the file to append to.
 * @return 6 if success, 7 if failed.
 */
 
//uint8_t AppendFile(const char *StringPath);
uint8_t AppendFile(const char *StringPath, const char *Data);

/**
 * @brief Deletes a file from the SD card.
 * @param StringPath Path to the file to delete.
 * @return 8 if success, 9 if failed.
 */
 
uint8_t DeleteFile(const char *StringPath);

/**
 * @brief Displays total and free space of the SD card.
 */
 
void GetSdCardSpace(void);

/**
 * @brief Create a file
 */

bool CreateFile(const char *StringPath);


/**
 * @brief Check if file is exist or not
 */
bool FileExists(const char *StringPath);

/**
 * @brief Log file to display message in SD Card.
 */
uint8_t LogToSDCard(const char *tag, const char *message);

/**
 * @briefUnmout SD Card after all functionality is completed.
 */
void SdCardUnmount(void);

#endif /* MAIN_FILE_H_ */