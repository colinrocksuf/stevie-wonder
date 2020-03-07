#pragma once
#include <ctre/Phoenix.h>
#include <iostream>

using namespace std;
using namespace frc;

class Sequencer 
{
    public:
    Sequencer();
    void initSequencer();
    void runSequencer(double sequencer,double feeder);
    private:
    VictorSPX * sequencer;
    VictorSPX * ballFeeder;
};