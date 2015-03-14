/* 
 * File:   dht11.h
 * Author: Toanpv
 *
 * Created on June 1, 2014, 4:28 PM
 */

#ifndef DHT11_H
#define	DHT11_H

#include "board.h"

#define DHT11_DATA	RC2
#define DHT11_TRIS	TRISC2

#define DHT_ER       0
#define DHT_OK       1

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t dht_get_tem_humi (uint8_t *tem,uint8_t *humi);

#ifdef	__cplusplus
}
#endif

#endif	/* DHT11_H */

