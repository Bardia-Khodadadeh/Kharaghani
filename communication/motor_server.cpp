#include "motor_server.h"

Motor_Server::Motor_Server() {}

void Motor_Server::run()
{
    _b1 = new MotorHandler(8573, 8574, "192.168.0.5");
    _b2 = new MotorHandler(8571, 8572, "192.168.0.4");

    exec();
}



