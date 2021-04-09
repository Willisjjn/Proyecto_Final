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
#include "sdk_mdlw_leds.h"
#include "sdk_pph_ec25au.h"
#include "sdk_pph_sht3x.h"
#include "sdk_pph_sth31_envio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HABILITAR_SENSOR_BME280		1
#define HABILITAR_MODEM_EC25		1
#define HABILITAR_SENSOR_SHT3X		1

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
	//uint32_t tiempo = 0xFFFFF;
	do {
		tiempo--;
	} while (tiempo != 0x0000);
}
/*
 * @brief   Application entry point.
 */
int main(void) {

	uint8_t ec25_mensaje_de_texto[]="Hola desde EC25";
	uint8_t ec25_estado_actual ;
	uint8_t ec25_detectado=0;

	bme280_data_t bme280_datos;
	uint8_t bme280_detectado = 0;
	uint8_t bme280_base_de_tiempo = 0;

	uint8_t sht3x_base_de_tiempo=0;
	sht3x_data_t sht3x_datos;
	uint8_t sht3x_detectado=0;
	uint8_t sht3x_dato_salida=0;

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
    }
    printf("OK\r\n");

    printf("Inicializa I2C1:");
    //inicializa puerto I2C1 y solo avanza si es exitoso el proceso
    if(i2c1MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");

#if HABILITAR_SENSOR_SHT3X
    printf("Detectando SHT3X:");
    //LLamado a funcion que identifica sensor SHT3X
    if(sht3xInit()== kStatus_Success){
    	sht3x_detectado=1;
    	printf("OK\r\n");
    }
#endif

#if HABILITAR_SENSOR_BME280
    printf("Detectando BME280:");
    //LLamado a funcion que identifica sensor BME280
    if (bme280WhoAmI() == kStatus_Success){
    	printf("OK\r\n");
    	(void)bme280Init();	//inicializa sensor bme280
    	bme280_detectado = 1;	//activa bandera que indica (SI HAY BME280)
    }
#endif

#if HABILITAR_MODEM_EC25
    //Inicializa todas las funciones necesarias para trabajar con el modem EC25
    printf("Inicializa modem EC25\r\n");
    ec25Inicializacion();

    //Configura FSM de modem para enviar mensaje de texto
    printf("Enviando mensaje de texto por modem EC25\r\n");
    ec25EnviarMensajeDeTexto(&ec25_mensaje_de_texto[0], sizeof(ec25_mensaje_de_texto));
#endif


    while(1) {

    	waytTime(1000000);

	#if HABILITAR_SENSOR_SHT3X
    	if(sht3x_detectado==1){
    		sht3x_base_de_tiempo++; //incrementa base de tiempo para tomar dato sensor SHT3X
			if(sht3x_base_de_tiempo>10){//	>10 equivale aproximadamente a 2s
				sht3x_base_de_tiempo=0; //reinicia contador de tiempo
	    		if (sht3xReadData(&sht3x_datos) == kStatus_Success) {//toma lectura humedad, temperatura
	    			printf("SHT3X ->");
	    			//printf("temperatura:0x%X ",sht3x_datos.temperatura);	//imprime temperatura sin procesar
	    			//printf("CRC8_t:0x%X ",sht3x_datos.crc_temperatura);	//imprime CRC8 de temperatura
        			//printf("humedad:0x%X ",sht3x_datos.humedad);	//imprime humedad sin procesar
        			//printf("CRC8_h:0x%X ",sht3x_datos.crc_humedad);	//imprime CRC8 de temperatura
           			printf("temperatura:       %.2f     ",(float)(-45.0+175.0*(sht3x_datos.temperatura/65535.0)));    //imprime temperatura
        			//printf("CRC8_t:         0x%X               ",sht3x_datos.crc_temperatura);    //imprime CRC8 de temperatura
        			printf("humedad:         %.2f  %%   ",(float)(100.0*(sht3x_datos.humedad/65535.0)));    //imprime humedad
        			//printf("CRC8_h:         0x%X             ",sht3x_datos.crc_humedad);    //imprime CRC8 de temperatura

        			printf("\r\n");	//Imprime cambio de linea
	    		}
			}
    	}
	#endif

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

	#if HABILITAR_MODEM_EC25
    	ec25_estado_actual = ec25Polling();	//actualiza maquina de estados encargada de avanzar en el proceso interno del MODEM
											//retorna el estado actual de la FSM

		switch(ec25_estado_actual){
    	case kFSM_RESULTADO_ERROR:
    		toggleLedRojo();
    		apagarLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_EXITOSO:
    		apagarLedRojo();
    		toggleLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_ERROR_RSSI:
    		toggleLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;

    	default:
    		apagarLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;
    	}
	#endif

		if (ec25_estado_actual==kFSM_RESULTADO_EXITOSO){
			while(kFSM_RESULTADO_EXITOSO){
			enviarMQTT_ValueSHT3x();
			waytTime(10000000);
			}
		}
    }
    return 0 ;
}
