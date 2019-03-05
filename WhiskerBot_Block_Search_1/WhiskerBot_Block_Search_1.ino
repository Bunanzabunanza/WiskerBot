//MY BOT with Feeler - WhiskerBot

// Include the libraries.
#include <RedBot.h>   

// Initiate the motor control class.
RedBotMotor motor;

// Joystick variables
int UDStart, LRStart; // Joystick benchmark variables at neutral
int UD, LR;           // Joystick vertical & horizontal variables
int UDP, LRP;         // Joystick values printed
int joyRead;
int UDDiff;
int LRDiff;

// Motor variables
#define fwdSpd 120    // Change speed values here
int pvtPwr;           // motor pwr level
int pvtDur;           // pivot duration if less than timeout, used by rtc
int ti;               // timer value
int stopDur;
int righT;            // flag?
int lefT;             // flag?
int leftPwr = 70;     // pvtPwr adjusted here
int rightPwr = -70;   // pvtPwr adjusted here
int sHort = 5;        // obsolete?
int returnToCenter = 750;  // obsolete?
int edgeAvoid = 100;
int fwdDur = 300;          // obsolete?

// Other variables
int pvt;
int edge;
int cnt;
int es;
int flat;
long interval = 0;
unsigned long startMillis = 0;     // start Timer
unsigned long int doneMillis = 0;  // startMillis plus ti
long int durMillis = 0;            // interval of time desired
long int counTer = 0;

void setup() {
  Serial.begin(9600);
  UDStart = (analogRead(A0));         // Benchmark joystick vertical
  LRStart = (analogRead(A1));         // Benchmark joystick horizontal
  Serial.print("AnalogRead A0 = ");   // Print to check benchmarks
  Serial.println(UDStart, DEC);
  Serial.print("AnalogRead A1 = ");
  Serial.println(LRStart, DEC);
  ti = 5000;
  Timer();
}

int search() {                   // SEARCH function block
  for(int i = 0; i < 2; i++) {   // 4 pivot phases, L,C,R,C
    if (i == 0) {                // Steps left params   
      pvtPwr = leftPwr;
      pvtDur = stepsDur;
      stopDur = sHort;
    }
    else  {
      pvtPwr = rightPwr;         // Steps right params
      pvtDur = stepsDur;
      stopDur = sHort;
    }
    for (pvt = 1; pvt <= 9; pvt++) {   // Pivot steps for edge and block scanning
      // Serial.println(pvt);
      sTeps = pivot();
      joyRead = Joystick();
      /*Serial.print("Vertical = ");
       Serial.print(UDDiff);
       Serial.print("  Horizontal = ");
       Serial.println(LRDiff);*/

      if (UDDiff > 100 && i == 0) {   //if deviation found to the left, turn right and sweep
        Serial.println("Edge left");
        pvtPwr = rightPwr;
        pvtDur = edgeAvoid; 
        stopDur = sHort;
        cnt = 0;
        righT = edgesweep();
        return pvt;
      } 
      if (UDDiff > 100 && i == 1) {   //if deviation found to the right, turn left and sweep
        Serial.println("Edge right");
        pvtPwr = leftPwr; 
        pvtDur = edgeAvoid; 
        stopDur = sHort;
        cnt = 0;
        lefT = edgesweep();
        return pvt;
      } 
      if (LRDiff > 120) {     // Horiz. deflection during clear sweep
        delay(4000);
        return pvt;
      } 
    }
    delay(100);
    if (i == 0) {              //return to center from left sweep
      Serial.print("i is = ");
      Serial.println(i);
      pvtPwr = rightPwr;
      pvtDur = returnToCenter;
      stopDur = sHort;
      righT = pivot();
      delay(150);
    }
    if (i == 1) {              //return to center from right sweep
      Serial.print("i is = ");
      Serial.println(i);
      pvtPwr = leftPwr;
      pvtDur = returnToCenter;
      stopDur = sHort;
      lefT = pivot();
      delay(150);
    }
  }
}

int pivot()  {
  motor.pivot(pvtPwr);    // Redboard library command
  delay(pvtDur);
  motor.stop();           // Redboard library command
  delay(stopDur);
  return pvt;
}

int edgesweep()  {
  Serial.println("Edge recovery and sweep");
  for (es = 1; es <=9; es++)  {
    edge = pivot();
    flat = Joystick();
    Serial.print("es = ");
    Serial.println(es);
    if ((UDDiff < 30) && (cnt == 0))  {        // Whisker back on flat tabletop
      delay (1000);
      cnt = 1;
      es = 4;
      continue;
      if ((LRDiff > 120) && (UDDiff < 100)) {  // Horiz. deflection during clear sweep
        Serial.print("Vertical = ");
        Serial.print(UDDiff);
        Serial.print("  Horizontal = ");
        Serial.println(LRDiff);
        delay(4000);
        return pvt;
      } 
    }
  }
}
void Joystick()  {
  UD = analogRead(A0);             // Read joystick vertical
  LR = analogRead(A1);             // Read joystick horizontal
  UDDiff = abs(UD - UDStart);  // Deviation from benchmark
  LRDiff = abs(LR - LRStart);
  /*Serial.print("Vertical = ");
   Serial.print(UDDiff);
   Serial.print("  Horizontal = ");
   Serial.println(LRDiff);*/
  return pvt;
}



void sweepLeft() {
  }

void sweepRight()  {
  }

void adVance()  {
  }
void reCover()  {
  }



int pivot()  {
  motor.pivot(pvtPwr);       // Redboard library command   
  
  motor.stop();              // Redboard library command
  delay(stopDur);
  return pvt;
}

void Timer() {
 startMillis = millis();
 counTer = 0;
 doneMillis = startMillis + ti;
  while(millis() < doneMillis) {    
    // Hang out and wait
    counTer = counTer + 1;
  } 
}


/* 
  void loop() {
  int searchv = search();
  Serial.println("Scan LOOP done");
  //Serial.println(searchv, DEC);
  motor.drive(fwdSpd);       // Redboard library command 
  delay(fwdDur);
  motor.brake();             // Redboard library command
  delay(150);
}
*/

//
 void loop() {  
 durMillis = 5000;
  // 4 pivot phases, L,C,R,C
  sweepLeft();

  sweepRight();

  adVance();

  pivot180();  

  


