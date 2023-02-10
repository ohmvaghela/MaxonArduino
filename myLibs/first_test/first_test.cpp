/**
 * IFX007T-Motor-Control.cpp    -   Library for the Infineon BLDC Motor Control Shield with IFX007T Motor driver.
 *
 * Tested microcontroller boards:
 *    Arduino UNO
 *    XMC4700 Relax kit for 5V shields
 *
 * The Library supports the following operation modes:
 *    sensorless BEMF mode (BLDC)
 *    hallsensor mode (BLDC)
 *    3 unipolar motors (DC)
 *    1 bidirectional motor (DC)
 *    2 bidirectional motors (DC)
 */

#include "first_test.h"

/**
 * Constructor 1
 * This Constructor is called, if no arguments are given to it and thus takes the default pin assignment
 */
FIRSTTEST::FIRSTTEST(void)
{
}

IFX007TMotorControl::~IFX007TMotorControl(void)
{
}

void FIRSTTEST::updateAngle(void)
{
    if (pulse < 0)
        pulse = (16384);
    pulse = pulse % (16384);
    angle = (float)pulse * 360 / (16384);
}

void changeA();
{
    Anew = !Anew;
    updatePulse();
}
void changeB();
{
    Bnew = !Bnew;
    updatePulse();
}
void updatePulse();
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
