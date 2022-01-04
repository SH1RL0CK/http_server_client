#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , httpClient(new HttpClient)
{
    ui->setupUi(this);
    QObject::connect(httpClient, &HttpClient::receivedResponse, this, &Widget::receivedResponse);
    QObject::connect(httpClient, &HttpClient::timeout, this, &Widget::timeout);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_sendRequestPushButton_clicked()
{

    httpClient->sendGetRequest(ui->adressLineEdit->text(), ui->portSpinBox->value(), ui->filenameLineEdit->text());
    ui->sendRequestPushButton->setDisabled(true);
    showOutput("Die Anfrage wird verschickt...", "orange");
}

void Widget::receivedResponse(Response response)
{
    ui->responseHeaderPlainTextEdit->setPlainText(response.header);
    ui->responseBodyPlainTextEdit->setPlainText(response.body);
    ui->sendRequestPushButton->setDisabled(false);
    showOutput("Der Server hat eine Antwot geliefert!", "green");
}

void Widget::timeout()
{
    ui->responseHeaderPlainTextEdit->setPlainText("");
    ui->responseBodyPlainTextEdit->setPlainText("");
    ui->sendRequestPushButton->setDisabled(false);
    showOutput("Der Server hat keine Antwot geliefert!", "red");
}

void Widget::showOutput(QString message, QString color)
{
    ui->outputLabel->setText(message);
    ui->outputLabel->setStyleSheet("color: " + color);
}



