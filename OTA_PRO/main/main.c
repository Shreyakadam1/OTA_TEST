/*
 * @file app_main.c
 * @brief Application entry point for reading temperature using MLX90614 with ESP32-S3.
 *
 * This file initializes the MLX90614 infrared temperature sensor and continuously
 * reads the temperature value at 1-second intervals. The temperature is printed
 * to console.
 *
 * @author Shreya Kadam
 * @date 2025-08-11
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mlx90614_Handler.h"
#include "SDCardHandler.h"

void app_main(void)
{
	float tempcelsius,tempfahrenheit;
	char  finaldata[100];
    
    const char *MlxFilePath = MOUNT_POINT "/MLX90614.txt";
    uint8_t numstatus = 0;
    
	
	if (InitSdCard() == RETURN_NUM_SDCARD_MOUNTSUCCESSFULLY) 
	{
		printf("SD Card Mount Successfull.\n");
		if (!FileExists(MlxFilePath)) 
		{
			if (CreateFile(MlxFilePath)) {
				printf("File created successfully.\n");
			} 
			else {
				printf("File not created.\n");
			}
		} 
		else {
			printf("File already exists. Continuing with operations.\n");
		}
	}
	else {
		return;
	}
    
	if (MLX90614I2CInit() == RETURN_NUM_MLX90614_INITSUCCESSFULLY) 
	{
		printf("I2C initialized.\n");
		
		vTaskDelay(pdMS_TO_TICKS(300));
		
		while (1)
		{
			if (MLX90614ReadTemp(&tempcelsius,&tempfahrenheit) == RETURN_NUM_MLX90614_READSUCCESSFULLY) 
			{

				sprintf(finaldata,"celsius=%.2f",tempcelsius);
                
				numstatus = AppendFile(MlxFilePath,finaldata);
				if (numstatus == RETURN_NUM_SDCARD_APPENDSUCCESSFULLY) {
					printf("Append Successfull.\n");
				} 
				else {
					printf("Append Fail.\n");
				}
			}
			else
			{
				printf("Failed to Read Temperature.\n");
			}
			vTaskDelay(pdMS_TO_TICKS(1000));
		} 
	}
	else
	{
		printf("Failed to initialize MLX90614.\n");
	}
	return;
}
