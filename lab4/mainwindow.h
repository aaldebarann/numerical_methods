#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QStyleFactory>
#include <QChartView>
#include <QLineSeries>
#include <stack>
#include "solver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *valueLabel;
    QChart *chart;
    QChartView *chartView;
    QLineEdit *n;
    QLineEdit *bca;
    QLineEdit *bcb;
    QLabel* summary;

    std::stack<QLineSeries*> seriesStack;
    std::stack<QLineSeries*> solutionStack;
    std::stack<QLineSeries*> errorStack;
    solver sl;

    int function_choice = 0;
    int task_choice = 0;
    int graph_choice = 0;

    void showGraph();
    void showErrorGraph();
    void showTrueGraph();
    void removeGraph();
    void removeTrueGraph();
    void removeErrorGraph();
    void showTable();
    void showSummary();
    void onRadioButtonClicked1();
    void onRadioButtonClicked2();
    void onRadioButtonClicked3();
};


#endif // MAINWINDOW_H
