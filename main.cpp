#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow(QString("window_num1"), nullptr);
    w->show();
    return QApplication::exec();
}
