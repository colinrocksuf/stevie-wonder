/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>
#include "limelight.h"
#include "turret.h"
#include "shooter.h"
#include "drivetrain.h"
#include "sequencer.h"
#include "intake.h"
#include "endgame.h"
#include "pneumatic.h"
#include "auto.h"
#include "ControlPanelMatch.h"
#include <frc/smartdashboard/SmartDashboard.h>


using namespace std;
using namespace frc;

limelight *limelightClass;
turret *turretClass;
ControllerClass *controllerClass;
shooter *shooterClass;
Drivetrain *drivetrainClass;
Sequencer* sequencerClass;
Intake* intakeClass;
Pneumatics* pneumaticsClass;
endgame* endgameClass;
autoC* autoClass;
ControlPanelMatch* colorWheel;

double rpm;

void Robot::RobotInit() {

  autoClass = new autoC();

  limelightClass = new limelight();
  turretClass = new turret();
  controllerClass = new ControllerClass();
  shooterClass = new shooter();
  drivetrainClass = new Drivetrain();
  sequencerClass = new Sequencer();
  intakeClass = new Intake();
  pneumaticsClass = new Pneumatics();
  endgameClass = new endgame();

  intakeClass->initIntake();
  sequencerClass->initSequencer();
  limelightClass->setPipeline(6);

  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  SmartDashboard::PutData("Auto Modes", &m_chooser);

  SmartDashboard::PutNumber("RPM", rpm);
}

void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() 
{
  drivetrainClass->pathweaverInit();
  autoClass->autoInit();
}

void Robot::AutonomousPeriodic() {
  autoClass->threeBall();
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() 
{
  colorWheel->Update(controllerClass->iDPadStateC1());
  limelightClass->updateLimelight();
  limelightClass->LEDMode(false);
  controllerClass->Update();

  drivetrainClass->drive(controllerClass->dLeftStickYC1, controllerClass->dRightStickYC1);
  pneumaticsClass->pShifter(controllerClass->bLeftBumperPressedC1, controllerClass->bRightBumperPressedC1);
  pneumaticsClass->pEndgame(controllerClass->bAButtonPressedC2, controllerClass->bYButtonPressedC2);
  pneumaticsClass->pArm(controllerClass->bXButtonPressedC2, controllerClass->bBButtonPressedC2);
  if (controllerClass->bXButtonPressedC2)
  {
    pneumaticsClass->arm(true);
  }
  else if (controllerClass->bBButtonPressedC2)
  {
    pneumaticsClass->arm(false);
  }
  turretClass->turn(controllerClass->dRightStickXC2);
  intakeClass->runIntake(controllerClass->dLeftTriggerC2, controllerClass->dRightTriggerC2);
  endgameClass->endgameControl(-1*controllerClass->dLeftTriggerC1, -1*controllerClass->dRightTriggerC1);
  if (controllerClass->bRightBumperC2)
  {
    limelightClass->LEDMode(true);
    limelightClass->setPipeline(1);
    //currently shooting with linear formula -- test table of known values
    shooterClass->shootInitiation();
    turretClass->autoTurn(true, limelightClass->getHorizontalOffset());
    if (shooterClass->shooterReady() && (controllerClass->dRightTriggerC1 > .1))
    {
      sequencerClass->runSequencer(-1*.5, -1*.9);
    }
    else       
    {
      sequencerClass->runSequencer(0, 0);
    }
  }
  else if (controllerClass->bLeftBumperC2)
  {
    sequencerClass->runSequencer(controllerClass->dLeftStickYC2, controllerClass->dRightStickYC2);
    if (controllerClass->iDPadStateC2 == 90)
    {
      shooterClass->shootInitiation();
    }
    else if (controllerClass->iDPadStateC2 == 180)
    {
      shooterClass->shootFullCourt();
    }
    else if (controllerClass->iDPadStateC2 == 270)
    {
      shooterClass->shootSafeZone();
    }
    else
    {
      shooterClass->shoot(0, 0);
    }
  }
  else
  {
    shooterClass->shoot(0, 0);
    sequencerClass->runSequencer(controllerClass->dLeftStickYC2, controllerClass->dRightStickYC2);
  }
}

void Robot::TestPeriodic() 
{
  cout << rpm << endl;
  controllerClass->Update();
  limelightClass->updateLimelight();
  limelightClass->LEDMode(true);
  endgameClass->endgameControl(controllerClass->dLeftTriggerC1, controllerClass->dRightTriggerC1);
  intakeClass->runIntake(controllerClass->dLeftTriggerC2, controllerClass->dRightTriggerC2);
  sequencerClass->runSequencer(controllerClass->dLeftStickYC2, controllerClass->dRightStickYC2);
  turretClass->turn(controllerClass->dRightStickXC2);
  shooterClass->shoot(rpm, 0);
}

#ifndef RUNNING_FRC_TESTS
int main() { return StartRobot<Robot>(); }
#endif
