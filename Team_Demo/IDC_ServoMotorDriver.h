/*
  IDC_ServoMotorDriver.h
  Created by Neil Dhar, 8 March, 2017.
*/

#ifndef IDC_ServoMotorDriver_h
#define IDC_ServoMotorDriver_h

#include <Arduino.h>
#include "Motor.h"
#include <Servo.h>

  class IDC_ServoMotorDriver: public Motor{
    private:
      Servo* servo;
      int pin;
      bool flip;
    public:
      IDC_ServoMotorDriver(int);
      IDC_ServoMotorDriver(int,bool);
      ~IDC_ServoMotorDriver();
      void initialise();
      void run(int);
      void disconnect();
  };

#endif
