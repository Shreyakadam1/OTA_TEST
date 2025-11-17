/**
 * @file mlx90614_Handler.h
 * @brief Interface for MLX90614 Temperature Sensor using I2C
 */
#include <esp_err.h>
#include <driver/i2c.h>
#include <esp_log.h>

/// I2C port used for MLX90614 communication
#define MLX90614_I2C_PORT       I2C_NUM_0

/// GPIO pin number for I2C SCL line
#define MLX90614_SCL_GPIO       16

/// GPIO pin number for I2C SDA line
#define MLX90614_SDA_GPIO       15

/// I2C clock frequency for MLX90614
#define MLX90614_I2C_FREQ_HZ    100000

/// I2C communication timeout in milliseconds
#define I2C_TIMEOUT_MS          1000

/// 7-bit I2C address of MLX90614 sensor
#define MLX90614_ADDR           0x5A

/// Register address for reading object temperature 
#define MLX90614_REG_ADDR       0x07

/// Scale factor for temperature conversion (0.02 °K/LSB)
#define SCALE_FACTOR            0.02f       

/// Constant to convert Kelvin to Celsius
#define KELVIN_OFFSET           273.15f 

/// Constant to convert Kelvin to fahrenheit
#define FAHRENHEIT_OFFSET  		32.0f

#define FAHRENHEIT_MULTIPLIER  	1.8f

/// Polynomial used for CRC-8 (PEC) calculation
#define CRC8_POLYNOMIAL         0x07  

#define RETURN_NUM_MLX90614_READSUCCESSFULLY	0

#define RETURN_NUM_MLX90614_READFAIL 	1

#define RETURN_NUM_MLX90614_INITSUCCESSFULLY	2

#define RETURN_NUM_MLX90614_INITFAIL 	3

#define num1	4

#define num2 	5



/**
 * @brief Initialize the I2C interface for the MLX90614 sensor.
 * 
 * @return 
 * - ESP_OK on success
 * - ESP_ERR_INVALID_ARG, ESP_FAIL, or other error codes from I2C driver on failure
 */
uint8_t MLX90614I2CInit(void);

/**
 * @brief Reads temperature data from the MLX90614 sensor.
 *
 * @param[out] tempcelsius    Pointer to store the temperature in Celsius.
 * @param[out] tempfahrenheit Pointer to store the temperature in Fahrenheit.
 *
 * @return ESP_OK if successful, otherwise an error code.
 */
uint8_t MLX90614ReadTemp(float *tempcelsius, float *tempfahrenheit);

/**
 * @brief Calculates 8-bit CRC (PEC) for I2C data integrity verification.
 *
 * Implements CRC-8 calculation used by MLX90614 to verify packet error code (PEC).
 *
 * @param[in] pecbuffer  Pointer to data array over which CRC is to be calculated.
 * @param[in] dataLength Length of the data in the buffer.
 *
 * @return 8-bit CRC value.
 */
uint8_t CalculateCrc8(uint8_t *pecbuffer, uint8_t dataLength);