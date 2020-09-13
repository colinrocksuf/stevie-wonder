#include "ControlPanelMatch.h"
#include <frc/livewindow/LiveWindow.h>
double speed = 0.1;
int Change = 0;
int Spun = 0;
static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
int x;
bool ba = false;
bool bb = false;
string savedColor;
rev::ColorSensorV3 m_colorSensor{i2cPort};
rev::ColorMatch m_colorMatcher;

string cols[] = {"R", "B", "Y", "G"};

static constexpr frc::Color kBlueTarget = frc::Color(0.143, 0.427, 0.429);
static constexpr frc::Color kGreenTarget = frc::Color(0.197, 0.561, 0.240);
static constexpr frc::Color kRedTarget = frc::Color(0.561, 0.232, 0.114);
static constexpr frc::Color kYellowTarget = frc::Color(0.361, 0.524, 0.113);

ControlPanelMatch::ControlPanelMatch()
{
    
    snowBlower = new TalonSRX(4);
    m_colorMatcher.AddColorMatch(kBlueTarget);
    m_colorMatcher.AddColorMatch(kRedTarget);
    m_colorMatcher.AddColorMatch(kGreenTarget);
    m_colorMatcher.AddColorMatch(kYellowTarget);
    m_colorSensor.ConfigureColorSensor(rev::ColorSensorV3::ColorResolution::k16bit, rev::ColorSensorV3::ColorMeasurementRate::k25ms);
    SmartDashboard::PutNumber("confidencescore", 5);
    SmartDashboard::PutNumber("speed", 0.2);
}


void ControlPanelMatch::RecColor()
{
    frc::Color detectedColor = m_colorSensor.GetColor();
    double confidence = SmartDashboard::GetNumber("confidencescore", 0.6);
    speed = SmartDashboard::GetNumber("speed", 0.6);
    frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, confidence);

    if(matchedColor == kBlueTarget)
    {
        x = 2;
    }
    else if(matchedColor == kRedTarget)
    {
        x = 0;
    }
    if(matchedColor == kGreenTarget)
    {
        x = 3;
    }
    if(matchedColor == kYellowTarget)
    {
        x = 1;
    }
    SmartDashboard::PutNumber("Red", detectedColor.red);
    SmartDashboard::PutNumber("Green", detectedColor.green);
    SmartDashboard::PutNumber("Blue", detectedColor.blue);
    SmartDashboard::PutString("Color", cols[x]);
    if(savedColor != cols[x])
    {
        Change++;
        savedColor = x;
    }
    frc::SmartDashboard::PutNumber("Changes", Change);
    frc::SmartDashboard::PutNumber("Spun", Spun);
    
}

bool ControlPanelMatch::SpinThree()
{
    RecColor();
    if(Change < 24 && Spun == 0)
    {
        snowBlower->Set(ControlMode::PercentOutput, speed);
        return true;
    }
    else
    {
        snowBlower->Set(ControlMode::PercentOutput, 0);
        Spun = 1;
        Change = 0;
        return false;
    }
}

bool ControlPanelMatch::SpinToColor()
{
    string driverStationColor = DriverStation::GetInstance().GetGameSpecificMessage();
    RecColor();
    if(Change != 1)
    {
        snowBlower->Set(ControlMode::PercentOutput, 0.15);
        return true;
    }
    else
    {
        snowBlower->Set(ControlMode::PercentOutput, 0);
        Spun = 2;
        return false;
    }
    
}

void ControlPanelMatch::StopAll()
{
    snowBlower->Set(ControlMode::PercentOutput, 0);
    Change = 0;
    Spun = 0;
}

void ControlPanelMatch::Update(int i)
{
    if(i == 0)
    {
        ba = true;
    }
    else if(i == 90)
    {
        bb = true;
    }
    else if(i == 270)
    {
        ba = false;
        bb = false;
    }
    if(ba == true)
    {
        ba = SpinThree();
    }
    else if(bb == true)
    {
        bb = SpinToColor();
    }
}