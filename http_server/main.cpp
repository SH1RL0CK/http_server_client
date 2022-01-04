#include <QCoreApplication>
#include <http_server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Startet den Server auf Port 8080
    HttpServer *server = new HttpServer(a.applicationDirPath() + "/../../http_server/files/");
    server->start(8080);
    return a.exec();
}
