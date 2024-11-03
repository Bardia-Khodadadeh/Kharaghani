#include "udp_server.h"

UDP_Server::UDP_Server() {}


void UDP_Server::run()
{
    _weightHandler = new WeightHandler(8569, 8570, "192.168.0.3");

    exec();
}

void UDP_Server::setEncoders(int _en)
{

}
