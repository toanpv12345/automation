/* 
 * File:   types.h
 * Author: Ken
 *
 * Created on August 22, 2012, 4:14 PM
 */
#ifndef TYPES_H
#define	TYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef bit                 bool;
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef int                 int16_t;
typedef unsigned int        uint16_t;
typedef short long          int24_t;
typedef unsigned short long uint24_t;
typedef long                int32_t;
typedef unsigned long       uint32_t;

#ifndef true
#define true    1
#endif

#ifndef false
#define false   0
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* TYPES_H */

