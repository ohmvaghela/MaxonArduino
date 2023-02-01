/*
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include "exo_angle_control/ExoAngle.h"

ros::NodeHandle nh;

void messageCb(const exo_angle_control::ExoAngle &angles)
{
//    Serial.println("angles recived : ");
//    Serial.print(angles.hipLeft);
//    
     digitalWrite(LED_BUILTIN, HIGH - digitalRead(LED_BUILTIN)); // blink the led
}
ros::Subscriber<exo_angle_control::ExoAngle> sub("desiredAngleTopic", &messageCb);

void setup()
{
//    Serial.begin(57600);
    pinMode(LED_BUILTIN, OUTPUT);
    nh.initNode();
    nh.subscribe(sub);
}

void loop()
{
  
    nh.spinOnce();
    delay(1);
}
