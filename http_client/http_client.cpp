#include "http_client.h"

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , tcpSocket(nullptr)
    , timeoutTimer(new QTimer)
{
    QObject::connect(timeoutTimer, &QTimer::timeout, this, &HttpClient::timeoutTimerFinished);
    timeoutTimer->setInterval(5000);
    timeoutTimer->setSingleShot(true);
}

void HttpClient::sendGetRequest(QString address, int port, QString filename)
{
    tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost(address, port);
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &HttpClient::readData);
    //Baut die Anfrage
    QString request = "GET /" + filename + " HTTP/1.1\r\n"
                      "HOST: " + address + ":" + QString::number(port) + "\r\n\r\n";
    tcpSocket->write(request.toLatin1());
    //Started den Timeout-Timer
    timeoutTimer->start();
}

void HttpClient::readData()
{
    if(!tcpSocket->bytesAvailable())
        return;
    QString responseStr = tcpSocket->readAll();
    tcpSocket->disconnectFromHost();
    Response response;
    //Teilt zwischen dem Antwort-Header und dem Antwort-Inhalt
    response.header = responseStr.section("\n\r\n", 0, 0);
    response.body = responseStr.section("\n\r\n", 1);
    //Stoppt den Timeout-Timer
    timeoutTimer->stop();
    tcpSocket->deleteLater();
    emit receivedResponse(response);
}

void HttpClient::timeoutTimerFinished()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->deleteLater();
    emit timeout();
}
