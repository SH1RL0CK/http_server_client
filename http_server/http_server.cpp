#include "http_server.h"

HttpServer::HttpServer(QString _workingDir, QObject *parent)
    : QObject(parent)
    , workingDir(_workingDir)
    , tcpServer(new QTcpServer)
{}

void HttpServer::start(int port)
{
    tcpServer->listen(QHostAddress::Any, port);
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &HttpServer::newConnection);
    std::cout << "Serving HTTP on 0.0.0.0 port " << port << "..." << std::endl;
}

void HttpServer::newConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &HttpServer::readData);
}

void HttpServer::readData()
{
    if(!tcpSocket->bytesAvailable())
        return;
    //liest die IP-Adresse des Clients aus
    QString logOuput = tcpSocket->peerAddress().toString().split(":").last();
    //Die IP-Adresse ist "localhost"
    if(logOuput == "1")
        logOuput = "127.0.0.1";
    logOuput += QDateTime::currentDateTime().toString(" - [dd/MMM/yyyy hh:mm:ss] - ");
    QString request = tcpSocket->readAll();
    QString responseCode = "", responseContent = "", contentType = "text/plain";

    //Es wurde eine GET-Anfrage geschickt
    if(request.startsWith("GET /"))
    {
        QString filename = request.split(" ").at(1);
        logOuput += "\"GET " + filename + "\" - ";
        if(QFileInfo(workingDir + filename).isDir())
            filename += "/index.html";
        if(QFileInfo::exists(workingDir + filename))
        {
            //Liest den Inhalt der angeforderten Datei
            QFile file(workingDir + filename);
            file.open(QFile::ReadOnly | QFile::Text);
            responseContent = file.readAll();
            file.close();
            //Bestimmt den MIME-Typ derangeforderten Datei
            contentType = mimeDatabase.mimeTypeForFile(workingDir + filename).name();
            responseCode = "200 Ok";
        }
        //Die angeforderte Datei, exestiert nicht
        else
        {
            responseCode = "404 Not Found";
            responseContent = "404 Not found";
        }
    }
    else
    {
        // Falsche Anfrage
        responseCode = "505 Bad Gateway";
        responseContent = "505 Bad Gateway";
    }
    sendResponse(responseCode, responseContent, contentType);
    logOuput += responseCode;
    //Gibt einen Output in der Konsole
    std::cout << logOuput.toStdString() << std::endl;
    tcpSocket->close();
}

void HttpServer::sendResponse(QString responseCode, QString responseContent, QString contentType)
{
    //Baut den Antwort-Header
    QString responseHeader = "HTTP/1.1 " + responseCode + "\n"
                             "Date: " + QDateTime::currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT\n"
                             "Server: Philipps HTTP-Server\n"
                             "Content-Length: " + QString::number(responseContent.toUtf8().size()) + "\n"
                             "Content-Language: en\n"
                             "Connection: close\n"
                             "Content-Type: " + contentType + "\n\r\n";
    tcpSocket->write(QString(responseHeader + responseContent).toLatin1());
}

