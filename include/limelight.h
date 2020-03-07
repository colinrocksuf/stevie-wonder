#pragma once

#include "ControllerClass.h"
#include "frc/smartdashboard/Smartdashboard.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"

class limelight
{
  public:
    limelight();

    void updateLimelight(),
    LEDMode(bool),
    setPipeline(int);

    bool hasTarget();
    double getHorizontalOffset(),
    getVerticalOffset();
  

    bool tv;
    double ty;
    double tx;

  private:
};