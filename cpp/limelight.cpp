#include "limelight.h"

std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");

limelight::limelight()
{
   tv = 0;
   table->PutNumber("ledMode", 1);
}

void limelight::updateLimelight()
{
    tv = table->GetNumber("tv",0.0);
    ty = table->GetNumber("ty",0.0);
    tx = table->GetNumber("tx",0.0);
}

void limelight::LEDMode(bool state)
{
    if (state)
    {
        table->PutNumber("ledMode", 3);
    }
    else
    {
        table->PutNumber("ledMode", 1);
    }
    
}

void limelight::setPipeline(int state)
{
    table->PutNumber("pipeline", state);
}

bool limelight::hasTarget()
{
    return tv;
}

double limelight::getHorizontalOffset()
{
    return tx;
}

double limelight::getVerticalOffset()
{
    return ty;
}