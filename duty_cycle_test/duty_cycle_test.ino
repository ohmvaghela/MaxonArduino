// encoder
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

long pulse = 0;
float angle = 0;

const byte ChA = 21;
const byte ChB = 20;

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



void setup()
{
  // basic
  Serial.begin(115200);
  Serial.println(" Infineon hall sensor BLDC motor test! ");
  // BLDC
  MyMotor.begin();
  // Adapt the following values according to the README if necessary
  MyMotor.MotorParam.MotorPolepairs =11;        // Amount of polepairs. If your motor has 8 poles, it has 4 pole PAIRS
  MyMotor.MotorParam.SensingMode = 1;           // If you use a Hallsensor set 1, for sensorless application 0
  MyMotor.MotorParam.PI_Reg_P = 0.01;           // P value for the PI-RPM regulator
  MyMotor.MotorParam.PI_Reg_I = 0.01;          // I value for the PI-RPM regulator
  MyMotor.configureBLDCMotor(MyMotor.MotorParam);

  // Encoder
  pinMode(ChA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
  pinMode(ChB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

}



void loop() {

  // BLDC
//  MyMotor.setHallBLDCmotorDCspeed(direction,30,WEAKENING);
//  Serial.println(dutyCycle);
  // Encoder
  updateAngle();
  if(angle < 180 ) 
  {
    
    Serial.println(1);
    MyMotor.setHallBLDCmotorDCspeed(direction,30,WEAKENING);  
  }
  else
  {
    Serial.println(0);
    MyMotor.setHallBLDCmotorDCspeed(direction,0,WEAKENING);  
  }
//    Serial.println(angle);
  
}


// Encoder
void updateAngle()
{
  if(pulse<0)pulse = (16384);
  pulse = pulse%(16384);
  angle = (float)pulse*360/(16384);
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
  // some update shit
/*
 * A : U-D | B : U-U
 * A : D-D | B : U-D
 * A : D-U | B : D-D
 * A : U-U | B : D-U
 */

       if(  Aold && !Anew &&  Bold  &&  Bnew ) pulse++;
  else if( !Aold && !Anew &&  Bold  && !Bnew ) pulse++;
  else if( !Aold &&  Anew && !Bold  && !Bnew ) pulse++;
  else if(  Aold &&  Anew && !Bold  &&  Bnew ) pulse++;
 
  else if(  Anew && !Aold &&  Bnew  &&  Bold ) pulse--;
  else if( !Anew && !Aold &&  Bnew  && !Bold ) pulse--;
  else if( !Anew &&  Aold && !Bnew  && !Bold ) pulse--;
  else if(  Anew &&  Aold && !Bnew  &&  Bold ) pulse--;

  Aold = Anew;
  Bold = Bnew;
}
