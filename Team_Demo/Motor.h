/*
  Motor.h - Abstract motor driver class
  Created by Neil Dhar, 10 December, 2016.
*/

#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

  class Motor{
    public:
      virtual void run(int);
      virtual void initialise();
  };

#endif
