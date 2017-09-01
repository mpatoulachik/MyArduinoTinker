
// define all the pins
#define ClockPin 2
#define DataPin 3
#define SignBit 20 // the number of the bit thatd efines the sign of the measurement
#define TimeoutDelay 20 //the time after which we consider the message as timed out

//declare variables
//we have 6x4 bits of data. We take the fist 16 as the value end the last 8 s the data
volatile unsigned long RawValue = 0;	//Raw reading of the bitstream
unsigned long OldRawValue = 0;
byte Data = 0;		//the rest of the 8 bits data nibblets

volatile bool ReadBit = 0;
volatile bool ClockFlag = 0;
volatile int BitPosition = 0; // the position of the bit we are at.
long Output = 0;
unsigned long LastClockTick = 0;
word Measurement =0;
int Sign = 1; 	//the sign of the dimension we measure
long lastprint = 0;

void setup()
{
	Serial.begin(115200);
 
  	pinMode(ClockPin, INPUT);  
  	pinMode(DataPin, INPUT); 
  

  	attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);
  	Serial.println("Start");	//to show it's alive
	
}

void loop()
{
	
	if(BitPosition == 0){
    LastClockTick = millis();
	}
	
	if (BitPosition == 24){ 	//We have a full message
		
		detachInterrupt(digitalPinToInterrupt(ClockPin));
		
		BitPosition = 0;
		//LastClockTick = millis();
		//RawValue = 0;
	}

	
	if((millis() - LastClockTick) > TimeoutDelay){	// nothing happened with the clock for a long time, so we are between 2 messages or missed something. get ready for the next.
		BitPosition = 0;
		detachInterrupt(digitalPinToInterrupt(ClockPin));
	    //attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);
		RawValue = 0;
		Serial.println("Timeout");
	}
	
	//we get the sign of the measurement
	if(bitRead(RawValue,SignBit) == 1){
		Sign = -1;
	    
	}
	else {
		Sign = 1;
	}



   Measurement = word(RawValue);


	if(RawValue != OldRawValue || millis() >= (lastprint+500)){
	 
		Serial.print(Measurement,BIN);
		Serial.print("\t");
		Serial.print(RawValue,BIN);
		Serial.print("\t");
		Serial.print(Sign);
		Serial.print("\t");
		Serial.println(Measurement);
		OldRawValue = RawValue;
		lastprint = millis();

	}
	
	attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);


}




void clockISR(){ //This is what happens when we detect a clock edge
  ReadBit = !(digitalRead(DataPin));
  bitWrite(RawValue,BitPosition,ReadBit);
  BitPosition ++;
}