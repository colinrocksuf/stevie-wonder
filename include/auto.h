#pragma once

#include <ctre/Phoenix.h>

#include "drivetrain.h"
#include "pneumatic.h"
#include "limelight.h"
#include "shooter.h"
#include "intake.h"
#include "sequencer.h"
#include "turret.h"


using namespace frc;
using namespace std;

class autoC
{

public:
    autoC();

    void autoInit();
    void threeBall();
    void eightBall();
    void colinEightBall();
    void driveIntake();
private:
    frc2::Timer autoTimer;
};