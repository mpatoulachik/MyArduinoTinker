
// define all the pins
#define ClockPin 2
#define DataPin 3
#define SignBit 20 // the number of the bit that defines the sign of the measurement
#define TimeoutDelay 20 //the time after which we consider the message as timed out

//declare variables
//we have 6x4 bits of data. We take the fist 16

volatile unsigned long RawValue = 0;	//Raw reading of the bitstream
unsigned long OldRawValue = 0;		//1 cycle old version of the above
//byte Data = 0;		//the rest of the 8 bits data nibblets
volatile bool ReadBit = 0;	//The data bit we read from the data line
volatile bool ClockFlag = 0;	//We read a clok pulse
volatile int BitPosition = 0; // the position of the bit we are at.
//long Output = 0;
unsigned long LastClockTick = 0;
word Measurement =0;
int Sign = 1; 		//the sign of the dimension we measure
long lastprint = 0; 	//time of last print
bool ReadCompleted = 0;	//If we have a complete message or not




void setup()
{
	Serial.begin(115200); 	
  	pinMode(ClockPin, INPUT);  
  	pinMode(DataPin, INPUT); 

  	//attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);

  	Serial.println("Start");	//to show it's alive
}



void loop()
{
	
	//if(ReadCompleted == 0){
    GetMeasurement();
	//}
	


	if(RawValue != OldRawValue && millis() >= (lastprint+500)){		//Print what we have read (only if new value and max every 500ms)
	 
		Serial.print(RawValue,BIN);
		Serial.print("\t");
		Serial.print(Sign);
		Serial.print("\t");
		Serial.println(Measurement);
		OldRawValue = RawValue;
		lastprint = millis();

	}

}






void clockISR(){ //This is what happens when we detect a clock edge
  ReadBit = !(digitalRead(DataPin));	//Read the data pin (inverted) and store it temporarly
  bitWrite(RawValue,BitPosition,ReadBit);	//Binary Write 1 or 0 at the proper position in our 16 bit value. We write right to left as the indicator outputs LSB first
  BitPosition ++;	//Move the position 1 notch for next time
}



void GetMeasurement(){	//This calls for a reading of the caliper

	attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);

	if(BitPosition == 0){		//reset the timer if we start from 0
    LastClockTick = millis();
	}
	
	if (BitPosition == 24){ 	//We have a full message
		
		detachInterrupt(digitalPinToInterrupt(ClockPin)); //we are done, free the CPU for other stuff
		
		BitPosition = 0;
		
		ReadCompleted = 1;
	}

	
	if((millis() - LastClockTick) > TimeoutDelay){	// nothing happened with the clock for a long time, so we are between 2 messages or missed something. reset everything and get ready for the next.
		//this way we ignore half messages (like if we start iin the middle of it)
		BitPosition = 0;
		detachInterrupt(digitalPinToInterrupt(ClockPin)); // free the processor so other stuff can happen
		RawValue = 0;
		Serial.println("Timeout"); //so we can see how often it happens
		ReadCompleted = 0;
		
	}
	
	//we get the sign of the measurement
	if(bitRead(RawValue,SignBit) == 1){
		Sign = -1;
	    	}

	else {
		Sign = 1;
	}


   Measurement = word(RawValue); //take the last 16 bits of our value


	


}