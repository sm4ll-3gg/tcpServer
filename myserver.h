#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>


class MyServer : public QWidget
{
    Q_OBJECT
public:
    explicit MyServer(int port, QWidget *parent = 0);

private slots:
    void        slotNewConnection();
    void        readClient();

private:
    void        sendToClient(QTcpSocket* socket, const QString& str);

private:
    QTextEdit*  textInfo;

    QTcpServer* server;
    quint16     nextBlockSize;
};

#endif // MYSERVER_H
