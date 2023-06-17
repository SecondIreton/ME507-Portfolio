/*
 * motor_driver.c
 *
 *  Created on: Apr 27, 2023
 *      Author: Trevor Foley
 */

#include "motor_driver.h"

void ConstructMotor(MotorPtr motor, TIM_HandleTypeDef *htim, uint32_t forward, uint32_t reverse)
{
	motor->htim = htim;
	motor->forward_channel = forward;
	motor->reverse_channel = reverse;
	SetDutyCycle(motor, 0);
}

void EnableMotor(MotorPtr motor)
{
	HAL_TIM_PWM_Start(motor->htim, motor->forward_channel);
	HAL_TIM_PWM_Start(motor->htim, motor->reverse_channel);
}

void CrippleMotor(MotorPtr motor)
{
	HAL_TIM_PWM_Stop(motor->htim, motor->forward_channel);
	HAL_TIM_PWM_Stop(motor->htim, motor->reverse_channel);
}

void SetDutyCycle(MotorPtr motor, int32_t duty)
{
	if (duty > 0 && duty < 100){
		__HAL_TIM_SET_COMPARE(motor->htim, motor->forward_channel, duty*47);
		HAL_TIM_PWM_Stop(motor->htim, motor->reverse_channel);
	}
	else if (duty < 0 && duty > -100){
		__HAL_TIM_SET_COMPARE(motor->htim, motor->reverse_channel, duty*-47);
		HAL_TIM_PWM_Stop(motor->htim, motor->forward_channel);
	}
	else {
		HAL_TIM_PWM_Stop(motor->htim, motor->reverse_channel);
		HAL_TIM_PWM_Stop(motor->htim, motor->forward_channel);
	}
}


