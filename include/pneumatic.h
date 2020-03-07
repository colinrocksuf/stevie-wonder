#pragma once
#include "frc/Compressor.h"
#include "frc/Solenoid.h"
#include "frc/DoubleSolenoid.h"
#include <iostream>

using namespace frc;

class Pneumatics
{
public:
    Pneumatics();
    void pShooter(double);
    void pShifter(bool, bool);
    void pArm(bool, bool);
    void arm(bool);
    void pDrive(double);
    void pEndgame(int, int);
    void pColor(double);
private:
    /*Compressor* pCompressor;
    Solenoid* Endgame2Single;
    Solenoid* Endgame1Single;
    Solenoid* shifterSingle;
    Solenoid* ColorWheelSingle;
    DoubleSolenoid* ArmDouble1;
    DoubleSolenoid* ArmDouble2;*/



};

