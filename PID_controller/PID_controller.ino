///////////////////////////////////////////////////
//BLDC
#include <SD.h>
#include "IFX007T-Motor-Control.h"
#define WEAKENING  0           // 0=normal, 1=weak (fast)
const float stepsPerDegree = 10.0; // Adjust this value based on your motor
String input = "+";
int direction = 1;
//float angle[6] = {1, 50, 200, 300, 600, 800} ;
float rpm=0;
uint8_t dutyCycle = 30;
//Create an instance of 'IFX007TMotorControl' called 'MyMotor'
IFX007TMotorControl MyMotor = IFX007TMotorControl();
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//Encoder
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

float pulse = 0;
float angle = 0;
float desiredAngle = 120;
float error;

const byte ChA = 21;
const byte ChB = 20;

int GearReduction = 1;
float PPR = 16384*GearReduction;
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
  
  Serial.print("error : ");
  Serial.println(error);

  rotateMotor();
  desiredAngle++;

}

void rotateMotor()
{
  MyMotor.setHallBLDCmotorDCspeed(direction,dutyCycle,WEAKENING);  
}


void updateDutyCycle();
{
  float b = 180.0f;
  dutyCycle = 30 + 20*(error/b);
}

void updateError()
{
  float a = 360.0f;
  error = desiredAngle - angle;
  error = error%a;
  if(error>180) 
  {
    error =180 - (error - (error/180)*180 );
    direction = !direction;
  }
}

void updateAngle()
{

  if(pulse<0)pulse = PPR;//4*4096 from data sheet
  pulse = pulse%PPR;
  angle = pulse*360/PPR;

}

void updateStates()
{
  updateAngle();
  updateError();
  updateDutyCycle();
}

void changeA()
{
  Anew = !Anew;
  updatePulse();
}
void changeB()
{
  Bnew = !Bnew;
  updatePulse();
}


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
