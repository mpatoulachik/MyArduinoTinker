// MotorInterruptsAndPot.ino
//test if we can read the encoder ticks and run a dc motor controlled by a pot


/***
 *    ██╗  ██╗ █████╗ ██████╗ ██████╗ ██╗    ██╗ █████╗ ██████╗ ███████╗
 *    ██║  ██║██╔══██╗██╔══██╗██╔══██╗██║    ██║██╔══██╗██╔══██╗██╔════╝
 *    ███████║███████║██████╔╝██║  ██║██║ █╗ ██║███████║██████╔╝█████╗  
 *    ██╔══██║██╔══██║██╔══██╗██║  ██║██║███╗██║██╔══██║██╔══██╗██╔══╝  
 *    ██║  ██║██║  ██║██║  ██║██████╔╝╚███╔███╔╝██║  ██║██║  ██║███████╗
 *    ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝  ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
 *                                                                      
 */


/*
Arduino uno, AF Motorshield, 5V geared DC motor with encoder wheel, Optointerrupter


Interrupts on an arduino Uno are I0= pin 2 and I1= Pin3
Pin 2 is always available on AF Motorshield
Pin 3 is available only if DC motor 2 and stepper1 are NOT used
--> Use DC motors as 3 and 4

Note that motor.setspeed is 0-255 and an analogRead (Pot) is 0-1023

Motor as M3
Optosensor as I2
Pot as A0



*/


//  ██╗███╗   ██╗██╗████████╗
//  ██║████╗  ██║██║╚══██╔══╝
//  ██║██╔██╗ ██║██║   ██║   
//  ██║██║╚██╗██║██║   ██║   
//  ██║██║ ╚████║██║   ██║   
//  ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝   
//                           


#include <AFMotor.h>

int ledPin = 13;    //onboard led
int potPin = 0;
AF_DCMotor motor(3); // create motor #3, 
volatile bool ledState = LOW;             //State of the led
volatile unsigned long counter = 0;       //Tick counter
unsigned long newMillis = 0;              //time counting
unsigned int potVal = 0;						//Read value of the pot


//  ███████╗███████╗████████╗██╗   ██╗██████╗ 
//  ██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗
//  ███████╗█████╗     ██║   ██║   ██║██████╔╝
//  ╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝ 
//  ███████║███████╗   ██║   ╚██████╔╝██║     
//  ╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝     
//                                            


void setup() 
{
  pinMode(ledPin, OUTPUT);                //Activating the led
  //pinMode(potPin, INPUT);
  Serial.begin(9600);                     // set up Serial library at 9600 bps
  motor.setSpeed(200);                    // set the speed to 200/255
  attachInterrupt(0, state, RISING);      //activate the interrupt
}


//  ██╗      ██████╗  ██████╗ ██████╗ 
//  ██║     ██╔═══██╗██╔═══██╗██╔══██╗
//  ██║     ██║   ██║██║   ██║██████╔╝
//  ██║     ██║   ██║██║   ██║██╔═══╝ 
//  ███████╗╚██████╔╝╚██████╔╝██║     
//  ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝     
//                                    


void loop() 
{
  digitalWrite(ledPin, ledState);         //blink the led
  potVal = analogRead(potPin);
  motor.setSpeed(potVal/4);

  motor.run(FORWARD);                     // turn motor on going forward
  //delay(500);
       if(millis() >= newMillis+500){          //Just some delay for slower serial update. No delay() because it would stop the rest of the action as well 
                Serial.print("Led");
                Serial.print(ledState);                 //Serial out for debugging
                Serial.print("-Cnt");
                Serial.print(counter);
                Serial.print("-pot");
                Serial.println(potVal);
                newMillis = millis();
              }
}
  
  


//  ██╗███╗   ██╗████████╗███████╗██████╗ ██████╗ ██╗   ██╗██████╗ ████████╗
//  ██║████╗  ██║╚══██╔══╝██╔════╝██╔══██╗██╔══██╗██║   ██║██╔══██╗╚══██╔══╝
//  ██║██╔██╗ ██║   ██║   █████╗  ██████╔╝██████╔╝██║   ██║██████╔╝   ██║   
//  ██║██║╚██╗██║   ██║   ██╔══╝  ██╔══██╗██╔══██╗██║   ██║██╔═══╝    ██║   
//  ██║██║ ╚████║   ██║   ███████╗██║  ██║██║  ██║╚██████╔╝██║        ██║   
//  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═╝        ╚═╝   
//                                                                          



void state(){                             //This is the function called by the interrupt
	ledState = !ledState;              //toggle the led state
  ++ counter;                             //increment the counter
}