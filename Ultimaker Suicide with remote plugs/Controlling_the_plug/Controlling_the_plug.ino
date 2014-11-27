/*
  We use my Nexa shield on a Uno.
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
int ledpin = 13;
int onPin = 8;
int offPin = 9;


void setup() {

pinMode(ledpin, OUTPUT);
pinMode(8, INPUT_PULLUP);
pinMode(9, INPUT_PULLUP);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);


  
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {

  // Switch on:
  // The first parameter represents the first setting of the rotary switch or "house code". 
  // A to D (A=1, B=2 and so on..) or I to IV depending on the brand.
  // 
  // The second parameter represents the secondsetting of the rotary switch (1 to 4 in my case). 
  if(digitalRead(onPin) == LOW){
   
    mySwitch.switchOn(3, 2);
    digitalWrite(ledpin,HIGH);
    Serial.println ("ON");
    }
  // Wait a second
  //delay(1000);
  
  // Switch off
  if(digitalRead(offPin) == LOW){
       
    mySwitch.switchOff(3, 2);
    digitalWrite(ledpin,LOW);
    Serial.println ("Off");
   }

   // Wait another second
  //delay(1000);
  
}
