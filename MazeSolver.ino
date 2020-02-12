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
 {{Left 1, Left 2}, {Right 1, Right 2}}
--------------------------------------
motorDir index :
 0 - Stop
 1 - Forward
 2 - Backward
--------------------------------------
roboDir index:
 0 - Stop 
 1 - Forward
 2 - Backward
 3 - Rotate CW (clockwise)
 4 - Rotate CCW (counterclockwise)
--------------------------------------
Motor output index(output to pins in format {pin1,pin2}, refer motorPin index)
 {0,0} - Stop
 {0,1} - Forward
 {1,0} - Backward
--------------------------------------
*/

// Pin Numbers
const int motorPins[][2]  = {{1,1},{1,1}};  // Pins connected to motor driver - Refer index at beginning
const int trigPins[]      = {1,1,1};        // Trigger pins of ultrasound sensors
const int echoPins[]      = {1,1,1};        // Echo pins of ultrasound sensors


// Callibration variables
const float usMaxDist[3]  = {10,10,10};     // Maximum distance till which ultrasound scans for obstacles
float usTimeOut[3];                         // Time in milliseconds after which ultrasound sensors will timeOut and ignore echoes
const float distFactor    = 0.034/2;        // Factor by which echo time has to be multiplied by to get distance to obstacle
int frontOpenDist         = 10;             // Distance above value returned, at which algorithm recognizes opening in front
int leftOpenDist          = 10;             // Same as above, for opening in left
int rightOpenDist         = 10;             // Same as above, for opening in right

// Debug control variables 
bool usDebug              = false;          // Display ultrasound output values through serial monitor?
bool motorDebug           = false;          // Display motor output values through serial monitor?
bool algoDebug             = true;          // Display change in direction and decisions made by algorithm

// Global variables used as buffer
int motorDir[2];                            // Stores value denoting diection / action to be executed by motors - Refer index at beginning for values and corresponding actions
int usDist[3];                              // Stores distance from obstacle last detected by ultraound sensors
int roboDir;                                // Stores direction in which robot should move. Refer index

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
  // If letter 't' is received via Serial, the debug modes change in the order
  // |---------------------|
  // | usDebug | motorDebug|
  // |---------------------|
  // | false   | false     | (initial state)
  // | true    | true      |
  // | false   | true      |
  // | true    | false     | (then cycles)
  // ----------------------|
  while(Serial.available())
  {
    if(Serial.read() == 't')
    {
      usDebug = !usDebug;
      if(usDebug)
      {
        motorDebug = ! motorDebug;
      }
    }
    Serial.println(usDebug);
    Serial.println(motorDebug);    
  }
  
  delay(100);
  for(int i = 0; i < 3; i++)
  {
    usRead(i);
    if(usDebug)
    {
      Serial.print(" Ultrasound sensor ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(usDist[i]);
    }   
  }

  if(usDist[0] >= leftOpenDist)             // Check for opening in left
  {
    // Go left
  }
  else if(usDist[1] <= frontOpenDist)       // Check for obstacle in front
  {
    if(usDist[2] >= rightOpenDist)
    {
      // Go right
    }
    else
    {
      // Go back, turn 180 degrees
    }
  }
}

// Interpret values stored in roboDir and assign values to motorDir, outputs are not given to motors.
void roboWrite()
{
  if(roboDir == 0)
  {
    motorDir[0] = 0;
    motorDir[1] = 0;
  }
  else if(roboDir == 1)
  {
    motorDir[0] = 1;
    motorDir[1] = 1;
  }
  else if(roboDir == 2)
  {
    motorDir[0] = 2;
    motorDir[1] = 2;
  }
  else if(roboDir == 3)
  {
    motorDir[0] = 1;
    motorDir[1] = 2;
  }
  else if(roboDir == 4)
  {
    motorDir[0] = 2;
    motorDir[1] = 1;
  }
}

// Write values stored in motorDir to motorPins appropriately to moe robot as needed
void motorWrite()
{
  for(int i = 0; i < 2; i++)
  {
    if(motorDebug)
    {
      Serial.println();
      Serial.print(" Motor no ");
      Serial.print(i);
      Serial.print(" : ");
    }
    if(motorDir[i] == 0)
    {
      if(motorDebug)  Serial.println("Stop");
      digitalWrite(motorPins[i][0],LOW);
      digitalWrite(motorPins[i][1],LOW);
    }
    else if(motorDir[i] == 1)
    {
      if(motorDebug)  Serial.println("Forward");
      digitalWrite(motorPins[i][0],LOW);
      digitalWrite(motorPins[i][1],HIGH);
    }
    else if(motorDir[i] == 2)
    {
      if(motorDebug)  Serial.println("Backward");
      digitalWrite(motorPins[i][0],HIGH);
      digitalWrite(motorPins[i][1],LOW);
    }
    else
      Serial.println(" Invalid entry");
  }
  Serial.println();
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
