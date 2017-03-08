/*
  IDC_ServoMotorDriver.cpp
  Created by Neil Dhar, 8 March, 2017.
*/
#include <Arduino.h>
#include <Servo.h>
#include "IDC_ServoMotorDriver.h"

IDC_ServoMotorDriver::IDC_ServoMotorDriver(int _pin): pin(_pin){
		servo = new Servo;
		flip = false;
}

IDC_ServoMotorDriver::IDC_ServoMotorDriver(int _pin, bool _flip): pin(_pin){
		servo = new Servo;
		flip = _flip;
}

void IDC_ServoMotorDriver::run(int power){
    power = constrain(power, -200, 200);
    if(flip) servo->writeMicroseconds(1500-power);
    else servo->writeMicroseconds(1500+power);
}

void IDC_ServoMotorDriver::initialise(){
	servo->attach(pin);
}
