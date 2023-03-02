//hipLeft
 ///////////////////////////////////////////////////
// BLDC
#include <SD.h>
#include "IFX007T-Motor-Control.h"
// ROS
#include <ros.h>
#include "std_msgs/Empty.h"
#include "exo_angle_control/ExoAngle.h"
#include "exo_angle_control/EncoderHL.h"

///////////////////////////////////////////////////
// BLDC
#define WEAKENING 0                // 0=normal, 1=weak (fast)
const float stepsPerDegree = 10.0; // Adjust this value based on your motor
int direction = 1;                 // 1 CW // 0 CCW
uint8_t dutyCycle = 80;

// Create an instance of 'IFX007TMotorControl' called 'MyMotor'
IFX007TMotorControl MyMotor = IFX007TMotorControl();

/////////
// Encoder
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

float pulse = 10.0;
float angle = 0;
float desiredAngle = 0;
float error;

const byte ChA = 21;
const byte ChB = 20;

long GearReduction = 150;
long PPR = 16384 * GearReduction;

/////////
// ROS
ros::NodeHandle nh;

void messageCb(const exo_angle_control::ExoAngle &angles)
{
    desiredAngle = angles.hipRight;
}
ros::Subscriber<exo_angle_control::ExoAngle> sub("desiredAngleTopic", &messageCb);
exo_angle_control::EncoderHL curAngle;
ros::Publisher pub_temp("updateHLTopic", &curAngle);

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
    delay(1);
}

void updateAndPublish()
{
    curAngle.angle = angle;
    pub_temp.publish(&curAngle);
}


void rotateMotor()
{
    MyMotor.setHallBLDCmotorDCspeed(1, 150, WEAKENING);
}

void updateDutyCycle()
{
    if (error > 3)
        dutyCycle = 30 + 20 * (error / 180.0f);
    else
        dutyCycle = 0;
}

void updateError()
{
    error = angle - desiredAngle;
    if (error > 0)
    {
        if (error < 180)
            direction = 1;
        else
        {
            direction = 0;
            error = 360 - error;
        }
    }
    else
    {
        error = -1 * error;
        if (error < 180)
            direction = 0;
        else
        {
            direction = 1;
            error = 360 - error;
        }
    }
}

void updateAngle()
{
    if (pulse < 0)
        pulse = PPR - 1; // 4*4096 from data sheet
    pulse = fmod(pulse, PPR);
    //  pulse = pulse - PPR*(int)(pulse/PPR);
    angle = (pulse * 360) / PPR;
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

void updatePulse()
{
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

    if (Aold && !Anew && Bold && Bnew)
        pulse = pulse + 1.0;
    else if (!Aold && !Anew && Bold && !Bnew)
        pulse = pulse + 1.0;
    else if (!Aold && Anew && !Bold && !Bnew)
        pulse = pulse + 1.0;
    else if (Aold && Anew && !Bold && Bnew)
        pulse = pulse + 1.0;

    else if (Anew && !Aold && Bnew && Bold)
        pulse = pulse - 1.0;
    else if (!Anew && !Aold && Bnew && !Bold)
        pulse = pulse - 1.0;
    else if (!Anew && Aold && !Bnew && !Bold)
        pulse = pulse - 1.0;
    else if (Anew && Aold && !Bnew && Bold)
        pulse = pulse - 1.0;

    Aold = Anew;
    Bold = Bnew;
}
