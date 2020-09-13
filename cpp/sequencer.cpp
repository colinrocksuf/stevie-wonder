#include "sequencer.h"

Sequencer::Sequencer()
{
    sequencer = new VictorSPX(23);
    ballFeeder = new VictorSPX(52);
}

void Sequencer::initSequencer()
{
    sequencer->SetInverted(false);
    ballFeeder->SetInverted(true);

    sequencer->SetNeutralMode(NeutralMode::Brake);
    ballFeeder->SetNeutralMode(NeutralMode::Brake);
}
void Sequencer::runSequencer(double sequence, double feed)
{
    sequencer->Set(ControlMode::PercentOutput, sequence);
    ballFeeder->Set(ControlMode::PercentOutput, feed);
}