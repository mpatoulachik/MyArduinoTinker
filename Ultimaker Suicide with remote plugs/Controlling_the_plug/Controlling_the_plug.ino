/*
  We use my Nexa shield on a Uno.
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
int ledpin = 13;


void setup() {
pinMode(ledpin, OUTPUT);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the first setting of the rotary switch or "house code". 
  // A to D (A=1, B=2 and so on..) or I to IV depending on the brand.
  // 
  // The second parameter represents the secondsetting of the rotary switch (1 to 4 in my case). 
  mySwitch.switchOn(3, 2);
  digitalWrite(ledpin,HIGH);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff(3, 2);
  digitalWrite(ledpin,LOW);
  // Wait another second
  delay(1000);
  
}
