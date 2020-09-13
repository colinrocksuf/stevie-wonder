#pragma once

#include <ctre/Phoenix.h>
#include "ControllerClass.h"
#include "frc/smartdashboard/Smartdashboard.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include <cmath>

class turret
{
  public:
  turret();
  void turn(double);
  void turnTo(double);
  void autoTurn(bool, double);

  private:
  TalonSRX *turretTalon;
};