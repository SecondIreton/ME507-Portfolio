/*
 * mecanum_driver.h
 *
 *  Created on: Jun 14, 2023
 *      Author: Trevor Foley
 */

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "motor_driver.h"

#ifndef INC_MECANUM_DRIVER_H_
#define INC_MECANUM_DRIVER_H_

typedef struct chassis *ChassisPtr;
typedef struct chassis {
	MotorPtr front_right;
	MotorPtr front_left;
	MotorPtr back_left;
	MotorPtr back_right;
} Chassis;

void ConstructChassis(ChassisPtr chassis, MotorPtr motor1, MotorPtr motor2, MotorPtr motor3, MotorPtr motor4);

void EnableChassis(ChassisPtr chassis);

void CrippleChassis(ChassisPtr chassis);

void GoForward(ChassisPtr chassis);

void GoBackward(ChassisPtr chassis);

void GoLeft(ChassisPtr chassis);

void GoRight(ChassisPtr chassis);

#endif /* INC_MECANUM_DRIVER_H_ */
