

// Using AF Motor shield, 2x 6v geared motors, spooling up and down as crane
//the goal is to have a crane with two winches that keeps the load level
// Pot and hanging weight to sense level (pendulum principle, the pot measures the angle to the vertical)
//Arduino powered by USB, shield by 6V PS





//  ██████╗ ███████╗███████╗ ██████╗██████╗ ██╗██████╗ ████████╗██╗ ██████╗ ███╗   ██╗
//  ██╔══██╗██╔════╝██╔════╝██╔════╝██╔══██╗██║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
//  ██║  ██║█████╗  ███████╗██║     ██████╔╝██║██████╔╝   ██║   ██║██║   ██║██╔██╗ ██║
//  ██║  ██║██╔══╝  ╚════██║██║     ██╔══██╗██║██╔═══╝    ██║   ██║██║   ██║██║╚██╗██║
//  ██████╔╝███████╗███████║╚██████╗██║  ██║██║██║        ██║   ██║╚██████╔╝██║ ╚████║
//  ╚═════╝ ╚══════╝╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝╚═╝        ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
//                                                                                    

                                                               

/* 
AF motor shield
Digital pins used by the shield, so we will use the analog ones (as digital):
All digital pins to be used with pullup reisitors --> Button pushed = LOW

Pushbutton for level reset on pin 14 (A0)
Toggleswitch (on-off-on) on pins 15 (A1) and 16 (A2) --> Motor selection for manual driving or auto mode for off
Pushbutton for up, pin 17 (A3)
Pushbutton for down, pin 18 (A4)
Level pot: analog pin A5

Function:


-You put i the device in manual mode and level it by hand. from there you push the reset button. The angle reading is then stored as reference and the system will try to keep the load at that angle no mattre what.


*/



											//   ██████╗ ██████╗ ██████╗ ███████╗
											//  ██╔════╝██╔═══██╗██╔══██╗██╔════╝
											//  ██║     ██║   ██║██║  ██║█████╗  
											//  ██║     ██║   ██║██║  ██║██╔══╝  
											//  ╚██████╗╚██████╔╝██████╔╝███████╗
											//   ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝
											//                                   

#include "AFMotor.h"

AF_DCMotor motor1(3);
AF_DCMotor motor2(4);
// pins
const int resetPin = 14;
const int motor1Pin = 15;
const int motor2Pin = 16;
const int upPin = 17;
const int downPin = 18;
const int potPin = A5;

//Variables internal to the program
int debugFlag = 1; // set to 1 to activate debug, 0 otherwise
unsigned long oldMillis = 0; //Used for debug

int motorMan = 0; //Gets the status of the manual/auto switch. 0 for maual, 1for motor 1, 2 for motor 2
int runDirection = RELEASE; //default= inactive motors
int potValue = -1; //pot reading
int potPreset = 0; //preset value of the level
boolean notLevel = false;



// --------------------Process variables: Here are your settings------------------------


int runSpeed = 200;	// General speed of the motors. Later improved with a speed pot ??
int lowSpeed = 100; // Speed when adjusting level.
int deadBand = 10;	// Dead band - hysteresis of the levelling function



//  ███████╗███████╗████████╗██╗   ██╗██████╗ 
//  ██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗
//  ███████╗█████╗     ██║   ██║   ██║██████╔╝
//  ╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝ 
//  ███████║███████╗   ██║   ╚██████╔╝██║     
//  ╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝     
//                                            



void setup() {
pinMode(14, INPUT_PULLUP);
pinMode(15, INPUT_PULLUP);
pinMode(16, INPUT_PULLUP);
pinMode(17, INPUT_PULLUP);
pinMode(18, INPUT_PULLUP);
pinMode(A5, INPUT);
if(debugFlag == 1){
    Serial.begin(9600);	// for debug purposes
}
}



//  ██╗      ██████╗  ██████╗ ██████╗ 
//  ██║     ██╔═══██╗██╔═══██╗██╔══██╗
//  ██║     ██║   ██║██║   ██║██████╔╝
//  ██║     ██║   ██║██║   ██║██╔═══╝ 
//  ███████╗╚██████╔╝╚██████╔╝██║     
//  ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝     
//                                    



void loop() {
	motor1.setSpeed(runSpeed);
	motor2.setSpeed(runSpeed);
	potValue = analogRead(potPin); //reads pot value and puts in in a convenient variable


// Resets level preset when button is pressed
if(digitalRead(resetPin) == LOW){
    potPreset = potValue;
	}

// if any button is pushed you're in manual mode, otherwise you're on auto.
if(digitalRead(motor1Pin) == LOW 
	|| digitalRead(motor2Pin) == LOW
	|| digitalRead(upPin) == LOW
	|| digitalRead(downPin) == LOW){

    manualMode();
	} 

 else {
 	motorStop(); //stops motors in case manual mode left them on
	autoMode();
 }

//enables (or not) the serial debug
	if(debugFlag == 1){
	    myDebug();
	}



}

