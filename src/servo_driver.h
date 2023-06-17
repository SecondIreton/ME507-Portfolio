/*
 * servo_driver.h
 *
 *  Created on: May 30, 2023
 *      Author: Trevor Foley
 */

#include "stdint.h"
#include "stm32f4xx_hal.h"

#ifndef INC_SERVO_DRIVER_H_
#define INC_SERVO_DRIVER_H_

typedef struct servo *ServoPtr;
typedef struct servo {
	int32_t position;
	uint32_t drive_channel;
	TIM_HandleTypeDef *htim;
} Servo;

void ConstructServo(ServoPtr servo, TIM_HandleTypeDef *htim, uint32_t drive_channel);

void GoToPosition(ServoPtr servo, int32_t position);

#endif /* INC_SERVO_DRIVER_H_ */
