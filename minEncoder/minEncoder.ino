///////////////////////////////////////////////////
//BLDC
#include <SD.h>
#include "IFX007T-Motor-Control.h"

//Create an instance of 'IFX007TMotorControl' called 'MyMotor'
IFX007TMotorControl MyMotor = IFX007TMotorControl();

#define WEAKENING  0           // 0=normal, 1=weak (fast)
const float stepsPerDegree = 10.0; // Adjust this value based on your motor
uint8_t dutyCycle = 30;

///////////////////////////////////////////////////

///////////////////////////////////////////////////
//Encoder
float desiredAngle = 90;

const byte ChA = 21;
const byte ChB = 20;
///////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println(" Infineon hall sensor BLDC motor test! ");


  MyMotor.begin();
  // Adapt the following values according to the README if necessary
  MyMotor.MotorParam.MotorPolepairs =11;        // Amount of polepairs. If your motor has 8 poles, it has 4 pole PAIRS
  MyMotor.MotorParam.SensingMode = 1;           // If you use a Hallsensor set 1, for sensorless application 0
  MyMotor.MotorParam.PI_Reg_P = 0.01;           // P value for the PI-RPM regulator
  MyMotor.MotorParam.PI_Reg_I = 0.01;          // I value for the PI-RPM regulator
  MyMotor.configureBLDCMotor(MyMotor.MotorParam);
  MyMotor.direction = 0;


  pinMode(ChA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChA),changeA, CHANGE);
  pinMode(ChB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);
}

void loop() {

  updateStates();
  rotateMotor();

}

void rotateMotor()
{
  MyMotor.setHallBLDCmotorDCspeed(MyMotor.direction,dutyCycle,WEAKENING);  
}

void updateStates()
{
  MyMotor.updateAngle();
  MyMotor.updateError(desiredAngle);
  dutyCycle = MyMotor.updateDutyCycle();
  // updateAngle();
  // updateError();
  // updateDutyCycle();

}

void changeA()
{
  MyMotor.Anew = !MyMotor.Anew;
  MyMotor.updatePulse();
}
void changeB()
{
  MyMotor.Bnew = !MyMotor.Bnew;
  MyMotor.updatePulse();
}
