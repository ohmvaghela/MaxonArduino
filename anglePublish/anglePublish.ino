// encoder
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

float pulse = 0;
int angle = 0;

const byte ChA = 21;
const byte ChB = 20;

int GearReduction = 1;
float PPR = 16384 * GearReduction;

// ros
#include <ros.h>
#include "exo_angle_control/EncoderKL.h"
#include "exo_angle_control/EncoderHL.h"
#include "exo_angle_control/EncoderHR.h"
ros::NodeHandle nh;

exo_angle_control::EncoderKL curAngle0;
ros::Publisher pub_temp0("updateKLTopic", &curAngle0);
exo_angle_control::EncoderHL curAngle1;
ros::Publisher pub_temp1("updateHLTopic", &curAngle1);
float desiredAngle = 0;
void messageCb(const exo_angle_control::EncoderHR &curAngle2)
{
    desiredAngle = curAngle1.angle;
}
ros::Subscriber<exo_angle_control::EncoderHR> sub("desiredAngleTopic", &messageCb);

void setup()
{
    // general
    Serial.begin(57600);

    // encoder
    pinMode(ChA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
    pinMode(ChB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

    // ros
    pinMode(LED_BUILTIN, OUTPUT);
    nh.initNode();
    nh.advertise(pub_temp0);
    nh.advertise(pub_temp1);
    nh.subscribe(sub);

    // nh.subscribe(sub);
}

void loop()
{
    // encoder
    updateAngle();

    // ros
    updateAndPublish();
    nh.spinOnce();
    delay(1);
}

void updateAndPublish()
{
    curAngle0.angle = angle;
    pub_temp0.publish(&curAngle0);
    curAngle1.angle = angle;
    pub_temp1.publish(&curAngle1);
}

void updateAngle()
{
    if (pulse < 0)
        pulse = PPR - 1; // 4*4096 from data sheet
    pulse = fmod(pulse, PPR);
    //  pulse = pulse - PPR*(int)(pulse/PPR);
    angle = (pulse * 360) / PPR;
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
        pulse += 1;
    else if (!Aold && !Anew && Bold && !Bnew)
        pulse += 1;
    else if (!Aold && Anew && !Bold && !Bnew)
        pulse += 1;
    else if (Aold && Anew && !Bold && Bnew)
        pulse += 1;

    else if (Anew && !Aold && Bnew && Bold)
        pulse -= 1;
    else if (!Anew && !Aold && Bnew && !Bold)
        pulse -= 1;
    else if (!Anew && Aold && !Bnew && !Bold)
        pulse -= 1;
    else if (Anew && Aold && !Bnew && Bold)
        pulse -= 1;

    Aold = Anew;
    Bold = Bnew;
}
