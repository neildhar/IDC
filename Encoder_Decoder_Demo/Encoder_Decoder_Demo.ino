#include "IDC_ScoreKeeper.h"

IDC_ScoreKeeper test(&Serial3, 1);

void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);

}

void loop() {
  test.set(0, true);
  test.set(2, true);
  test.setState(0, 0b00001);
  
  
  Serial.println(test.getState(0), BIN);
  Serial.println(test.getState(1), BIN);
  Serial.println(test.getScore());
  test.update();

  delay(200);
}
