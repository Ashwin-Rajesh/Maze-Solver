const int motorPins[][2]  = {{2,3},{4,6}};  // Pins connected to motor driver - Refer index at beginning
const int LED = LED_BUILTIN;

const bool algoDebug = true;
const bool motorDebug = true;

int motorDir[2];                            // Stores value denoting diection / action to be executed by motors - Refer index at beginning for values and corresponding actions
float usDist[3];                            // Stores distance from obstacle last detected by ultraound sensors
int roboDir;                                // Stores direction in which robot should move. Refer index

void setup() 
{
  pinMode(LED,OUTPUT);
  
  pinMode(motorPins[0][0],OUTPUT);
  pinMode(motorPins[0][1],OUTPUT);
  pinMode(motorPins[1][0],OUTPUT);
  pinMode(motorPins[1][1],OUTPUT);
}

void loop() {
  digitalWrite(LED,HIGH);
  for(int i = 0; i < 5; i++)
  {
    frontStep(200);
    delay(2000);
  }
  digitalWrite(LED,LOW);
  
  leftStep(500);
  delay(2000);

  rightStep(500);
  delay(2000);

}

void frontStep(int dly)
{
  front();
  delay(dly);
  stopStep();
  if(algoDebug)     Serial.println("  -moving forward");  
}

void backStep(int dly)
{
  back();
  delay(dly);
  stopStep();
  if(algoDebug)     Serial.println("  -moving backwards");
}

void leftStep(int dly)
{
  left();
  delay(dly);
  /*while(usGet(1) <= openDist[1])
  {
    left();
    delay(dly/10);
  }*/
  stopStep();
  if(algoDebug)     Serial.println("  -turning left");
}

void rightStep(int dly)
{
  right();
  delay(dly);
  /*while(usGet(1) <= openDist[1])
  {
    right();
    delay(dly/10);
  }*/
  stopStep();
  if(algoDebug)     Serial.println("  -turning right");
}

void stopStep()
{
  roboDir = 0;
  roboWrite();
  motorWrite();
}

void left()
{
  roboDir = 4;
  roboWrite();
  motorWrite();
}

void right()
{
  roboDir = 3;
  roboWrite();
  motorWrite();
}

void back()
{
  roboDir = 2;
  roboWrite();
  motorWrite();
}

void front()
{
  roboDir = 1;
  roboWrite();
  motorWrite();
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
