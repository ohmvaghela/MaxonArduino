#include <ros.h>
#include <std_msgs/String.h>
#include "exo_angle_control/ExoAngle.h"

ros::NodeHandle nh;
exo_angle_control::ExoAngle AnglePub;
ros::Publisher EncoderPub("EncoderTopic0", &AnglePub);

bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

float pulse = 0;
int angle = 0;

const byte ChA = 21;
const byte ChB = 20;

int GearReduction = 1;
float PPR = 16384*GearReduction;

void setup() {
  Serial.begin(115200);

  pinMode(ChA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChA), changeA, CHANGE);
  pinMode(ChB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ChB), changeB, CHANGE);

  nh.initNode();
  nh.advertise(EncoderPub);

}

void loop() {

  Serial.print("pulse : ");
  Serial.print(pulse);
  Serial.print(" | Angle : ");
  Serial.println(angle);
  updateAngle();

  PublishAngleFn();
  delay(1);
}

void PublishAngleFn()
{
  AnglePub.hipLeft = 0;
  AnglePub.hipRight = 0;
  AnglePub.kneeLeft = angle;
  AnglePub.kneeRight = 0;
  EncoderPub.publish(&AnglePub);
}


void updateAngle()
{
  if(pulse<0)pulse = PPR-1;//4*4096 from data sheet
  pulse = fmod(pulse,PPR);
//  pulse = pulse - PPR*(int)(pulse/PPR);
  angle = (pulse*360)/PPR;
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


       if(  Aold && !Anew &&  Bold  &&  Bnew ) pulse += 1;
  else if( !Aold && !Anew &&  Bold  && !Bnew ) pulse += 1;
  else if( !Aold &&  Anew && !Bold  && !Bnew ) pulse += 1;
  else if(  Aold &&  Anew && !Bold  &&  Bnew ) pulse += 1;

  else if(  Anew && !Aold &&  Bnew  &&  Bold ) pulse -= 1;
  else if( !Anew && !Aold &&  Bnew  && !Bold ) pulse -= 1;
  else if( !Anew &&  Aold && !Bnew  && !Bold ) pulse -= 1;
  else if(  Anew &&  Aold && !Bnew  &&  Bold ) pulse -= 1;

  Aold = Anew;
  Bold = Bnew;
}
