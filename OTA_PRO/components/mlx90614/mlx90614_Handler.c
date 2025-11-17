/**
 * @file mlx90614_Handler.c
 * @author    
 *        Shreya Kadam 
 * @brief Driver implementation for MLX90614 temperature sensor using I2C on ESP32-S3.
 *
 * This driver handles initialization, reading object temperature, and CRC-8 PEC validation
 * for the MLX90614 infrared temperature sensor. It communicates over the I2C protocol.
 *
 */
#include "mlx90614_Handler.h"


/**
 * @brief Initializes the I2C bus for communication with MLX90614.
 *
 * Configures the I2C master interface using the GPIOs and frequency defined in mlx90614.h.
 *
 * @return
 *      - RETURN_NUM_MLX90614_INITSUCCESSFULLY on successful initialization.
 *      - RETURN_NUM_MLX90614_INITFAIL or appropriate error code from i2c_param_config or i2c_driver_install.
 */
uint8_t MLX90614I2CInit(void)
{
	i2c_config_t conf =	{
	.mode = I2C_MODE_MASTER,
	.sda_io_num = MLX90614_SDA_GPIO,
	.scl_io_num = MLX90614_SCL_GPIO,
	.sda_pullup_en = GPIO_PULLUP_ENABLE,
	.scl_pullup_en = GPIO_PULLUP_ENABLE,
	.master.clk_speed = MLX90614_I2C_FREQ_HZ,
	};
	esp_err_t err = i2c_param_config(MLX90614_I2C_PORT, &conf);
	
	if (err != ESP_OK){ 
		return RETURN_NUM_MLX90614_INITFAIL;
	}
	
	if(i2c_driver_install(MLX90614_I2C_PORT, conf.mode, 0, 0, 0) == ESP_OK)
		return RETURN_NUM_MLX90614_INITSUCCESSFULLY;
	else
		return RETURN_NUM_MLX90614_INITFAIL; 
}


 /**
 * @brief Reads the object temperature from MLX90614.
 *
 * This function communicates with the MLX90614 over I2C to read the raw temperature data,
 * verifies the data integrity using PEC (Packet Error Code), and converts the reading into
 * Celsius and Fahrenheit.
 *
 * @param[out] tempcelsius     Pointer to store the measured temperature in degrees Celsius.
 * @param[out] tempfahrenheit  Pointer to store the measured temperature in degrees Fahrenheit.
 *
 * @return
 *     - RETURN_NUM_MLX90614_READSUCCESSFULLY:      Temperature read successfully.
 *     - RETURN_NUM_MLX90614_READFAIL:              PEC verification failed (data corruption detected).
 */

uint8_t MLX90614ReadTemp(float *tempcelsius,float *tempfahrenheit) 
{
	uint8_t lsb, msb, pec;
	uint8_t pecbuffer[6];  
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (MLX90614_ADDR << 1) | I2C_MASTER_WRITE, true);  
	i2c_master_write_byte(cmd, MLX90614_REG_ADDR, true);                        
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (MLX90614_ADDR << 1) | I2C_MASTER_READ, true);  

	i2c_master_read_byte(cmd, &lsb, I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &msb, I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &pec, I2C_MASTER_NACK);
	i2c_master_stop(cmd);

	esp_err_t ret = i2c_master_cmd_begin(MLX90614_I2C_PORT, cmd, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK){ 
		return RETURN_NUM_MLX90614_READFAIL;
	}

	pecbuffer[0] = (MLX90614_ADDR << 1) | I2C_MASTER_WRITE; 
	pecbuffer[1] = MLX90614_REG_ADDR;
	pecbuffer[2] = (MLX90614_ADDR << 1) | I2C_MASTER_READ;   
	pecbuffer[3] = lsb;
	pecbuffer[4] = msb;

	uint8_t calcpec = CalculateCrc8(pecbuffer, 5);

	if (calcpec != pec)	{
		return RETURN_NUM_MLX90614_READFAIL;  
	}

	uint16_t rawtemp = (msb << 8) | lsb;
	float kelvin = (rawtemp * SCALE_FACTOR);
	*tempcelsius = kelvin - KELVIN_OFFSET;
	*tempfahrenheit = (FAHRENHEIT_MULTIPLIER * (kelvin - KELVIN_OFFSET)) + FAHRENHEIT_OFFSET;
	
	return RETURN_NUM_MLX90614_READSUCCESSFULLY;
 
}


/**
 * @brief Calculates 8-bit CRC (PEC) for I2C data integrity verification.
 *
 * Implements CRC-8 calculation used by MLX90614 to verify packet error code (PEC).
 *
 * @param[in] pecbuffer Pointer to data array over which CRC is to be calculated.
 * @param[in] dataLength Length of the data in the buffer.
 *
 * @return 8-bit CRC value.
 */
uint8_t CalculateCrc8(uint8_t *pecbuffer, uint8_t dataLength)
{
	uint8_t crc = 0x00;
	for (uint8_t byteIndex = 0; byteIndex < dataLength; byteIndex++) {
		crc ^= pecbuffer[byteIndex];
		for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
			if (crc & 0x80){
				crc = (crc << 1) ^ CRC8_POLYNOMIAL;
			}
			else {
				crc <<= 1;
			}
		}
	}
	return crc;
}