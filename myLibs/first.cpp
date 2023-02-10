
bool Aold = false;
bool Bold = false;
bool Anew = false;
bool Bnew = false;

long pulse = 0;
float angle = 0;
uint8_t duty = 0;
int desired_angle = 30;
int error = 0;

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

void updatePulse()
{
    // some update shit
    /*
     * A : U-D | B : U-U
     * A : D-D | B : U-D
     * A : D-U | B : D-D
     * A : U-U | B : D-U
     */

    if (Aold && !Anew && Bold && Bnew)
        pulse++;
    else if (!Aold && !Anew && Bold && !Bnew)
        pulse++;
    else if (!Aold && Anew && !Bold && !Bnew)
        pulse++;
    else if (Aold && Anew && !Bold && Bnew)
        pulse++;

    else if (Anew && !Aold && Bnew && Bold)
        pulse--;
    else if (!Anew && !Aold && Bnew && !Bold)
        pulse--;
    else if (!Anew && Aold && !Bnew && !Bold)
        pulse--;
    else if (Anew && Aold && !Bnew && Bold)
        pulse--;

    Aold = Anew;
    Bold = Bnew;
}