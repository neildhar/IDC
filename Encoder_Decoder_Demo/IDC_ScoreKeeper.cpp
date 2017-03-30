/*
  IDC_ScoreKeeper.cpp
  Created by Neil Dhar, 30 March, 2017.
*/
#include <Arduino.h>
#include "IDC_ScoreKeeper.h"

IDC_ScoreKeeper::IDC_ScoreKeeper(Stream* _XBee,int _id){
	myID = _id;
	myCurByte = _id<<5;
	XBee = _XBee;
}

void IDC_ScoreKeeper::set(int pos, bool val){
    if(val)
			myCurByte |= (1<<pos);
		else
			myCurByte &= ~(1<<pos);

		byte state = myCurByte & 0b00011111;
		states[myID] = state;
}

byte IDC_ScoreKeeper::getState(int byteID){
	return states[byteID];
}


void IDC_ScoreKeeper::sendByte(){
	XBee ->write(myCurByte);
}

void IDC_ScoreKeeper::update(){
	while(XBee ->available()){
		byte incoming = XBee -> read();
		byte state = incoming & 0b00011111;
		bool valid = true;
		int id = incoming>>5;
		if(id<5){
			stateBuffer[id][stateBufferIndex[id]] = state;
			stateBufferIndex[id] = (stateBufferIndex[id]+1)%BUFFER_SIZE;

			for(int i=0; i<BUFFER_SIZE;i++) //check to ensure all data in buffer is the same
				if(stateBuffer[id][i]!=stateBuffer[id][0])
					valid = false;

			if(valid)
				states[id] = state;
		}
	}
	this->sendByte();
}

int IDC_ScoreKeeper::countOnes(byte toCount){
	int count = 0;
	for(int i=0;i<5;i++)
		if(toCount & (1<<i))
			count++;
	return count;
}

void IDC_ScoreKeeper::setState(int id, byte state){
	states[id] = state;
}

int IDC_ScoreKeeper::getScore(){
	int score = 0;
	score += (countOnes(states[0]&states[1])*10);
	score += (countOnes(states[2]&states[3])*10);
	score += (((bool)states[4])*150);
	return score;
}
