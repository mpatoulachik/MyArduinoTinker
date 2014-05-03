

// TestAFMotorandInterrupts.ino
//test if we can read the encoder ticks and run a dc motor


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

Motor as M3
Optosensor as I2
(Pot as A0)



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
AF_DCMotor motor(3); // create motor #3, 
volatile bool ledState = LOW;             //State of the led
volatile unsigned long counter = 0;       //Tick counter
unsigned long newMillis = 0;              //time counting


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
  Serial.begin(9600);                     // set up Serial library at 9600 bps
  motor.setSpeed(200);                    // set the speed to 200/255
  attachInterrupt(0, state, CHANGE);      //activate the interrupt
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

  motor.run(FORWARD);                     // turn motor on going forward
  //delay(500);
       if(millis() >= newMillis+500){          //Just some delay for slower serial update. No delay() because it would stop the rest of the action as well 
                Serial.print(ledState);                 //Serial out for debugging
                Serial.print("-");
                Serial.println(counter);
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
