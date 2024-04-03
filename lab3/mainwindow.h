#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QSlider>
#include <QStringList>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QtDataVisualization>
#include <QStyleFactory>
#include <iostream>
#include <vector>
#include <cmath>
#include "solver.h"

#define type_d high_precision_type

using high_precision_type = boost::multiprecision::cpp_dec_float_50;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Q3DSurface *view;
    QLineEdit *xn;
    QLineEdit *yn;
    QLineEdit *eps;
    QLineEdit *maxsteps;
    QLineEdit *w;
    QLineEdit *eps2;
    QLineEdit *maxsteps2;
    QLineEdit *w2;
    QLabel* summary;
    QSlider *slider;
    QLabel *valueLabel;
    QSurface3DSeries *iterations[10];
    QSurface3DSeries *iterations2[10];
    QSurface3DSeries *dataSeries = nullptr;
    QSurface3DSeries *dataPSeries = nullptr;
    QSurface3DSeries *dataTrueSeries = nullptr;
    solver slv;
    solver slv2;
    std::vector<std::vector<std::vector<type_d>>> v;
    std::vector<std::vector<std::vector<type_d>>> v2;
    std::vector<std::vector<std::vector<type_d>>> z;
    type_d a, b, c, d;
    int selectedTask = Functions::test;
    int selectedMeth = 0;
    MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

    bool activePlot{false};
    bool lastPlotButton{false};
    void showGraph();
    void showTrueGraph();
    void showPGraph();
    void removeGraph();
    void removeTrueGraph();
    void removePGraph();
    void showTable();
    void showSummary();
    void setT(int t);
    void onRadioButtonClicked();
};

#endif // MAINWINDOW_H
