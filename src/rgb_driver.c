/*
 * rgb_driver.c
 *
 *  Created on: May 30, 2023
 *      Author: Trevor Foley
 */

#include "rgb_driver.h"

void ConstructRGBSensor(RGBPtr sensor, I2C_HandleTypeDef *hi2c)
{
	sensor->hi2c = hi2c;
	SetControlRegister(sensor, 0x00, 0x01);
}

void SetControlRegister(RGBPtr sensor, uint8_t reg, uint8_t value)
{
	uint8_t data = value;
	HAL_I2C_Mem_Write(sensor->hi2c, (uint16_t)(0x29<<1), 0x80 | reg, 1, &data, 1, 100);
}

uint8_t ReadControlRegister(RGBPtr sensor, uint8_t reg)
{
	uint8_t data;
	HAL_I2C_Mem_Read(sensor->hi2c, (uint16_t)(0x29<<1), 0x80 | reg, 1, &data, 1, 100);
	return data;
}

uint16_t ReadColorRegister(RGBPtr sensor, uint8_t reg)
{
	uint16_t datal, datah;
	uint8_t data[2];
	HAL_I2C_Mem_Read(sensor->hi2c, (uint16_t)(0x29<<1), 0x80 | reg, 1, data, 2, 100);
	datal = (uint16_t) data[0];
	datah = (uint16_t) data[1];
	datah <<= 8;
	return datah | datal;
}

void GetRawColors(RGBPtr sensor, uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear)
{
	*red = ReadColorRegister(sensor, 0x16);
	*green = ReadColorRegister(sensor, 0x18);
	*blue = ReadColorRegister(sensor, 0x1A);
	*clear = ReadColorRegister(sensor, 0x14);
	HAL_Delay((256 - 0xEB) * 12 / 5 + 1);
}

void GetColors(RGBPtr sensor, uint16_t* r, uint16_t* g, uint16_t* b)
{
	uint16_t red, green, blue, clear;
	GetRawColors(sensor, &red, &green, &blue, &clear);
	uint32_t sum = clear;

	if (clear == 0) {
		*r = *g = *b = 0;
		return;
	}

	*r = red*255/sum;
	*g = green*255/sum;
	*b = blue*255/sum;
}


