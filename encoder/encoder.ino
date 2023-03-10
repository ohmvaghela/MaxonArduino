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

}

void loop() {

  Serial.print("pulse : ");
  Serial.print(pulse);
  Serial.print(" | Angle : ");
  Serial.println(angle);
  updateAngle();
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



















#include <ros.h>
#include <std_msgs/String.h>
#include "exo_angle_control/ExoAngle.h"

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter2", &str_msg);

void messageCb(const exo_angle_control::ExoAngle &angles)
{
  // do nothing
}
ros::Subscriber<exo_angle_control::ExoAngle> sub("desiredAngleTopic", &messageCb);


char hello[13] = "hello world!";

void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
}

void loop()
{
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
}