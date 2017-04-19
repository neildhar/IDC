/*
  IDC_ScoreKeeper.h
  Created by Neil Dhar, 30 March, 2017.
*/

#ifndef IDC_ScoreKeeper_h
#define IDC_ScoreKeeper_h

#include <Arduino.h>

  class IDC_ScoreKeeper{
    private:
      const int BUFFER_SIZE = 5;
      Stream* XBee;
      int myID;
      byte states[5];
      byte stateBuffer[5][BUFFER_SIZE];
      int stateBufferIndex[5];
    public:
      IDC_ScoreKeeper(Stream*, int);
      void set(int, bool);
      byte getState(int);
      void sendByte();
      void update();
      int getScore();
      int countOnes(byte);
      void setState(int, byte); //for testing only
      int pairScore(int);
      int pairScore();
  };

#endif
