/*
 * rgb_driver.h
 *
 *  Created on: May 30, 2023
 *      Author: Trevor Foley
 */

#include "stdint.h"
#include "stm32f4xx_hal.h"

#ifndef INC_RGB_DRIVER_H_
#define INC_RGB_DRIVER_H_

typedef struct rgb_sensor *RGBPtr;
typedef struct rgb_sensor {
	I2C_HandleTypeDef *hi2c;
} RGB;

void ConstructRGBSensor(RGBPtr sensor, I2C_HandleTypeDef *hi2c);

uint16_t ReadColorRegister(RGBPtr sensor, uint8_t reg);

void GetRawColors(RGBPtr sensor, uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear);

void GetColors(RGBPtr sensor, uint16_t* r, uint16_t* g, uint16_t* b);

void SetControlRegister(RGBPtr sensor, uint8_t reg, uint8_t value);

uint8_t ReadControlRegister(RGBPtr sensor, uint8_t reg);

#endif /* INC_RGB_DRIVER_H_ */
