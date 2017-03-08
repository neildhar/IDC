#include "IDC_ServoMotorDriver.h"

IDC_ServoMotorDriver lMotor(13, false);
IDC_ServoMotorDriver rMotor(12, true);
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
