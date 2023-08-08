/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdio.h>

#include "x_ram_checks.h"
#include "bsp_util.h"

#include "stm32g0xx_hal.h"
#include "microej.h"


#pragma section = "RWDATA"
#pragma section = "HEAP"
#pragma section = "CSTACK"



#define RAM_START_ADDR    ((uint32_t)&__ICFEDIT_region_RAM_start__ + (__section_size("RWDATA")) + (__section_size("HEAP")) + (__section_size("CSTACK"))) // unused memory start area
#define RAM_END_ADDR      ((uint32_t)&__ICFEDIT_region_RAM_end__)

extern const volatile unsigned int __ICFEDIT_region_RAM_start__;
extern const volatile unsigned int __ICFEDIT_region_RAM_end__;


/* Define the RAM zone structure on which the tests will run */
static  X_RAM_CHECKS_zone_t RAM_zones[] = {
{
	.start_address = 0,
	.end_address = 0,
	.write_speed = 0,
	.read_speed = 0,
	.transfert_speed = 0
}

};

/**
 *
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
        RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
        return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
        RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
        RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return number of zones to test
 */
uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;	
        return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
 	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
  	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
  	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
  	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
  	RAM_zones[0].start_address = RAM_START_ADDR;
        RAM_zones[0].end_address = RAM_END_ADDR;
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}
