#include "shooter.h"
#include <iostream>

using namespace std;

ControllerClass *controller;
TalonFX shooter1(61);
TalonFX shooter2(62);

shooter::shooter()
{
    motorOutput = 0;

    shooter1.ConfigFactoryDefault();
    shooter1.SetInverted(TalonFXInvertType::CounterClockwise);
    /*shooter1.ConfigNominalOutputForward(0, 30);
	shooter1.ConfigNominalOutputReverse(0, 30);
	shooter1.ConfigPeakOutputForward(1, 30);
	shooter1.ConfigPeakOutputReverse(-1, 30);*/
    shooter1.Config_kF(0, 0.056209, 30); //https://www.chiefdelphi.com/t/falcon-500-velocity-control-loop-issue/372901/6 see this
    shooter1.Config_kP(0, .6, 30);
    shooter1.Config_kI(0, 0.001, 30);
    shooter1.Config_kD(0, 6, 30);

    shooter2.ConfigFactoryDefault();
    shooter2.SetInverted(TalonFXInvertType::Clockwise);
	/*shooter2.ConfigNominalOutputForward(0, 30);
	shooter2.ConfigNominalOutputReverse(0, 30);
	shooter2.ConfigPeakOutputForward(1, 30);
	shooter2.ConfigPeakOutputReverse(-1, 30);*/
    shooter2.Config_kF(0, 0.056209, 30);
    shooter2.Config_kP(0, .6, 30);
    shooter2.Config_kI(0, 0.001, 30);
    shooter2.Config_kD(0, 6, 30);
}

void shooter::shoot(double speed, double ty)
{
    double h = 75.452, //delta Y in inches.
    pi = 3.1415926535,
    g = 32.2, //accel. due to gravity in feet
    d = (h/atan((pi*(29.4+ty))/180))/12, //distance in ft
    S = sqrt(pow((d),2)-pow((h/12),2)), //horizontal distance in feet
    a = pi*55/180; // launch angle in radians

    double ballSpeed = sqrt((-1*g * pow(S,2)) / (2 * pow(cos(a),2) * (h/12 - S * tan(a))));
    //sqrt((-32.2*40^2)/(0.657980*(71.25/12 - 1.428148*40)))/10/1.0471975512*2048*2
     //Needed ball feet per second converted to native encoder values
    //(7.2*(sqrt((-32.2*pow(S,2))/(.657979*(6.25-S*1.428148)))))/397; - old
    //cout << d << endl;
    //cout << sqrt((-32.2*pow(S,2))/(0.657980*(75.452/12 - 1.428148*S)))*195.5695940706 << endl;
    //sqrt((-32.2*pow(S,2))/(0.657980*(75.452/12 - 1.428148*S)))*195.5695940706
    //motorOutput = sqrt((-32.2*pow(S,2))/(0.657980*(75.452/12 - 1.428148*S)))*195.5695940706 + 2000 + 50*S-10; test this
    //6950 from initiation
    //10250 from full ct
    //cout << "Horizontal Distance: " << S << " Initial Velocity Required: " << ballSpeed << endl << " Current Motor Output: " << motorOutput << endl;
    /*if(state == true)
    {
        
        shooter1.Set(ControlMode::Velocity, motorOutput);
        shooter2.Set(ControlMode::Velocity, motorOutput);
    }
    else if (state == false)
    {
        shooter1.Set(ControlMode::Velocity, 0);
        shooter2.Set(ControlMode::Velocity, 0);
    }*/
    motorOutput = speed;
    shooter1.Set(ControlMode::Velocity, speed);
    shooter2.Set(ControlMode::Velocity, speed);
    
}

void shooter::shootInitiation()
{
    motorOutput = 7200;
    shooter1.Set(ControlMode::Velocity, 7200);
    shooter2.Set(ControlMode::Velocity, 7200);
}

void shooter::shootFullCourt()
{
    motorOutput = 10375;
    shooter1.Set(ControlMode::Velocity, 10375);
    shooter2.Set(ControlMode::Velocity, 10375);
}

void shooter::shootSafeZone()
{
    motorOutput = 8200;
    shooter1.Set(ControlMode::Velocity, 8200);
    shooter2.Set(ControlMode::Velocity, 8200);
}

bool shooter::shooterReady()
{
    double threshold = motorOutput*.015;
    double difference = abs(shooter1.GetSelectedSensorVelocity() - motorOutput);
    if (difference < threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}