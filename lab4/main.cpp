#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Лабораторная работа №4: Построерние интерполирующего кубического сплайна");
    QIcon icon = QIcon("icon.ico");
    w.setWindowIcon(icon);
    w.setGeometry(100, 100, 1, 1);
    w.show();
    return a.exec();
}
