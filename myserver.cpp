#include "myserver.h"

#include <QTime>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

MyServer::MyServer(int port, QWidget *parent)
    : QWidget(parent),
      nextBlockSize(0)
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(nullptr,
                              "Server Error",
                              "Unable to start the server: "
                              + server->errorString());
        return;
    }
    connect(server, &QTcpServer::newConnection,
            this, &MyServer::slotNewConnection);


    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("<H1>Server</H1>"));
    mainLayout->addWidget(textInfo);
    setLayout(mainLayout);
}

void MyServer::slotNewConnection()
{
   QTcpSocket* pClientSocket = server->nextPendingConnection();
   connect(pClientSocket, &QTcpSocket::disconnected,
           pClientSocket, &QObject::deleteLater);
   connect(pClientSocket, &QTcpSocket::readyRead,
           this, &MyServer::readClient);
}

void MyServer::readClient()
{
    QTcpSocket* pClientSocket = dynamic_cast<QTcpSocket*>(sender());
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_8);
    for(;;)
    {
        if(nextBlockSize == 0)
        {
            if(pClientSocket->bytesAvailable() < sizeof(quint16))
                break;

            in >> nextBlockSize;
        }

        if(pClientSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        QString message = time.toString() + " " + "Client has sent - " + str;
        textInfo->append(message);

        nextBlockSize = 0;

        sendToClient(pClientSocket,
                     "Server Response: Received \"" + str + "\"");
    }
}

void MyServer::sendToClient(QTcpSocket *socket, const QString &str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}
