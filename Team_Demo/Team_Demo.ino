#define lPin 5
#define rPin 4

#define gLEDPin 8

#define RFIDEnablePin  7
#define RFID_START  0x0A
#define RFID_STOP   0x0D

#define XBeeSerial Serial1
#define RFIDSerial Serial2
#define LCDSerial Serial3

#define QTIThreshold 200

#include "IDC_ServoMotorDriver.h"
#include "IDC_ScoreKeeper.h"

IDC_ServoMotorDriver rMotor(13, true);
IDC_ServoMotorDriver lMotor(12, false);

IDC_ScoreKeeper ScoreKeeper(&XBeeSerial, 1); //initialise as beater 1

void setup() {
  XBeeSerial.begin(9600); //initialise XBee
  pinMode(gLEDPin, OUTPUT); //set send LED pin mode
  
  Serial.begin(9600);

  //initialise motors
  lMotor.initialise();
  rMotor.initialise();
  
  pinMode(RFIDEnablePin, OUTPUT);
  digitalWrite(RFIDEnablePin, HIGH);
  RFIDSerial.begin(2400); //initialise RFID reader serial
  LCDSerial.begin(2400);
}

bool onHash = false;
bool hasObject = false;
int hashCount = 0;
long lastXBeeRec=0; /*time of last received XBee byte*/
char strbuf[10];
long lQTI, rQTI;

void loop() {
  hashCount = 0;
  while(true){
      //COMMUNICATION
      ScoreKeeper.update();
      
     if(hashCount==5){ //if five hashes, back up
        lMotor.run(-150);
        rMotor.run(-150);
        delay(1000);
        hashCount++;
     }
     else if(hashCount == 6){ //program has ended
        lMotor.run(0);
        rMotor.run(0);
        delay(200); //wait for robot to completely stop
        lMotor.disconnect();
        rMotor.disconnect();
        
        LCDSerial.write(16); //turn on LCD
        LCDSerial.write(12); //clear lcd
        delay(5);
        LCDSerial.write(17); //enable backlight
      
        LCDSerial.print(itoa(ScoreKeeper.getScore(), strbuf, 10)); //print score
        LCDSerial.print("\n");
        LCDSerial.print(itoa(ScoreKeeper.pairScore(), strbuf, 10));
        continue; //skip line following
     }
      //LINE FOLLOWING
      //Read QTIs
      lQTI = RCtime(lPin);
      rQTI = RCtime(rPin);
    
      //Serial.print(lQTI); Serial.print(" "); Serial.println(rQTI);
    
      //if both white, keep going forward
      if(lQTI<QTIThreshold && rQTI<QTIThreshold){
        onHash = false;
        lMotor.run(100);
        rMotor.run(100);
      }
    
      //if left QTI is on the line, turn left
      else if(lQTI>QTIThreshold && rQTI<QTIThreshold){
        onHash = false;
        lMotor.run(-150);
        rMotor.run(150);
      }
    
      //if right QTI is on the line, turn right
      else if(lQTI<QTIThreshold && rQTI>QTIThreshold){
        onHash = false;
        lMotor.run(150);
        rMotor.run(-150);
      }
    
      //if both QTIs are on the line, you are at a hash mark
      else if(lQTI>QTIThreshold && rQTI>QTIThreshold){
        //if detecting this hashmark for the first time, stop and detect object
        if(!onHash){
            onHash = true;
            hashCount++;
            lMotor.run(0);
            rMotor.run(0);
            delay(200); //wait for robot to completely stop
            lMotor.disconnect(); //cut power to motors to save power and reduce jitter
            rMotor.disconnect();
    
            hasObject = false; //assume there is no object there
            
            /*SENSING CODE BEGINS HERE*/
            while(RFIDSerial.available()) //flush old data
                RFIDSerial.read();
            digitalWrite(RFIDEnablePin, LOW); //enable RFID reader
            
             for(int i=0;i<1500;i++){ //give the reader some time to get data
                if(RFIDSerial.available()) break;
                delay(1);
             }
            digitalWrite(RFIDEnablePin, HIGH); //disable RFID reader
            while(RFIDSerial.available()){
                if(RFIDSerial.read() == RFID_START){ //if there is valid incoming data, object has been detected
                    hasObject = true;
                }
            }
            /*SENSING CODE ENDS HERE*/
            ScoreKeeper.set(hashCount-1, hasObject);
            
            if(hasObject){
                digitalWrite(gLEDPin, hasObject); //light up the green LED if object is there
                delay (1000);
                digitalWrite(gLEDPin, false);
            }
            lMotor.initialise();
            rMotor.initialise();

            lMotor.run(150); //compensate for jitter
            rMotor.run(150);
            delay(200);
            
        }
    
        //if was on hash mark in previous cycle, keep moving until off hash mark
        else{
            lMotor.run(150);
            rMotor.run(150);
        }
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

