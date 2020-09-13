#include "intake.h"

Intake::Intake()
{
    Intake1 = new VictorSPX(19);
    Intake2 = new VictorSPX(14);
}

void Intake::initIntake()
{
    Intake2->Follow(*Intake1);

    Intake1->SetInverted(false);
    Intake2->SetInverted(false);

    Intake1->SetNeutralMode(NeutralMode::Brake);
    Intake2->SetNeutralMode(NeutralMode::Brake);
}
void Intake::runIntake(double L, double R)
{
    if(L != 0)
    {
        Intake1->Set(ControlMode::PercentOutput, -1*L);
    }
    else if(R != 0)
    {
        Intake1->Set(ControlMode::PercentOutput, R);
    }
    else
    {
        Intake1->Set(ControlMode::PercentOutput, 0);
    }
}
