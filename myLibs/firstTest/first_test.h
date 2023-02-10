#ifndef FIRST_TEST_H
#define FIRST_TEST_H

#include <Arduino.h>
class FIRSTTEST
{
public:
    //------------- User Functions --------------------------------------------------------------------------------
// new
    FIRSTTEST(void);
    ~FIRSTTEST(void);

    bool Aold = false;//
    bool Bold = false;//
    bool Anew = false;//
    bool Bnew = false;//

    long pulse = 0;//
    float angle = 0;//

    void updateAngle(void);
    void changeA();
    void changeB();
    void updatePulse();

// private:
    // bool StartupBLDC(bool dir); // Algorithm to start up the motor, as long as theres no BEMF
    // void changeBEMFspeed(bool direction, uint16_t dutycycle);
    // uint8_t UpdateHall(void);

};

#endif