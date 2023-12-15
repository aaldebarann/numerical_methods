#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QtDataVisualization>
#include <QStyleFactory>
#include <iostream>
#include <vector>
#include <cmath>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Q3DSurface *view;
    QLineEdit *xn;
    QLineEdit *yn;
    QLineEdit *eps;
    QLineEdit *maxsteps;
    QSlider *slider;
    QLabel *valueLabel;
    std::vector<QSurface3DSeries> iterations;
    QSurface3DSeries *dataSeries;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool sphereAdded{false};
    void showGraph();
    void showTable();
    void setT(int t);
};
#endif // MAINWINDOW_H
