#include "drivetrain.h"
#include <iostream>

using namespace std;
using namespace frc;

//these 3 values are automatically found from using robot characterization
auto ks = 0.151_V; //static friction
auto kv = 4.28 * 1_V * 1_s / 1_m; //velocity component
auto ka = 0.199 * 1_V * 1_s * 1_s / 1_m; //acceleration, kinda doesn't matter?
double kP = 0.161; // proportional gain (REDO THIS!! Talon output is 1023 not 1)
auto trackWidth = 0.695325_m; //horizontal distance between wheels
auto kMaxSpeed = 1_mps;
auto kMaxAcceleration = .5_mps_sq;

constexpr double kInch = (2048*18.75)/(6*wpi::math::pi);

WPI_TalonFX leftMaster(11);
WPI_TalonFX leftSlave(12);
WPI_TalonFX rightMaster(13);
WPI_TalonFX rightSlave(14);

DifferentialDrive drivetrain(leftMaster, rightMaster);

DifferentialDriveKinematics kinematics(trackWidth);
Trajectory trajectory;
RamseteController ramsete(2.0, 0.7);
Pose2d pose;
Rotation2d gyroAngle;
DifferentialDriveOdometry odometry(gyroAngle, pose);
SimpleMotorFeedforward<units::meters> feedforward(ks, kv, ka);

Drivetrain::Drivetrain()
{
    gyro = new AHRS(SPI::Port::kMXP);

    //left motor cluster
    leftMaster.ConfigFactoryDefault();
    leftMaster.SetInverted(TalonFXInvertType::CounterClockwise);
    leftMaster.Config_kF(0, 0.048927863, 30);
    leftMaster.Config_kP(0, kP, 30);
    leftMaster.ConfigMotionAcceleration(1*38400/(.1524*wpi::math::pi)/10);
    leftMaster.ConfigMotionCruiseVelocity(.5*38400/(.1524*wpi::math::pi)/10);
    //leftMaster.SetNeutralMode(Brake);
    
    
    leftSlave.ConfigFactoryDefault();
    leftSlave.Follow(leftMaster);
    leftSlave.SetInverted(TalonFXInvertType::FollowMaster);
    leftSlave.Config_kF(0, 0.048927863, 30);
    leftSlave.Config_kP(0, kP, 30);
    //leftSlave.SetNeutralMode(Brake);

    //right motor cluster
    rightMaster.ConfigFactoryDefault();
    rightMaster.SetInverted(TalonFXInvertType::CounterClockwise);
    rightMaster.Config_kF(0, 0.048927863, 30);
    rightMaster.Config_kP(0, kP, 30);
    //rightMaster.SetNeutralMode(Brake);

    
    rightSlave.ConfigFactoryDefault();
    rightSlave.Follow(rightMaster);
    rightSlave.SetInverted(TalonFXInvertType::FollowMaster);
    rightSlave.Config_kF(0, 0.048927863, 30);
    rightSlave.Config_kP(0, kP, 30);

    drivetrain.SetRightSideInverted(true);
    

    //rightSlave.SetNeutralMode(Brake);
}

void Drivetrain::pathweaverInit()
{
    TrajectoryConfig config(kMaxSpeed, kMaxAcceleration);
    //make sure path is found in rio ftp: /home/lvuser/deploy/paths
    gyro->Reset();
    resetEncoders();
    gyro->ResetDisplacement();
    timer.Reset();
    timer.Start();
    resetOdometry(pose);

    config.SetKinematics(kinematics);
    config.AddConstraint(DifferentialDriveVoltageConstraint(feedforward, kinematics, 10_V)); 
    cout << "Pathweaver initialized.\n";
    
}

