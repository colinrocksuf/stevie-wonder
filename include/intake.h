#pragma once
#include <ctre/Phoenix.h>
#include <iostream>

using namespace std;
using namespace frc;

class Intake 
{
    public:
    Intake();
    void initIntake();
    void runIntake(double,double);
    private:
    VictorSPX * Intake1;
    VictorSPX * Intake2;
};  