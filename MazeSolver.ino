
/*
Indices used to refer in arrays
--------------------------------------
General Ultrasound index (in arrays) :
 0 : Left
 1 : Center
 2 : Right
 * Applied to variables trigPins, echoPins,usMaxDist,usDist,usTimeOut
--------------------------------------
General Motor index (in arrays)
 0 - Left
 1 - Right
 * Applied to variables motorPins, motorDir
--------------------------------------
motorPins index :
 (0,0) - Left 1
 (0,1) - Left 2
 (1,0) - Right 1
 (1,1) - Right 2
--------------------------------------
motorDir index :
 0 - Stop
 1 - Forward
 2 - Backward
 3 - Rotate cw  (clockwise)
 4 - Rotate ccw (counter-clockwise)
*/

// Pin Numbers
const int motorPins[][2]  = {{1,1},{1,1}};  // Pins connected to motor driver - Refer index at beginning
const int trigPins[]      = {1,1,1};        // Trigger pins of ultrasound sensors
const int echoPins[]      = {1,1,1};        // Echo pins of ultrasound sensors


// Callibration variables
const float usMaxDist[3]  = {10,10,10};     // Maximum distance till which ultrasound scans for obstacles
float usTimeOut[3];                         // Time in milliseconds after which ultrasound sensors will timeOut and ignore echoes
const float distFactor    = 0.034/2;        // Factor by which echo time has to be multiplied by to get distance to obstacle 

// Global variables used as buffer
int motorDir[2];                            // Stores value denoting diection / action to be executed by motors - Refer index at beginning for values and corresponding actions
int usDist[3];                              //Stores distance from obstacle last detected by ultraound sensors

void setup() 
{
  // Setting pinModes
  pinMode(motorPins[0][0],OUTPUT);
  pinMode(motorPins[0][1],OUTPUT);
  pinMode(motorPins[1][0],OUTPUT);
  pinMode(motorPins[1][1],OUTPUT);

  pinMode(trigPins[0],OUTPUT);
  pinMode(trigPins[1],OUTPUT);
  pinMode(trigPins[2],OUTPUT);
  
  pinMode(echoPins[0],INPUT);
  pinMode(echoPins[1],INPUT);
  pinMode(echoPins[2],INPUT);

  // Initialising Serial communications
  Serial.begin(9600);
  Serial.println(" Initialised serial communications. ");
  Serial.println();
  
  // Setting timeout and displaying all callibration variables
  Serial.println(" Callibration values");
  Serial.print(" Distance factor    : ");
  Serial.println(distFactor);
  for(int i = 0; i < 3; i++)
  {
    usTimeOut[i] = usMaxDist[i] / distFactor;
    
    Serial.print(" Ultrasound sensor index  :");
    Serial.println(i);
    Serial.print(" Max distance       : ");
    Serial.println(usMaxDist[i]);    
    Serial.print(" Timeout            : ");
    Serial.println(usTimeOut[i]);
  }
  Serial.println();
}

void loop() 
{
  delay(100);
  for(int i = 0; i < 3; i++)
  {
    usRead(i);
    /*
    Serial.print(" Ultrasound sensor ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(usDist[i]);
    */
  }  
}

// Write values stored in motorDir to motorPins appropriately to moe robot as needed
void motorWrite()
{
  Serial.println(" motor functionality not implemented");
}

// Get readings from an ultrasound sensor and store in usDist array
void usRead(int index)
{
  if(index >= 0 && index <= 2)
  {
    digitalWrite(trigPins[index],HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[index],LOW);
    int echoTime = pulseIn(echoPins[index],HIGH,usMaxDist[index] / distFactor);
    usDist[index] = getDistance(echoTime);
  }
}

// Function returns distance to obstacle from time, received as argument
float getDistance(int echoTime)
{
  return echoTime * distFactor;
}
