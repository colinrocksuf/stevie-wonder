#include "auto.h"

using namespace std;
using namespace frc;

limelight *limelightC;
turret *turretC;
ControllerClass *controllerC;
shooter *shooterC;
Drivetrain *drivetrainC;
Sequencer* sequencerC;
Intake* intakeC;
Pneumatics* pneumaticsC;

autoC::autoC()
{
    limelightC = new limelight();
    turretC = new turret();
    controllerC = new ControllerClass();
    shooterC = new shooter();
    drivetrainC = new Drivetrain();
    sequencerC = new Sequencer();
    intakeC = new Intake();
    pneumaticsC = new Pneumatics();
}

void autoC::autoInit()
{
    autoTimer.Start();
    autoTimer.Reset();
}

void autoC::threeBall()
{
    pneumaticsC->arm(true);
    intakeC->runIntake(.5, 0);
    if (autoTimer.Get() < 8.5_s)
    {
        limelightC->updateLimelight();
        limelightC->LEDMode(true);
        drivetrainC->drive(0, 0);
    
        shooterC->shootInitiation();
        if (shooterC->shooterReady())
        {
            sequencerC->runSequencer(-1*.5, -1*.3);
        }
        else       
        {
            sequencerC->runSequencer(0, 0);
        }
    }
    else
    {
        sequencerC->runSequencer(0, 0);
        shooterC->shoot(0, 0);
    }
    
    if (autoTimer.Get() > 13.5_s)
    {
        drivetrainC->drive(.2, .2);
    }
}

void autoC::eightBall()
{
    pneumaticsC->arm(true);
    intakeC->runIntake(.5, 0);
    if (autoTimer.Get() < 4_s)
    {
        limelightC->updateLimelight();
        limelightC->setPipeline(0);
        limelightC->LEDMode(true);
        drivetrainC->drive(0, 0);
    
        shooterC->shootInitiation();
        if (shooterC->shooterReady())
        {
            sequencerC->runSequencer(-1*.5, -1*.3);
        }
        else       
        {
            sequencerC->runSequencer(0, 0);
        }
    }
    else if (autoTimer.Get() < 6_s)
    {
        sequencerC->runSequencer(7.50, 0);
        //drivetrainC->driveMotion();
        drivetrainC->AutoDrive(-1*283.5);
    }
    else if(autoTimer.Get() < 9_s)
    {
        drivetrainC->AutoDrive(0);
    }
    else
    {
        if (shooterC->shooterReady())
        {
            sequencerC->runSequencer(-1*.5, -1*.3);
        }
        else       
        {
            sequencerC->runSequencer(0, 0);
        }
    }   
}

void autoC::colinEightBall() // car work -- test inverted pathweaver && do position turret
{
    limelightC->updateLimelight();
    pneumaticsC->arm(true);
    intakeC->runIntake(1, 0);
    shooterC->shootSafeZone();

    if (autoTimer.Get() < 2.125_s)
    {
        drivetrainC->followPath(1, false);
        turretC->turnTo(1);
    }

    if (autoTimer.Get() < 5_s)
    {
        turretC->autoTurn(true, limelightC->getHorizontalOffset());
        if (shooterC->shooterReady())
        {
            sequencerC->runSequencer(-1*.5, -1*.3);
        }
        else       
        {
            sequencerC->runSequencer(0, 0);
        }
    }
    else
    {
        sequencerC->runSequencer(0, 0);
        shooterC->shoot(0, 0);
    }

    if (autoTimer.Get() < 7.631_s)
    {
        drivetrainC->followPath(2, false);
        turretC->turnTo(1);
    }

    if (autoTimer.Get() == 7.631_s)
    {
        drivetrainC->pathweaverInit();
    }

    if (autoTimer.Get() < 10.262_s)
    {
        drivetrainC->followPath(3, true);
    }

    if (autoTimer.Get() > 10.262_s)
    {
        turretC->autoTurn(true, limelightC->getHorizontalOffset());
        if (shooterC->shooterReady())
        {
            sequencerC->runSequencer(-1*.5, -1*.3);
        }
        else       
        {
            sequencerC->runSequencer(0, 0);
        }
    }
}