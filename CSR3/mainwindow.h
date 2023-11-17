#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QChart>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLineSeries>
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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWidget *topLeftWidget;
    QWidget *topRightWidget;
    QWidget *bottomLeftWidget;
    QWidget *bottomRightWidget;
    QWidget *centralWidget;
    QLineEdit *step;
    QLineEdit *x0;
    QLineEdit *xn;
    QLineEdit *a11;
    QLineEdit *a12;
    QLineEdit *a21;
    QLineEdit *a22;
    QLineEdit *u1;
    QLineEdit *u2;
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

    void ShowGraph();
    void ShowTable();
    void ClearGraph();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
