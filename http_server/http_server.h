#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QMimeDatabase>
#include <iostream>

/**
 * @brief Klasse, die den HTTP-Server zur Verfügung stellt
 */
class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QString _workingDir, QObject *parent = nullptr);

    /**
     * @brief Startet den Server
     * @param port Der Port, auf dem der Server läuft
     */
    void start(int port);

private slots:
    /**
     * @brief Ein neuer Client, hat sich mit dem Server verbunden
     */
    void newConnection();
    /**
     * @brief Der Server verarbeitet die vom Client empfangenen Befehle
     */
    void readData();

private:
    /**
     * @brief Ordner, in dem die Dateien liegen
     */
    QString workingDir;
    /**
     * @brief Die MIME-Datenbank hilft, den MIME-Typ der Datei herauszufinden
     */
    QMimeDatabase mimeDatabase;
    /**
     * @brief Der TCP-Server
     */
    QTcpServer *tcpServer;
    /**
     * @brief Der TCP-Socket
     */
    QTcpSocket *tcpSocket;

    /**
     * @brief Sendet eine Antwort an den Client
     * @param Der Antwort-Code
     * @param Inhalt der Antwort
     * @param Der MIME-Typ des Inhalts
     */
    void sendResponse(QString responseCode, QString responseContent, QString contentType);
};

#endif // HTTPSERVER_H