//  ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗ 
//  ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝ 
//  ██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗
//  ██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║
//  ██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝
//  ╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝ 
//                                            


// Debug through serial, gives variable and input status every 500 ms
void myDebug(){
	if(millis() >= (oldMillis + 500)){
	    
	    Serial.print(" Level: ");
	    Serial.print(notLevel);
		Serial.print(" Pot: ");
		Serial.print(potValue);
		Serial.print(" Preset: ");
		Serial.print(potPreset);
		Serial.print(" Reset: ");
		Serial.print(digitalRead(resetPin));
		Serial.print(" Dir: ");
		Serial.print(runDirection);
		Serial.print(" Mode: ");
		Serial.println(motorMan);
		oldMillis = millis();
		}
}



//   █████╗ ██╗   ██╗████████╗ ██████╗     ███╗   ███╗ ██████╗ ██████╗ ███████╗
//  ██╔══██╗██║   ██║╚══██╔══╝██╔═══██╗    ████╗ ████║██╔═══██╗██╔══██╗██╔════╝
//  ███████║██║   ██║   ██║   ██║   ██║    ██╔████╔██║██║   ██║██║  ██║█████╗  
//  ██╔══██║██║   ██║   ██║   ██║   ██║    ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  
//  ██║  ██║╚██████╔╝   ██║   ╚██████╔╝    ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗
//  ╚═╝  ╚═╝ ╚═════╝    ╚═╝    ╚═════╝     ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝
//                                                                             



void autoMode(){
	//Checks if levelling is needed
	if((abs(potValue - potPreset) > deadBand) && potPreset > 0){
	    notLevel = true;
		}
	else {
		notLevel = false;
		}

	if(notLevel == true){
		autoRun();
	}

	
}

void autoRun(){
	//Runs the motors as needed by levelling function
	if(potValue < potPreset){
	   runDirection = FORWARD;
	  } 
  	else if(potValue > potPreset){
  	    runDirection = BACKWARD;
  	}

	  
	   
	   motor1.setSpeed(lowSpeed);
	   motor1.run(runDirection); 
	   delay(100);//motor runs for at least 100ms to avoid micromovements at the edge of the deadband
	
}


//  ███╗   ███╗ █████╗ ███╗   ██╗██╗   ██╗ █████╗ ██╗         ███╗   ███╗ ██████╗ ██████╗ ███████╗
//  ████╗ ████║██╔══██╗████╗  ██║██║   ██║██╔══██╗██║         ████╗ ████║██╔═══██╗██╔══██╗██╔════╝
//  ██╔████╔██║███████║██╔██╗ ██║██║   ██║███████║██║         ██╔████╔██║██║   ██║██║  ██║█████╗  
//  ██║╚██╔╝██║██╔══██║██║╚██╗██║██║   ██║██╔══██║██║         ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  
//  ██║ ╚═╝ ██║██║  ██║██║ ╚████║╚██████╔╝██║  ██║███████╗    ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗
//  ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝    ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝
//                                                                                                

void manualMode(){
		//Checks what motor should be run
	if(digitalRead(motor1Pin) == LOW){	//Motor 1 is selected for manual run
	    motorMan = 1;

	}
	else if(digitalRead(motor2Pin) == LOW){ //Motor 2 is selected for manual run
	    motorMan = 2;
	}
	else {	//No motor is selected --> Auto / dual mode
		motorMan = 0;
	}
	// check in which direction the motor should run
	if(digitalRead(upPin) == HIGH && digitalRead(downPin) == LOW){
	  runDirection = FORWARD; 
	}
	else if(digitalRead(downPin) == HIGH && digitalRead(upPin) == LOW){
	  runDirection = BACKWARD; 
	}
	else {runDirection = RELEASE; // in case the 2 buttons are pressed, nothing should happen
	}

	// Actually run the motors
	//runs the motor(s) that have been selected above in the direction decided by the buttons. No buttons= inactive state

	if(motorMan == 1){
	    motor1.run(runDirection);
		}
		else if(motorMan == 2){
	   		 motor2.run(runDirection);
		} 
		else {
	    	motor1.run(runDirection);
	    	motor2.run(runDirection);

	}
}


void motorStop(){
	//Stops all motors
	runDirection = RELEASE;
	motor1.run(runDirection);
	motor2.run(runDirection);
}






//  ████████╗ ██████╗ ██████╗  ██████╗ 
//  ╚══██╔══╝██╔═══██╗██╔══██╗██╔═══██╗
//     ██║   ██║   ██║██║  ██║██║   ██║
//     ██║   ██║   ██║██║  ██║██║   ██║
//     ██║   ╚██████╔╝██████╔╝╚██████╔╝
//     ╚═╝    ╚═════╝ ╚═════╝  ╚═════╝ 
//                                     

/*

default for preset = -1 , that means uncalibrated. (ANALOG IS 0-1023)

-Spare one pin by making only one motor manually driveable (and spare some code)
-Add IR sensor for hovering ability (auto mode)
-HW: Add a board with a 7806 to power everything through 1 power supply (or check if 5v is ok ....)

*/