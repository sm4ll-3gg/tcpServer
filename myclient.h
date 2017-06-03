#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>

class MyClient : public QWidget
{
    Q_OBJECT

public:
    MyClient(const QString& host, int port, QWidget *parent = 0);
    ~MyClient();

private slots:
    void        slotReadyRead();
    void        slotError(QAbstractSocket::SocketError);
    void        slotSendToServer();
    void        slotConnected();

private:
    QTcpSocket* socket;
    quint16     nextBlockSize;

    QTextEdit*  textInfo;
    QLineEdit*  textInput;
};

#endif // MYCLIENT_H
