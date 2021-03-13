/*! @file : sdk_hal_gpio.c
 * @author  Alvaro Augusto Narvaez Yepez -- Willis Jose Jimenez Naranjo
 * @version 1.0.0
 * @date    7 mar. 2021
 * @brief   Funcion principal main
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"

#include "sdk_hal_uart0.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_i2c1.h"
#include "sdk_pph_bme280.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HABILITAR_SENSOR_BME280		1

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


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/



void waytTime(uint32_t tiempo) {
	//uint32_t tiempo = 0x1FFFF;
	do {
		tiempo--;
	} while (tiempo != 0x0000);
}
/*
 * @brief   Application entry point.
 */
int main(void) {


	bme280_data_t bme280_datos;
	uint8_t bme280_detectado = 0;
	uint8_t bme280_base_de_tiempo = 0;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    printf("Inicializa UART0:");
    //inicializa puerto UART0 y solo avanza si es exitoso el proceso
    if(uart0Inicializar(115200)!=kStatus_Success){	//115200bps
    	printf("Error");
    	return 0 ;
    };

    printf("Inicializa I2C1:");
    //inicializa puerto I2C1 y solo avanza si es exitoso el proceso
    if(i2c1MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");

#if HABILITAR_SENSOR_BME280
    printf("Detectando BME280:");
    //LLamado a funcion que identifica sensor BME280
    if (bme280WhoAmI() == kStatus_Success){
    	printf("OK\r\n");
    	(void)bme280Init();	//inicializa sensor bme280
    	bme280_detectado = 1;	//activa bandera que indica (SI HAY BME280)
    }
#endif


    while(1) {

    	waytTime(2000000);

    	#if HABILITAR_SENSOR_BME280
    	if(bme280_detectado == 1){
    		bme280_base_de_tiempo ++;	//incrementa base de tiempo para tomar dato bme280
    		if(bme280_base_de_tiempo > 10){	//	>10 equivale aproximadamente a 2s
    			bme280_base_de_tiempo = 0; //reinicia contador de tiempo
    			if(bme280ReadData(&bme280_datos) == kStatus_Success){	//toma lectura humedad, presion, temperatura
        			printf("BME280 ->");
    				printf("temperatura:0x%X ",bme280_datos.temperatura);	//imprime temperatura sin procesar
        			printf("humedad:0x%X ",bme280_datos.humedad);	//imprime humedad sin procesar
        			printf("presion:0x%X ",bme280_datos.presion);	//imprime presion sin procesar
        			printf("\r\n");	//Imprime cambio de linea
    			}
    		}
    	}
       #endif
    }
    return 0 ;
}
