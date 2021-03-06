/*
 * sdk_pph_sth31_envio.h
 * @author  Alvaro Augusto Narvaez Yepez -- Willis Jose Jimenez Naranjo
 * @version 1.0.0
 * @date    1/03/2021
 *      Author: hp
 */

#ifndef SDK_PPH_STH31_ENVIO_H_
#define SDK_PPH_STH31_ENVIO_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_hal_uart0.h"

/*!
 * @addtogroup PPH
 * @{
 */
/*!
 * @addtogroup EC25AU
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/


enum _comandos_at_topic_sht3x {
	kAT_QMTSUB_T_L = 0,
	kAT_QMTPUB_DATO_S,
	kAT_QMTSUB_H_L,
	kAT_QMTPUB_H_L,
	kAT_QMTSUB_T_H,
	kAT_QMTPUB_T_H,
	kAT_QMTSUB_H_H,
	kAT_QMTPUB_H_H,
};

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
void waytTimeModem(uint32_t tiempo);

status_t enviarMQTT_ValueSHT3x(void);

/** @} */ // end of X group
/** @} */ // end of X group

#endif /* SDK_PPH_STH31_ENVIO_H_ */
