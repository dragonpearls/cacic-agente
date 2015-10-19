#ifndef UICLIENT_H
#define UICLIENT_H

#include <QAbstractSocket>
#include <QString>
#include <QTcpSocket>
#include <QWidget>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class UiClient: public QWidget
{
    Q_OBJECT
public:
    UiClient(const QString &dirpath,QWidget *parent =0);
    bool isConnected();

signals:
    void finalizar();
public slots:
    void on_forcarClicked();
    void on_finalizarClicked();

private slots:
    void on_bytesWritten(qint64 bytes);
    void on_connected();
    void on_disconnected();
    void on_readyRead();

private:
    QByteArray formatData(QString message, int messageLength);
    void parseData(const QString &dataReceived);
    void setupSocketConnection();

    bool connected;
    bool canSend;
    LogCacic *logcacic;
    QByteArray lastDataWritten;
    QString mainFolder;
    QTcpSocket *socket;
};

#endif // UICLIENT_H