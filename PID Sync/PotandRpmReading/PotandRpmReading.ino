// MotorInterruptsAndPot.ino
//Test if we can read a rpm value
// get how many microseconds between pulses


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
The wheel has 48 teeth, the motor goes at about 120rpm max. that is a tick every 10 ms, which should be plenty fr the arduino to do its stuff between interrupts. (I guess...)

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

// User config variables
byte nTeeth = 48; //Number of teeth on the encoder wheel. Up to 255 as datatype is byte
//unsigned long refreshRate = 100 // how fast RPMs are calculated

// System variables

int ledPin = 13;    //onboard led
int potPin = 0;     //Potentiometer wiper
AF_DCMotor motor(3); // create motor #3, 
volatile bool ledState = LOW;             //State of the led
volatile unsigned long counter = 0;       //Tick counter
unsigned long newMillis = 0;              //time counting
volatile unsigned long cntMicros = 0;              //time counting for RPM counter
float rpm = 0;
unsigned int potVal = 0;						//Read value of the pot
unsigned long elapsedMicros = 0;

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
  potVal = analogRead(potPin);            //Read pot value
  motor.setSpeed(potVal/4);               //Divide by four to get the 1024 to be a 256; Set this as motor speed

  motor.run(FORWARD);                     // turn motor on going forward
elapsedMicros = 
rpm = 60/cntMicros*nTeeth/1000000;





  //delay(500);
       if(millis() >= newMillis+500){          //Just some delay for slower serial update. No delay() because it would stop the rest of the action as well 
                Serial.print("Led");
                Serial.print(ledState);                 //Serial out for debugging
                Serial.print("-Cnt");
                Serial.print(cntMicros);
                Serial.print("-pot");
                Serial.print(potVal);
    			Serial.print("rpm");
                Serial.println(rpm);

                newMillis = millis();
              }
cntMicros = micros();
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
  	cntMicros = micros();
  	++ counter;                             //increment the counter
}