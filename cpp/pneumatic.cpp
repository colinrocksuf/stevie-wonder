#include "pneumatic.h"

int Color = 0;
int Endgame = 0;
int Arm = 0;
int shooter = 0;

using namespace std;
using namespace frc;

Compressor compressor(0);
Solenoid endgameSingle1(3);
Solenoid endgameSingle2(2);
Solenoid shifterSingle(0);
Solenoid colorWheelSingle(1);
DoubleSolenoid armDouble1(4, 5);
DoubleSolenoid armDouble2(6, 7);


Pneumatics::Pneumatics()
{
    /*pCompressor = new Compressor(0);
    Endgame2Single = new Solenoid(3,0);
    Endgame1Single = new Solenoid(2,0);
    shifterSingle = new Solenoid(0,0);
    ColorWheelSingle = new Solenoid(1,0);
    ArmDouble1 = new DoubleSolenoid(0,6,7);
    ArmDouble2 = new DoubleSolenoid(0,4,5);*/
}
void Pneumatics::pColor(double ColorControl)
{
    if((ColorControl == 1) && (Color==0))
    {
        colorWheelSingle.Set(true);
        Color=1;
    
    }
    if((ColorControl==1)&& (Color==1))
    {
        colorWheelSingle.Set(false);
        Color=0;
    }
}
void Pneumatics::pEndgame(int EndgameUp, int EndgameDown)
{
    if (EndgameUp == 1)
    {
        cout << "intake pneumatics = true\n";
        endgameSingle1.Set(true);
        endgameSingle2.Set(true);
    }
    else if (EndgameDown == 1)
    {
        cout << "intake pneumatics = false\n";
        endgameSingle1.Set(false);
        endgameSingle2.Set(false);
    }
}
void Pneumatics::pArm(bool ArmDoubleControl, bool ArmDoubleControl2)
{
    if(ArmDoubleControl)
    {
        armDouble1.Set(DoubleSolenoid::kForward);
        armDouble2.Set(DoubleSolenoid::kForward);
    }
    if(ArmDoubleControl2)
    {
        armDouble1.Set(DoubleSolenoid::kReverse);
        armDouble2.Set(DoubleSolenoid::kReverse);
    }
}

void Pneumatics::arm(bool ArmDoubleControl)
{
    if (ArmDoubleControl)
    {
        armDouble1.Set(DoubleSolenoid::kForward);
    }
    if (!ArmDoubleControl)
    {
        armDouble1.Set(DoubleSolenoid::kReverse);
    }


}
void Pneumatics::pShifter(bool leftBumper, bool rightBumper)
{
    if (rightBumper)
    {
        cout << "shifted\n";
        shifterSingle.Set(true);
        cout << shifterSingle.Get() << endl;
    }
    else if (leftBumper)
    {
        cout << "not shifted\n";
        shifterSingle.Set(false);
        cout << shifterSingle.Get() << endl;
    }
}
