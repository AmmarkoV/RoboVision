#include "motor_system.h"
#include "configuration.h"

int InitMotorSystem()
{
    LoadConfiguration(); // Set Correct values to VisCortx :P 2nd time but cannot make something better right now :P

    RobotInit(rd01_device,arduino_device);
    return 1;
}


int CloseMotorSystem()
{
    RobotClose();
    return 1;
}
