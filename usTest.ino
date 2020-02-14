
const int trigPins[]      = {7,9,11};       // Trigger pins of ultrasound sensors
const int echoPins[]      = {8,10,12};      // Echo pins of ultrasound sensors

float usMaxDist[3]        = {30,30,30};     // Maximum distance till which ultrasound scans for obstacles
float usTimeOut[3];                         // Time in milliseconds after which ultrasound sensors will timeOut and ignore echoes
const float distFactor    = 0.034/2;        // Factor by which echo time has to be multiplied by to get distance to obstacle
float openDist[3]         = {25,25,25};     // Distance above value returned, at which algorithm recognizes opening

bool usDebug              = false;          // Display ultrasound output values through serial monitor?

float usDist[3];                            // Stores distance from obstacle last detected by ultraound sensors

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPins[0],OUTPUT);
  pinMode(trigPins[1],OUTPUT);
  pinMode(trigPins[2],OUTPUT);
  
  pinMode(echoPins[0],INPUT);
  pinMode(echoPins[1],INPUT);
  pinMode(echoPins[2],INPUT);  
  
  Serial.begin(115200);
  Serial.println(" Initialised serial communications. ");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  for(int i = 0; i < 3; i++)
   {
      usRead(i);
      Serial.println(usDist[i]);
   }
}

void usRead(int index)
{
  digitalWrite(trigPins[index],HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[index],LOW);
  int echoTime = pulseIn(echoPins[index],HIGH,usTimeOut[index]);
  if(echoTime == 0)
    usDist[index] = usMaxDist[index];
  else
    usDist[index] = getDistance(echoTime);
}

float getDistance(int echoTime)
{
  return echoTime * distFactor;
}
