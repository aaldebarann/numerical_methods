#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QChart>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>
#include <iostream>
#include <cmath>
#include "solver.h"

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWidget *topLeftWidget;
    QWidget *topRightWidget;
    QWidget *bottomLeftWidget;
    QWidget *bottomRightWidget;
    QWidget *centralWidget;
    QLineEdit *grid;
    QTabWidget *tabWidget;
    QTableWidget *tableWidget;
    QTextEdit *summary;
    QChart *chart;
    QChart *pchart;
    QChartView *chartView;
    QChartView *pchartView;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *paxisX;
    QValueAxis *paxisY;

    int n = 10;
    double epsilon1 = 0;
    double xerr = 0;
    int tabCounter = 1;

    void onTask1Clicked();
    void onTask2Clicked();
    void onClearPlotButtonClicked();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
