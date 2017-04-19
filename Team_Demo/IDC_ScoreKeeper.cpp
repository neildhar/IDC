/*
  IDC_ScoreKeeper.cpp
  Created by Neil Dhar, 30 March, 2017.
*/
#include <Arduino.h>
#include "IDC_ScoreKeeper.h"

IDC_ScoreKeeper::IDC_ScoreKeeper(Stream* _XBee,int _id){
	myID = _id;
	XBee = _XBee;
}

void IDC_ScoreKeeper::set(int pos, bool val){
    if(val) //if object is present
		states[myID] |= (1<<pos); //set bit at position to 1
    else
		states[myID] &= ~(1<<pos); //set bit at position to 0
}

byte IDC_ScoreKeeper::getState(int byteID){
	return states[byteID];
}


void IDC_ScoreKeeper::sendByte(){
	XBee ->write((myID<<5)|states[myID]); //send tagged state (tag is highest 3 bits)
}

void IDC_ScoreKeeper::update(){
	while(XBee ->available()){
		byte incoming = XBee -> read(); //read incoming byte
		byte state = incoming & 0b00011111; //get bottom 5 bits (state) of incoming byte
		bool valid = true; //assume data is valid
		byte id = incoming>>5; //get id of incoming byte

		if(id<5){
			stateBuffer[id][stateBufferIndex[id]] = state; //append latest state to buffer of states
			stateBufferIndex[id] = (stateBufferIndex[id]+1)%BUFFER_SIZE; //update index in state buffer

			for(int i=0; i<BUFFER_SIZE;i++) //check to ensure all data in buffer is the same
				if(stateBuffer[id][i]!=stateBuffer[id][0])
					valid = false;

			if(valid) //if all data is the same (i.e. same robot has sent same state enough times to fill its buffer), update the state
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
	score += this->pairScore(1);
	score += this->pairScore(2);
	score += this->pairScore(3);
	return score;
}

int IDC_ScoreKeeper::pairScore(int pair){
    if(pair == 1)
        return (countOnes(states[0]&states[1])*10); //if beater/chaser 1
    else if(pair == 2)
        return (countOnes(states[2]&states[3])*10); //if beater/chaser 2
    else if (pair == 3)
        return (((bool)states[4])*150); //if seeker
    return 0; //if errror
}

int IDC_ScoreKeeper::pairScore(){
    if(myID<2)
        return this->pairScore(1);
    else if(myID<4)
        return this->pairScore(2);
    return this->pairScore(3);

}
