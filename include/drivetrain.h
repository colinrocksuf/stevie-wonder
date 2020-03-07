#pragma once

#include <ctre/Phoenix.h>
#include "frc/SpeedController.h"
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include "frc/kinematics/DifferentialDriveOdometry.h"
#include "frc/kinematics/DifferentialDriveWheelSpeeds.h"
#include "frc/trajectory/TrajectoryConfig.h"
#include "frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h"
#include "frc/drive/DifferentialDrive.h"
#include "ahrs.h"
#include "units/units.h"
#include <frc/Filesystem.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <wpi/Path.h>
#include <wpi/SmallString.h>
#include "frc/controller/RamseteController.h"
#include "frc2/Timer.h"
#include "wpi/math"

using namespace frc;
using namespace std;

class Drivetrain
{

public:
    Drivetrain();
    void pathweaverInit();
    void followPath(int, bool);
    Pose2d getPose(bool);
    Rotation2d getHeading();
    void resetEncoders();
    void resetOdometry(frc::Pose2d pose);
    void drive(double, double);
    void driveMotion(double distanceInMeters, double velocityInMeters, double accelerationInMeters);
    bool AutoDrive(double);

private:
    AHRS *gyro;
    frc2::Timer timer;
};