/*
 * motor_driver.h
 *
 *  Created on: Apr 27, 2023
 *      Author: Trevor Foley
 */
#include "stdint.h"
#include "stm32f4xx_hal.h"

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_

typedef struct motor *MotorPtr;
typedef struct motor {
	int32_t duty_cycle;
	uint32_t forward_channel;
	uint32_t reverse_channel;
	TIM_HandleTypeDef *htim;
} Motor;

void ConstructMotor(MotorPtr motor, TIM_HandleTypeDef *htim, uint32_t forward, uint32_t reverse);

void EnableMotor(MotorPtr motor);

void CrippleMotor(MotorPtr motor);

void SetDutyCycle(MotorPtr motor, int32_t duty);

#endif /* INC_MOTOR_DRIVER_H_ */
