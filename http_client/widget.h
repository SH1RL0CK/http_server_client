#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "http_client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
/**
 * @brief Klasse, für die GUI
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    /**
     * @brief Schickt die Anfrage auf Knopfdruck
     */
    void on_sendRequestPushButton_clicked();
    /**
     * @brief Der Server hat eine Antwort geschickt, diese wird in der GUI angezeigt
     */
    void receivedResponse(Response response);
    /**
     * @brief Es gab ein Timeout, der Benutzer wird darüber informiert
     */
    void timeout();

private:
    Ui::Widget *ui;
    /**
     * @brief Der HTTP-Client
     */
    HttpClient* httpClient;

    /**
     * @brief Zeigt dem Benutzer eine Nachricht
     * @param message Die Nachricht
     * @param color Die Farbe, in der sie angezeigt wird
     */
    void showOutput(QString message, QString color);
};
#endif // WIDGET_H
