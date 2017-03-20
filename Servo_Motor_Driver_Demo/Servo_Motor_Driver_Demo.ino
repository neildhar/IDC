#include "IDC_ServoMotorDriver.h"

IDC_ServoMotorDriver rMotor(13, true);
IDC_ServoMotorDriver lMotor(12, false);
void setup() {
lMotor.initialise();
rMotor.initialise();
}

void loop() {
    for(int i=-200;i<200;i++){
        lMotor.run(i);
        rMotor.run(i);
        delay(50);
    }

    for(int i=200;i>-200;i--){
        lMotor.run(i);
        rMotor.run(i);
        delay(50);
    }
}
