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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *valueLabel;
    QChart *chart;
    QChartView *chartView; // график
    QLineEdit *n;   // число разбиений
    QLineEdit *bca; // граничное условие в a
    QLineEdit *bcb; // граничное условие в b
    QLineEdit *eps; // параметр выхода по точности
    QLabel* summary;

    std::stack<QLineSeries*> seriesStack;
    std::stack<QLineSeries*> solutionStack;
    std::stack<QLineSeries*> errorStack;

    // можно по сути создать стек с самими решениями, добавлять в него решения при новывх условиях и удалять при удалении с графика
    // чтобы не считать одно и то же при нажатии на кнопки решения, аналитического решения и ошибки

    int function_choice = 0; // выбор из трёх функций
    int task_choice = 0; // выбор из тестовой функции, обычной и периодической
    int graph_choice = 0; // выбор графика функции, производной или второй производной

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
