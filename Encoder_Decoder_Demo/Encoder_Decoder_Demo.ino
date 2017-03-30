#include "IDC_ScoreKeeper.h"

IDC_ScoreKeeper test(Serial3, 0);

void setup() {
  Serial3.begin(9600);

}

void loop() {
  test.set(0, true);
  test.update();

}
