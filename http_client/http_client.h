#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * @brief Struktur, die eine HTTP-Antwort darstellt
 */
struct Response
{
    /**
     * @brief Der Antwort-Header
     */
    QString header;
    /**
     * @brief Der Antwort inhalt
     */
    QString body;
};

/**
 * @brief Klasse, die den HTTP-Client zur Verfügung stellt
 */
class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    /**
     * @brief Sendet eine HTTP-Get-Anfrage an einen Server
     * @param address Adresse des Servers
     * @param port Port des Servers
     * @param filename Datei, die vom Server geschickt werden soll
     */
    void sendGetRequest(QString address, int port, QString filename);

signals:
    /**
     * @brief Signal (für die GUI); wird ausgelöst, wenn der Server eine Antwort geschickt hat
     * @param response Die Antwort vom Server
     */
    void receivedResponse(Response response);
    /**
     * @brief Signal (für die GUI); wird ausgelöst, wenn keine Antwort vom Server kam
     */
    void timeout();

private slots:
    /**
     * @brief Bearbeitet die Antwort vom Server
     */
    void readData();
    /**
     * @brief Der Timer, für die Überpüfung eines Timeouts, ist abgelaufen
     */
    void timeoutTimerFinished();

private:
    /**
     * @brief Der TcpSocket, über den die Kommunikation läuft
     */
    QTcpSocket *tcpSocket;
    /**
     * @brief Timer, zur Überpüfung eines Timeouts bei einer Anfrage
     */
    QTimer *timeoutTimer;
};

#endif // HTTPCLIENT_H
