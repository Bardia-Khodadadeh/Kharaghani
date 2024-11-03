#include "abstractcontroller.h"

AbstractController::AbstractController(int receiverPort, int senderPort, QString ipAddress)
    :_ipAddress{ipAddress}, _receiverPort{receiverPort}, _senderPort{senderPort}
{
}

AbstractController::AbstractController(int port)
    :_port{port}
{
}

void AbstractController::writeDataEthernet(QByteArray txDatagram, CommunicationType comType, int count, bool flush)
{
    if(comType == UDP)
    {
        for(int i=0;i<count;i++)
        {
            QHostAddress ard_Address(_ipAddress);
            _udpSocketSend->writeDatagram(txDatagram.data(), txDatagram.size(), ard_Address, _senderPort);
            _udpSocketSend->flush();
        }
    }
    else if(comType == TCP)
    {
        _clientSocket->write(txDatagram);
        _clientSocket->flush();
    }
}

void AbstractController::closeEthernetConnection()
{
    if(communicationType == UDP)
    {
        _udpSocketSend->close();
        _udpSocketReceive->close();

    }
    else if(communicationType == TCP)
    {
        _server->close();
        _clientSocket->close();
    }
}
