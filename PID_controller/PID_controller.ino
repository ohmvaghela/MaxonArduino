// about encoder connection 
// 20 - ChA - 6
// 21 - ChB - 8
// CCW - Positive

///////////////////////////////////////////////////
//BLDC
#include <SD.h>
#include "IFX007T-Motor-Control.h"

#define WEAKENING  0           // 0=normal, 1=weak (fast)
const float stepsPerDegree = 10.0; // Adjust this value based on your motor
int direction = 1;// 1 CW // 0 CCW
uint8_t dutyCycle = 300;

//Create an instance of 'IFX007TMotorControl' called 'MyMotor'
IFX007TMotorControl MyMotor = IFX007TMotorControl();
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//Encoder
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

float pulse = 10.0;
float angle = 0;
float desiredAngle = 90;
float error;

const byte ChA = 21;
const byte ChB = 20;

long GearReduction = 80;
long PPR = 16384*GearReduction;
///////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println(" Infineon hall sensor BLDC motor test! ");


  pinMode(ChA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
  pinMode(ChB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

  MyMotor.begin();
  // Adapt the following values according to the README if necessary
  MyMotor.MotorParam.MotorPolepairs =11;        // Amount of polepairs. If your motor has 8 poles, it has 4 pole PAIRS
  MyMotor.MotorParam.SensingMode = 1;           // If you use a Hallsensor set 1, for sensorless application 0
  MyMotor.MotorParam.PI_Reg_P = 0.01;           // P value for the PI-RPM regulator
  MyMotor.MotorParam.PI_Reg_I = 0.01;          // I value for the PI-RPM regulator
  MyMotor.configureBLDCMotor(MyMotor.MotorParam);


}

void loop() {

  updateStates();

  printStates();
  rotateMotor();

}

void printStates()
{
  Serial.print("current angle : ");
  Serial.print(angle);
//  Serial.print(" | Pulse : ");
//  Serial.print(pulse);
  Serial.print(" | Desired angle : ");
  Serial.print(desiredAngle);
  Serial.print(" | error : ");
  Serial.print(error);
  Serial.print(" | dutyCycle: ");
  Serial.print(dutyCycle);
  if(direction) Serial.println(" | CW");
  else          Serial.println(" | CCW");

}

void rotateMotor()
{
  MyMotor.setHallBLDCmotorDCspeed(direction,dutyCycle,WEAKENING);  
}


void updateDutyCycle()
{
  if(error > 3) dutyCycle = 50 + 20*(error/180.0f);
  else dutyCycle = 0;
}

void updateError()
{
  error = angle - desiredAngle;
  if(error > 0)
  {
    if(error < 180) direction = 1;
    else 
    {
      direction = 0;
      error = 360 - error;
    }
  }
  else
  {
    error = -1*error;
    if(error < 180) direction = 0;
    else 
    {
      direction = 1;
      error = 360 - error;
    }
  }
}


void updateAngle()
{
  if(pulse<0)pulse = PPR-1;//4*4096 from data sheet
  pulse = fmod(pulse,PPR);
//  pulse = pulse - PPR*(int)(pulse/PPR);
  angle = (pulse*360)/PPR;
}
void updateStates()
{
  updateAngle();
  updateError();
  updateDutyCycle();
}

void changeA()
{
  Anew = digitalRead(ChA);
  updatePulse();
}
void changeB()
{
  Bnew = digitalRead(ChB);
  updatePulse();
}

//void changeA() {
//  int a = digitalRead(encoderPinA);
//  int b = digitalRead(encoderPinB);
//  if (a == b) {
//    pulse--;
//  } else {
//    pulse++;
//  }
//}



void updatePulse(){
/* Pulse ++
 * A : HIGH-LOW  | B : HIGH-HIGH
 * A : LOW -LOW  | B : HIGH-LOW 
 * A : LOW -HIGH | B : LOW -LOW 
 * A : HIGH-HIGH | B : LOW -HIGH
 */
/* Pulse--
 * A : LOW  - HIGH  | B : HIGH - HIGH
 * A : LOW  - LOW   | B : LOW  - HIGH
 * A : HIGH - LOW   | B : LOW  - LOW 
 * A : HIGH - HIGH  | B : HIGH - LOW 
 */

       if(  Aold && !Anew &&  Bold  &&  Bnew ) pulse = pulse + 1.0;
  else if( !Aold && !Anew &&  Bold  && !Bnew ) pulse = pulse + 1.0;
  else if( !Aold &&  Anew && !Bold  && !Bnew ) pulse = pulse + 1.0;
  else if(  Aold &&  Anew && !Bold  &&  Bnew ) pulse = pulse + 1.0;
 
  else if(  Anew && !Aold &&  Bnew  &&  Bold ) pulse = pulse - 1.0;
  else if( !Anew && !Aold &&  Bnew  && !Bold ) pulse = pulse - 1.0;
  else if( !Anew &&  Aold && !Bnew  && !Bold ) pulse = pulse - 1.0;
  else if(  Anew &&  Aold && !Bnew  &&  Bold ) pulse = pulse - 1.0;

  Aold = Anew;
  Bold = Bnew;
}
