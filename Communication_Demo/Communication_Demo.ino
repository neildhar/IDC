void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, INPUT);
}

long lastRec=0;

void loop() {
  if(Serial3.available() && !digitalRead(6)){
    Serial.println((char)Serial3.read());
    digitalWrite(9, HIGH);
    lastRec = millis();
  }
  else if(digitalRead(6)){
    digitalWrite(8, HIGH);
    Serial3.println("M");
  }
 if(!digitalRead(6)) digitalWrite(8,LOW);
 if(!Serial3.available()&&millis()-lastRec>50) digitalWrite(9, LOW);
} 
