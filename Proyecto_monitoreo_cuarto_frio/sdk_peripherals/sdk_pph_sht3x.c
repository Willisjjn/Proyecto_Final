/*! @file : sdk_pph_sht3x.c
 * @author  Alvaro Augusto Narvaez Yepez -- Willis Jose Jimenez Naranjo
 * @version 1.0.0
 * @date    15 mar. 2021
 * @brief   Driver para sensor SHT3X
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_pph_sht3x.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 status_t sht3xSendCommand(uint16_t command_to_send){
 	status_t status;
 	uint8_t i2c_data;
 	uint8_t i2c_addr;

 	i2c_addr =  (uint8_t)(command_to_send>>8);
 	i2c_data = (uint8_t) (command_to_send);

 	status = sht3xWriteByte(&i2c_data, 1, SHT3X_ADDRESS, i2c_addr, 1);

 	return(status);
 }
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 //------------------------------------
status_t sht3xInit(void) {
	status_t status;
	status=sht3xSendCommand(SHT3X_CMD_PERIODIC_MEASUREMENT_WITH_ART);
	return (status);
}
 //------------------------------------
status_t sht3xReadData(sht3x_data_t *data) {
	status_t status;
	uint8_t i2c_data[8];

	status = sht3xReadByte(&i2c_data[0], 6, SHT3X_ADDRESS, SHT3X_CMD_FETCH_DATA, 2);

	if (status == kStatus_Success) {
		data->temperatura = (uint16_t) (i2c_data[0] << 8);
		data->temperatura |= (uint16_t) (i2c_data[1]);
		data->crc_temperatura=i2c_data[2];

		data->humedad = (uint32_t) (i2c_data[3] << 8);
		data->humedad |= (uint32_t) (i2c_data[4]);
		data->crc_humedad=i2c_data[5];

		return (kStatus_Success);
	} else {
		return (status);
	}
}

