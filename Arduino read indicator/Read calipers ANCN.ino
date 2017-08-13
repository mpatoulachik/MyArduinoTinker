

// define all the pins
#define ClockPin 2
#define DataPin 3
#define SignBit 20 // the number of the bit thatd efines the sign of the measurement
#define TimeoutDelay 20 //the time after which we consider the message as timed out

//declare variables
//we have 6x4 bits of data. We take the fist 16 as the value end the last 8 s the data
unsigned int Measurement = 0;	//measurement value in hunderdth of mm
byte Data = 0;		//the rest of the 8 bits data nibblets

volatile bool ReadBit = 0;
volatile bool ClockFlag = 0;
int BitPosition = 0; // the position of the bit we are at.
long Output = 0;
unsigned long LastClockTick = 0;




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
	if(ClockFlag == 1){	//we recieved a bit

	    bitWrite(Measurement,BitPosition,ReadBit);
	    //Measurement << 1;
	    BitPosition ++;
	    LastClockTick = millis();
	    ClockFlag = 0;

	}
	if (BitPosition == 15){
		Serial.print(Measurement,BIN);
		Serial.print("\t");
		Serial.println(Measurement);
		detachInterrupt(digitalPinToInterrupt(ClockPin));
		
		
	}
	if((millis() - LastClockTick) > TimeoutDelay){
	    attachInterrupt(digitalPinToInterrupt(ClockPin), clockISR, FALLING);
		BitPosition = 0;
	}
}




void clockISR(){ //This is what happens when we detect a clock edge
 ClockFlag = 1; 
 ReadBit = !(digitalRead(DataPin));
}