/*
  IDC_ScoreKeeper.h
  Created by Neil Dhar, 30 March, 2017.
*/

#ifndef IDC_ScoreKeeper_h
#define IDC_ScoreKeeper_h

#include <Arduino.h>

  class IDC_ScoreKeeper{
    private:
      int myCurByte = 0;
      Stream& XBee;
      int myID;
      byte states[5];
    public:
      IDC_ScoreKeeper(Stream&, int);
      void set(int, bool);
      byte getState(int);
      void sendByte();
      void update();
      int getScore();
      int countOnes(byte);
  };

#endif
