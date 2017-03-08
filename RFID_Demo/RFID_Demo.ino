/*
  
  Parallax RFID Reader: Basic Demonstration       
                                                         
  Author: Joe Grand [www.grandideastudio.com]             
  Contact: support@parallax.com                            
  
  Program Description:
  
  This program provides a simple demonstration of the Parallax RFID Card
  Reader (#28140). If a valid RFID tag is placed in front of the reader,
  its unique ID is displayed in the Arduino Serial Monitor. 
  
  Please refer to the product manual for full details of system functionality 
  and capabilities.

  Revisions:
  
  1.0 (April 30, 2014): Initial release
  
*/

// include the SoftwareSerial library so we can use it to talk to the RFID Reader
#include <SoftwareSerial.h>

#define enablePin  9   // Connects to the RFID's ENABLE pin
#define rxPin      10  // Serial input (connects to the RFID's SOUT pin)
#define txPin      11  // Serial output (unused)

#define BUFSIZE    11  // Size of receive buffer (in bytes) (10-byte unique ID + null character)

#define RFID_START  0x0A  // RFID Reader Start and Stop bytes
#define RFID_STOP   0x0D

// set up a new serial port
SoftwareSerial rfidSerial =  SoftwareSerial(rxPin, txPin);

void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(enablePin, OUTPUT);
  pinMode(rxPin, INPUT);

  digitalWrite(enablePin, HIGH);  // disable RFID Reader
  
  // setup Arduino Serial Monitor
  Serial.begin(9600);
  while (!Serial);   // wait until ready
  Serial.println("\n\nParallax RFID Card Reader");
  
  // set the baud rate for the SoftwareSerial port
  rfidSerial.begin(2400);

  Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor
}

void loop()  // Main code, to run repeatedly
{
  /* 
    When the RFID Reader is active and a valid RFID tag is placed with range of the reader,
    the tag's unique ID will be transmitted as a 12-byte printable ASCII string to the host
    (start byte + ID + stop byte)
    
    For example, for a tag with a valid ID of 0F0184F07A, the following bytes would be sent:
    0x0A, 0x30, 0x46, 0x30, 0x31, 0x38, 0x34, 0x46, 0x30, 0x37, 0x41, 0x0D
    
    We'll receive the ID and convert it to a null-terminated string with no start or stop byte. 
  */   
  
  digitalWrite(enablePin, LOW);   // enable the RFID Reader
  
  // Wait for a response from the RFID Reader
  // See Arduino readBytesUntil() as an alternative solution to read data from the reader
  char rfidData[BUFSIZE];  // Buffer for incoming data
  char offset = 0;         // Offset into buffer
  rfidData[0] = 0;         // Clear the buffer    
  while(1)
  {
    if (rfidSerial.available() > 0) // If there are any bytes available to read, then the RFID Reader has probably seen a valid tag
    {
      rfidData[offset] = rfidSerial.read();  // Get the byte and store it in our buffer
      if (rfidData[offset] == RFID_START)    // If we receive the start byte from the RFID Reader, then get ready to receive the tag's unique ID
      {
        offset = -1;     // Clear offset (will be incremented back to 0 at the end of the loop)
      }
      else if (rfidData[offset] == RFID_STOP)  // If we receive the stop byte from the RFID Reader, then the tag's entire unique ID has been sent
      {
        rfidData[offset] = 0; // Null terminate the string of bytes we just received
        break;                // Break out of the loop
      }
          
      offset++;  // Increment offset into array
      if (offset >= BUFSIZE) offset = 0; // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bounds
    }
  }

  Serial.println(rfidData);       // The rfidData string should now contain the tag's unique ID with a null termination, so display it on the Serial Monitor
  Serial.flush();                 // Wait for all bytes to be transmitted to the Serial Monitor
}



