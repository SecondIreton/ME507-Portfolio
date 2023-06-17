/*
 * servo_driver.c
 *
 *  Created on: May 30, 2023
 *      Author: Trevor Foley
 */

#include "servo_driver.h"

void ConstructServo(ServoPtr servo, TIM_HandleTypeDef *htim, uint32_t drive_channel)
{
	servo->htim = htim;
	servo->drive_channel = drive_channel;
	GoToPosition(servo, 0);
}

void GoToPosition(ServoPtr servo, int32_t position)
{
	__HAL_TIM_SET_COMPARE(servo->htim, servo->drive_channel, (int)(position/3.6 + 50));
}
