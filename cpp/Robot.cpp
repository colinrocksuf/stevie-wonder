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

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  drivetrainClass->pathweaverInit();
  autoClass->autoInit();

  /*m_autoSelected = m_chooser.GetSelected();
  m_autoSelected = SmartDashboard::GetString("Auto Selector", kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
    autoC->autoInit();
  } else {
    // Default Auto goes here
  }*/
}

void Robot::AutonomousPeriodic() {
  autoClass->threeBall();

  /*if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
    autoC->threeBall();
  } else {
    // Default Auto goes here
  }*/
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() 
{

  /*rpm = SmartDashboard::GetNumber("RPM", rpm);
  cout << rpm << endl;
  controllerClass->Update();
  limelightClass->updateLimelight();
  limelightClass->LEDMode(true);
  endgameClass->endgameControl(controllerClass->dLeftTriggerC1, controllerClass->dRightTriggerC1);
  intakeClass->runIntake(controllerClass->dLeftTriggerC2, controllerClass->dRightTriggerC2);
  sequencerClass->runSequencer(controllerClass->dLeftStickYC2, controllerClass->dRightStickYC2);
  turretClass->turn(controllerClass->dRightStickXC2);
  drivetrainClass->drive(controllerClass->dLeftStickYC1, controllerClass->dRightStickYC1);
  shooterClass->shoot(rpm, 0);*/
    
    limelightClass->updateLimelight();
    limelightClass->LEDMode(false);
    controllerClass->Update();

    //cout << "shooter ready: " << shooterClass->shooterReady() << endl;

    drivetrainClass->drive(controllerClass->dLeftStickYC1, controllerClass->dRightStickYC1);
    pneumaticsClass->pShifter(controllerClass->bLeftBumperPressedC1, controllerClass->bRightBumperPressedC1);
    //pneumaticsClass->pEndgame(controllerClass->bXButtonPressedC2, controllerClass->bBButtonPressedC2);
    pneumaticsClass->pArm(controllerClass->bXButtonPressedC2, controllerClass->bBButtonPressedC2);
    //test if statement:
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

    
    //shooterClass->shoot(false, limelightClass->getVerticalOffset());
    //cout << "left bumper state: " << controllerClass->bLeftBumperC2 << endl;
    //cout << "dpad state: " << controllerClass->iDPadStateC2 << endl;
    if (controllerClass->bRightBumperC2)
    {
        limelightClass->LEDMode(true);
        limelightClass->setPipeline(1);
        //shoot with linear formula OR table (idk what to use yet)
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
  /*if (controllerClass->bLeftBumperC2)
    {
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
    }*/
}

#ifndef RUNNING_FRC_TESTS
int main() { return StartRobot<Robot>(); }
#endif
