#include "turret.h"

#include <iostream>
using namespace std;

#define TPort 49

turret::turret() 
{
    turretTalon = new TalonSRX(TPort);
    turretTalon->ConfigFactoryDefault();
    turretTalon->SetInverted(true);
    turretTalon->Config_kP(0, 0, 30);
    turretTalon->Config_kI(0, 0, 30); //P/100
    turretTalon->Config_kD(0, 0, 30); //100*P
    turretTalon->SetNeutralMode(NeutralMode::Brake);
}

void turret::turn(double speed)
{
    if (abs(speed) > .15)
    {
        turretTalon->Set(ControlMode::PercentOutput, speed/4);
    }
    else
    {
        turretTalon->Set(ControlMode::PercentOutput, 0);
    }

    
}

void turret::turnTo(double angle)
{
    turretTalon->Set(ControlMode::Position, angle);
}

void turret::autoTurn(bool state, double tx)
{
    bool direction;
    if (tx >= 0)
    {
        direction = true;
    }
    else
    {
        direction = false;
    }
    int correctedDirection = direction ? 1 : -1;
    cout << correctedDirection << endl;

    cout << "Turret speed: " << correctedDirection*(.15+(abs(tx)*.005)) << endl;
    if (state && (abs(tx) > 1))
    {
        turretTalon->Set(ControlMode::PercentOutput, correctedDirection*.175+(abs(tx)*.015));
        //to do: position control: currentangle+tx
    }
    else
    {
        turretTalon->Set(ControlMode::PercentOutput, 0);
    }
}







