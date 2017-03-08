void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
  pinMode(8, OUTPUT); /*set send LED pin mode*/
  pinMode(9, OUTPUT);   /*set receive LED pin mode*/
  pinMode(6, INPUT); /* set button pin mode*/
}

long lastRec=0; /*time of last received byte*/

void loop() {
  if(Serial3.available() && !digitalRead(6)){ /*if XBee data is available and button is not pressed, start reading from serial buffer*/
    Serial.println((char)Serial3.read()); /*print received byte*/
    digitalWrite(9, HIGH); /*turn on receive LED*/
    lastRec = millis(); /* update time of last received byte*/
  }
  else if(digitalRead(6)){ /*if button is pressed, start sending*/
    digitalWrite(8, HIGH); /*turn on send LED*/
    Serial3.print('M'); /*send a byte*/
  }
 if(!digitalRead(6)) digitalWrite(8,LOW); /*if the button is not pressed, turn off the send LED*/
 if(!Serial3.available()&&millis()-lastRec>50) digitalWrite(9, LOW); /*ensure LED is on for at least 50ms to ensure it visibly turns on and to give time for the next byte*/
} 
