/*
 * mecanum_driver.c
 *
 *  Created on: Jun 14, 2023
 *      Author: Trevor Foley
 */

#include "mecanum_driver.h"

void ConstructChassis(ChassisPtr chassis, MotorPtr motor1, MotorPtr motor2, MotorPtr motor3, MotorPtr motor4)
{
	chassis->front_right = motor1;
	chassis->back_right = motor2;
	chassis->front_left = motor3;
	chassis->back_left = motor4;
}

void EnableChassis(ChassisPtr chassis)
{
	EnableMotor(chassis->front_right);
	EnableMotor(chassis->front_left);
	EnableMotor(chassis->back_left);
	EnableMotor(chassis->back_right);
}

void CrippleChassis(ChassisPtr chassis)
{
	CrippleMotor(chassis->front_right);
	CrippleMotor(chassis->front_left);
	CrippleMotor(chassis->back_left);
	CrippleMotor(chassis->back_right);
}

void GoForward(ChassisPtr chassis)
{
	SetDutyCycle(chassis->front_right, 35);
	SetDutyCycle(chassis->front_left, 35);
	SetDutyCycle(chassis->back_left, 35);
	SetDutyCycle(chassis->back_right, 35);
}

void GoBackward(ChassisPtr chassis)
{
	SetDutyCycle(chassis->front_right, -35);
	SetDutyCycle(chassis->front_left, -35);
	SetDutyCycle(chassis->back_left, -35);
	SetDutyCycle(chassis->back_right, -35);
}

void GoLeft(ChassisPtr chassis)
{
	SetDutyCycle(chassis->front_right, 35);
	SetDutyCycle(chassis->front_left, -35);
	SetDutyCycle(chassis->back_left, 35);
	SetDutyCycle(chassis->back_right, -35);
}

void GoRight(ChassisPtr chassis)
{
	SetDutyCycle(chassis->front_right, -35);
	SetDutyCycle(chassis->front_left, 35);
	SetDutyCycle(chassis->back_left, -35);
	SetDutyCycle(chassis->back_right, 35);
}
