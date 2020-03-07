#include "endgame.h"
#include "iostream"

using namespace std;

VictorSPX endgameMaster(31);
VictorSPX endgameSlave(32);

endgame::endgame()
{
    endgameMaster.SetInverted(false);
    endgameMaster.SetNeutralMode(NeutralMode::Brake);

    endgameSlave.SetInverted(true);
    endgameSlave.SetNeutralMode(NeutralMode::Brake);
}

void endgame::endgameControl(double speed1, double speed2)
{   
    if (abs(speed1) > .1)
    {
        endgameMaster.Set(ControlMode::PercentOutput, speed1);
    }
    else
    {
        endgameMaster.Set(ControlMode::PercentOutput, 0);
    }
    if (abs(speed1) > .1)
    {
        endgameSlave.Set(ControlMode::PercentOutput, speed1);
    }
    else
    {
        endgameSlave.Set(ControlMode::PercentOutput, 0);
    }
}