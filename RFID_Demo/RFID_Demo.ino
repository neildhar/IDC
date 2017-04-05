#define RFIDEnablePin  7
#define RFID_START  0x0A
#define RFID_STOP   0x0D
#define gLEDPin 8
#define RFIDSerial Serial2

void setup(){
  pinMode(RFIDEnablePin, OUTPUT);
  digitalWrite(RFIDEnablePin, HIGH);
  Serial.begin(9600);
  RFIDSerial.begin(2400);
  pinMode(gLEDPin, OUTPUT);
}

void loop(){
  digitalWrite(RFIDEnablePin, LOW);   // enable the RFID Reader

  bool isTag = false;
  while(RFIDSerial.available()) //run until the serial buffer is empty
    if(RFIDSerial.read() == RFID_START)
        isTag = true; //if a valid tag has been read, set the tag flag
  if(isTag){
    digitalWrite(gLEDPin, HIGH); //if a tag is read turn on the green LED
    delay(100);
    digitalWrite(gLEDPin,LOW);
  }
}



