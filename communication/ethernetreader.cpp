#include "ethernetreader.h"

EthernetReader::EthernetReader(int receiverPort, int senderPort, QString ipAddress)
    :_ipAddress{ipAddress}, _receiverPort{receiverPort}, _senderPort{senderPort}
{

}

EthernetReader::EthernetReader(int port)
    :_port{port}
{

}

void EthernetReader::writeDataEthernet(QByteArray txDatagram, CommunicationType comType, int count, bool flush)
{
    // if(_ipAddress == "192.168.0.4" && txDatagram[0] != 'T')
    //     qDebug() << "Sent Data to" << _ipAddress << ":" << txDatagram;
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

void EthernetReader::closeEthernetConnection()
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
