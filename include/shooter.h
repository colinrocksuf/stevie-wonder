#pragma once

#include <ctre/Phoenix.h>
#include <stdlib.h>
#include "ControllerClass.h"
#include "frc/smartdashboard/Smartdashboard.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"

class shooter
{
  public:
  shooter();

  double motorOutput;

  void shoot(double speed, double ty);
  void shootInitiation();
  void shootFullCourt();
  void shootSafeZone();
  bool shooterReady();

  private:
  //TalonFX *shooter1;
  //TalonFX *shooter2;
};