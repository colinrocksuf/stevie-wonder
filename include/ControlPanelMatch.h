#ifndef CONTROLPANELMATCH_H
#define CONTROLPANELMATCH_H

#include <ctre/Phoenix.h>
#include <frc/TimedRobot.h>
#include <string>
#include <iostream>
#include <frc/DriverStation.h>
#include <frc/util/color.h>
#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"
#include <frc/smartdashboard/smartdashboard.h>
using namespace std;
using namespace frc;
class ControlPanelMatch
{
    public:
        ControlPanelMatch();
        bool SpinThree();
        bool SpinToColor();
        void RecColor();
        void StopAll();
        void Update(int);
    private:
        TalonSRX *snowBlower;
};


#endif