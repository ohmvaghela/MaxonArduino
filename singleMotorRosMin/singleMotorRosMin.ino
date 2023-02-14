///////////////////////////////////////////////////
// BLDC
#include <SD.h>
#include "IFX007T-Motor-Control.h"
// ROS
#include <ros.h>
#include "std_msgs/Empty.h"
#include "exo_angle_control/ExoAngle.h"

///////////////////////////////////////////////////
// BLDC
#define WEAKENING 0                // 0=normal, 1=weak (fast)
const float stepsPerDegree = 10.0; // Adjust this value based on your motor
// int direction = 1;                 // 1 CW // 0 CCW
uint8_t dutyCycle = 30;

// Create an instance of 'IFX007TMotorControl' called 'MyMotor'
IFX007TMotorControl MyMotor = IFX007TMotorControl();

/////////
// Encoder
float desiredAngle = 90;

const byte ChA = 21;
const byte ChB = 20;
/////////
// ROS
ros::NodeHandle nh;

int hipLeft = 0;
int hipRight = 0;
int kneeLeft = 0;
int kneeRight = 0;

void messageCb(const exo_angle_control::ExoAngle &angles)
{
    desiredAngle = angles.hipLeft;
    Serial.println("angles recived : ");
    Serial.print(desiredAngle);
    
    // digitalWrite(LED_BUILTIN, HIGH - digitalRead(LED_BUILTIN)); // blink the led
}
ros::Subscriber<exo_angle_control::ExoAngle> sub("desiredAngleTopic", &messageCb);


exo_angle_control::ExoAngle curAngle;
ros::Publisher pub_temp("curAngleTopic", &curAngle);

///////////////////////////////////////////////////

void setup()
{
    /////////
    // defult
    Serial.begin(115200);
    Serial.println(" Infineon hall sensor BLDC motor test! ");

    /////////
    // encoder
    pinMode(ChA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
    pinMode(ChB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

    /////////
    // BLDC
    MyMotor.begin();
    // Adapt the following values according to the README if necessary
    MyMotor.MotorParam.MotorPolepairs = 11; // Amount of polepairs. If your motor has 8 poles, it has 4 pole PAIRS
    MyMotor.MotorParam.SensingMode = 1;     // If you use a Hallsensor set 1, for sensorless application 0
    MyMotor.MotorParam.PI_Reg_P = 0.01;     // P value for the PI-RPM regulator
    MyMotor.MotorParam.PI_Reg_I = 0.01;     // I value for the PI-RPM regulator
    MyMotor.configureBLDCMotor(MyMotor.MotorParam);
    MyMotor.direction = 0;
    /////////
    // ROS
    nh.initNode();
    nh.subscribe(sub);
    nh.advertise(pub_temp);

}

void loop()
{
    // encoder
    updateStates();
    
    // BLDC
    rotateMotor();

    // ROS
    updateAndPublish();
    nh.spinOnce();
}

void updateAndPublish()
{
    curAngle.hipLeft = hipLeft;
    curAngle.hipRight = hipRight;
    curAngle.kneeLeft = kneeLeft;
    curAngle.kneeRight = kneeRight;
    pub_temp.publish(&curAngle);
}


void rotateMotor()
{
    MyMotor.setHallBLDCmotorDCspeed(MyMotor.direction, dutyCycle, WEAKENING);
}

void updateStates()
{
    MyMotor.updateAngle();
    MyMotor.updateError(desiredAngle);
    MyMotor.updateDutyCycle();
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
