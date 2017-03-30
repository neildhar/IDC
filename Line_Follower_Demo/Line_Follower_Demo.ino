#define lPin 10
#define cPin 5
#define rPin 4

#define QTIThreshold 200

#include "IDC_ServoMotorDriver.h"

IDC_ServoMotorDriver rMotor(13, true);
IDC_ServoMotorDriver lMotor(12, false);

void setup() {
  Serial.begin(9600);
  lMotor.initialise();
  rMotor.initialise();
}
bool onHash = false;
long lQTI, rQTI, cQTI;

void loop() {
  lQTI = RCtime(lPin);
  rQTI = RCtime(rPin);
  //cQTI = RCtime(cPin);

  //Serial.print(lQTI); Serial.print(" "); Serial.print(cQTI); Serial.print(" "); Serial.println(rQTI);

  if(lQTI<QTIThreshold && rQTI<QTIThreshold){
    onHash = false;
    lMotor.run(100);
    rMotor.run(100);
  }
  else if(lQTI>QTIThreshold && rQTI<QTIThreshold){
    onHash = false;
    lMotor.run(-150);
    rMotor.run(150);
  }

  else if(lQTI<QTIThreshold && rQTI>QTIThreshold){
    onHash = false;
    lMotor.run(150);
    rMotor.run(-150);
  }
  else if(lQTI>QTIThreshold && rQTI>QTIThreshold){
    if(!onHash){
        onHash = true;
        lMotor.run(0);
        rMotor.run(0);
        delay (1000);
    }
    else{
        lMotor.run(150);
        rMotor.run(150);
    }
  }
  

}

long RCtime(int sensPin){
   
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(2);                       // wait 2 ms to make sure cap is discharged
   
   pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
   long startTime = micros();
   while(digitalRead(sensPin));   // wait for pin to go low
   long result  = micros()-startTime;
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   return result;                   // report results   
}

