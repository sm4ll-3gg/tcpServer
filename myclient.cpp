#include "myclient.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDataStream>
#include <QTime>

MyClient::MyClient(const QString &host, int port, QWidget *parent)
    : QWidget(parent),
      nextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    connect(socket, &QTcpSocket::connected,
            this, &MyClient::slotConnected);
    connect(socket, &QTcpSocket::readyRead,
            this, &MyClient::slotReadyRead);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));

    textInfo = new QTextEdit;
    textInfo->setReadOnly(true);
    textInput = new QLineEdit;

    QPushButton* sendButton = new QPushButton("&Send");
    connect(sendButton, &QPushButton::clicked,
            this, &MyClient::slotSendToServer);
    connect(textInput, &QLineEdit::returnPressed,
            this, &MyClient::slotSendToServer);

    // View
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("<H1>Client</H1>"));
    mainLayout->addWidget(textInfo);
    mainLayout->addWidget(textInput);
    mainLayout->addWidget(sendButton);
}

MyClient::~MyClient()
{

}

void MyClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_8);
    for(;;)
    {
        if(nextBlockSize == 0)
        {
            if(socket->bytesAvailable() < sizeof(quint16))
                break;

            in >> nextBlockSize;
        }

        if(socket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        textInfo->append(time.toString() + " " + str);
        nextBlockSize = 0;
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError("Error: ");

    QString errorMessage("");

    if(err == QAbstractSocket :: HostNotFoundError)
        errorMessage = "The host was not found.";
    else if(err == QAbstractSocket::RemoteHostClosedError)
        errorMessage = "The remote host is closed.";
    else if(err == QAbstractSocket::ConnectionRefusedError)
        errorMessage = "The connection was refused.";
    else errorMessage = socket->errorString();

    strError += errorMessage;

    textInfo->append(strError);
}

void MyClient::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << QTime::currentTime() << textInput->text();
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    socket->write(arrBlock);
    textInput->clear();
}

void MyClient::slotConnected()
{
    textInfo->append("Received the connected() signal");
}