void Drivetrain::followPath(int path, bool inverted)
{
    wpi::SmallString<64> deployDirectory;
    filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "paths");
    switch(path)
    {
        case 1:
            wpi::sys::path::append(deployDirectory, "8ball1.wpilib.json");
        case 2:
            wpi::sys::path::append(deployDirectory, "8ball2.wpilib.json");
        case 3:
            wpi::sys::path::append(deployDirectory, "8ball3.wpilib.json");
    }
    
    trajectory = Trajectory(TrajectoryUtil::FromPathweaverJson(deployDirectory));
    auto time = timer.Get();
    //cout << "time: " << time << endl;
    pose = getPose(inverted);
    ChassisSpeeds adjustedSpeeds = ramsete.Calculate(pose, trajectory.Sample(time));
    //cout << "chassis speeds: " << adjustedSpeeds.vx << endl;
    DifferentialDriveWheelSpeeds adjustedWheelSpeeds = kinematics.ToWheelSpeeds(adjustedSpeeds);
    if(!inverted)
    {
        leftMaster.Set(ControlMode::Velocity, adjustedWheelSpeeds.left.to<double>()*38400/(.1524*wpi::math::pi)/10);
        //cout << adjustedWheelSpeeds.left.to<double>() << " " << adjustedWheelSpeeds.left.to<double>()*38400/(.1524*wpi::math::pi)/10 << endl;
        rightMaster.Set(ControlMode::Velocity, adjustedWheelSpeeds.right.to<double>()*38400/(.1524*wpi::math::pi)/10);
        //cout << adjustedWheelSpeeds.right.to<double>() << " " <<  adjustedWheelSpeeds.right.to<double>()*38400/(.1524*wpi::math::pi)/10 << endl;
    }
    else if (inverted)
    {
        leftMaster.Set(ControlMode::Velocity, -1*adjustedWheelSpeeds.right.to<double>()*38400/(.1524*wpi::math::pi)/10);
        //cout << adjustedWheelSpeeds.left.to<double>() << " " << adjustedWheelSpeeds.left.to<double>()*38400/(.1524*wpi::math::pi)/10 << endl;
        rightMaster.Set(ControlMode::Velocity, -1*adjustedWheelSpeeds.left.to<double>()*38400/(.1524*wpi::math::pi)/10);
        //cout << adjustedWheelSpeeds.right.to<double>() << " " <<  adjustedWheelSpeeds.right.to<double>()*38400/(.1524*wpi::math::pi)/10 << endl;
    }
    
}

Pose2d Drivetrain::getPose(bool inverted)
{
    double leftDistanceInMeters = (inverted ? -1 : 1) * leftMaster.GetSelectedSensorPosition(0)/38400*.1524*wpi::math::pi;
    double rightDistanceInMeters = (inverted ? -1 : 1) * rightMaster.GetSelectedSensorPosition(0)/38400*.1524*wpi::math::pi; //*-1
    cout << "current left in meters: " << 1_m*leftDistanceInMeters << endl;
    cout << "current right in meters: " << 1_m*rightDistanceInMeters << endl;
    cout << "left encoder position: " << leftMaster.GetSelectedSensorPosition(0)*wpi::math::pi*.1524/38400 << endl;
    odometry.Update(getHeading(), 1_m*leftDistanceInMeters, 1_m*rightDistanceInMeters);
    return odometry.GetPose();
}

Rotation2d Drivetrain::getHeading()
{
    cout << "angle " << -1*gyro->GetAngle() << endl;
    return Rotation2d(-1*units::degree_t(gyro->GetAngle()));
    //return Rotation2d(-1*units::degree_t(remainder(gyro->GetAngle(), 360)));
}

void Drivetrain::resetEncoders()
{
    leftMaster.SetSelectedSensorPosition(0, 0, 30);
    rightMaster.SetSelectedSensorPosition(0, 0, 30);
}

void Drivetrain::resetOdometry(frc::Pose2d pose)
{
    odometry.ResetPosition(pose, getHeading());
}

void Drivetrain::drive(double left, double right)
{
    drivetrain.TankDrive(left, right);
}

void Drivetrain::driveMotion(double distanceInMeters, double velocityInMeters, double accelerationInMeters)
{
    leftMaster.ConfigMotionAcceleration(accelerationInMeters*38400/(.1524*wpi::math::pi)/10);
    leftMaster.ConfigMotionCruiseVelocity(velocityInMeters*38400/(.1524*wpi::math::pi)/10);
    rightMaster.ConfigMotionAcceleration(accelerationInMeters*38400/(.1524*wpi::math::pi)/10);
    rightMaster.ConfigMotionCruiseVelocity(velocityInMeters*38400/(.1524*wpi::math::pi)/10);

    leftMaster.Set(ControlMode::MotionMagic, distanceInMeters*(38400*.1524*wpi::math::pi));
    rightMaster.Set(ControlMode::MotionMagic, distanceInMeters*(38400*.1524*wpi::math::pi));

    /*double difference = abs(leftMaster.GetActiveTrajectoryPosition() - leftMaster.GetSelectedSensorPosition());
    double threshold = 180*distanceInMeters;
    if (difference < threshold)
    {
        return true;
    }
    else
    {
        return false;
    }*/
}

bool Drivetrain::AutoDrive(double dist)
{
    leftMaster.ConfigMotionCruiseVelocity(9*kInch*12, 30);
    leftMaster.ConfigMotionAcceleration(9*kInch*12, 30);
    rightMaster.ConfigMotionCruiseVelocity(4.5*kInch*12, 30);
    rightMaster.ConfigMotionAcceleration(4.5*kInch*12, 30);
    leftMaster.Set(ControlMode::MotionMagic, kInch*dist);
    rightMaster.Set(ControlMode::MotionMagic, kInch*dist);

}