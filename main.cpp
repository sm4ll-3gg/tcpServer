#include "myserver.h"
#include "myclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer s(2323);
    s.show();
    MyClient c("localhost", 2323);
    c.show();

    return a.exec();
}
