#define gLEDPin 8
#define rLEDPin 9
#define buttonPin 6

void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
  pinMode(gLEDPin, OUTPUT); /*set send LED pin mode*/
  pinMode(rLEDPin, OUTPUT);   /*set receive LED pin mode*/
  pinMode(6, INPUT); /* set button pin mode*/
}

long lastXBeeRec=0; /*time of last received XBee byte*/

void loop() {
  if(Serial3.available() && !digitalRead(buttonPin)){ /*if XBee data is available and button is not pressed, start reading from serial buffer*/
    Serial.println((char)Serial3.read()); /*print received byte*/
    digitalWrite(rLEDPin, HIGH); /*turn on receive LED*/
    lastXBeeRec = millis(); /* update time of last received byte*/
  }
  else if(digitalRead(buttonPin)){ /*if button is pressed, start sending*/
    digitalWrite(gLEDPin, HIGH); /*turn on send LED*/
    Serial3.print('M'); /*send a byte*/
  }
 if(!digitalRead(buttonPin)) digitalWrite(gLEDPin,LOW); /*if the button is not pressed, turn off the send LED*/
 if(!Serial3.available()&&millis()-lastXBeeRec>50) digitalWrite(rLEDPin, LOW); /*ensure LED is on for at least 50ms to ensure it visibly turns on and to give time for the next byte*/
} 
