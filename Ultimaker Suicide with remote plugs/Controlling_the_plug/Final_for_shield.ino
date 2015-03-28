/*
  We use my suicide shield on a Uno.
  Output pin for the transmitter is #11
  Input pin from the optocoupler is #12 This is monitoring Ultimaker pin A0. A0 is put high when Marlin starts and down when a M81 code is read
  Pins 8 and 9 are broken out as well for future purposes. Let's say pin 9 is for a cooldown fan and pin 8 is for enabling the function.
  Pin 13 with led is used to indicate status (OFF= printer should be killed)

  Original library coming from http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
// Create switch object
RCSwitch mySwitch = RCSwitch();
// Assign pins
int ledpin = 13;
int UltiPin = 12;
int enablePin = 8;
int fanPin = 9;

//variables
unsigned long StartMillis = 0;

// Setup params
unsigned int CooldownTime = 10; //cooldown time in seconds



void setup() {

pinMode(ledpin, OUTPUT);
pinMode(fanPin, OUTPUT);
pinMode(UltiPin, INPUT_PULLUP);
pinMode(enablePin, INPUT_PULLUP);


  // Transmitter is connected to Arduino Pin #11  
  mySwitch.enableTransmit(11);
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);


  Serial.begin(9600);
  Serial.println("Start");
  delay(10000); //give the printer time to start and activate the output pin
}

void loop() {
//  check if the function is enabled, otherwise do nothing. then wait.
if(digitalRead(enablePin)==0){
    Serial.println("enabled");


  //Serial.println(digitalRead(UltiPin));

  digitalWrite(ledpin,HIGH);
  StartMillis=(millis() + 10000);
  // check if the Ulti has been done for 10 solid seconds and then start the shutdown sequence.
  // the input is active as long as the Ultimaker is on
  while(digitalRead(UltiPin)==1){
    
    Serial.println("counting");
    Serial.print("CD: ");
    Serial.println(StartMillis);
    if(millis() >= (StartMillis)){
      Serial.println("call suicide func");
      //cooldown(); 
      suicide();
      
      }
  }
  delay(1000);
} else {
  Serial.println("disabled");
  delay(5000);
}



/*

  // Switch on:
  // The first parameter represents the first setting of the rotary switch or "house code". 
  // A to D (A=1, B=2 and so on..) or I to IV depending on the brand.
  // 
  // The second parameter represents the second setting of the rotary switch (1 to 4 in my case). 
  if(digitalRead(UltiPin) == LOW){
   
    mySwitch.switchOff(3, 2);
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
  */
  }


void suicide()  {
  // suicide the printer
  Serial.println("Committing suicide");
  digitalWrite(ledpin,LOW);

  // Switching off the power outlet:

  // The first parameter represents the first setting of the rotary switch or "house code". 
  // A to D (A=1, B=2 and so on..) or I to IV depending on the brand.
  // 
  // The second parameter represents the second setting of the rotary switch (1 to 4 in my case). 
  mySwitch.switchOff(3, 2);

  // Everything should be dead by now, but if the transmission failed it will repeat after this delay
  delay(5000);
}

void cooldown() {
  Serial.println("starting cooldown")
  //Activates a cooldown fan for n seconds
  for(int i=0; i<CooldownTime; i++){
      digitalWrite(fanPin, HIGH);
      delay(1000); //each iteration is a second long (give or take a few ms)
  }
  digitalWrite(fanPin, LOW);
  delay(1000);
}