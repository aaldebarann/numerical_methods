#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <iostream>
#include <vector>
#include <cmath>

struct test_result{
    int istep;
    double x;
    double v1;
    double v2;
    double divu;
    double ele;
    double stepsize;
    int divc;
    int mulc;
    double ve;
    double divve;
};

struct first_result{
    int istep;
    double x;
    double v1;
    double v2;
    double divu;
    double ele;
    double stepsize;
    int divc;
    int mulc;
};

struct second_result{
    int istep;
    double x;
    std::vector<double> uv;
    std::vector<double> uv1;
    std::vector<double> div;
    std::vector<double> ele;
    double stepsize;
    int divc;
    int mulc;
};

struct summary {
    int totalsteps = 0;
    double boundary = 0;
    double maxele = 0;
    double maxh = 0;
    double xmaxh = 0;
    double minh = 0;
    double xminh = 0;
    double maxdev = 0;
    double xmaxdev = 0;
};

std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator+(const std::vector<double>& v1, const double& r);
std::vector<double> operator*(const std::vector<double>& v, double scalar);
double f0(double p, double x, double u);
double f0sol(double x, double u_0);
double f1(double p, double x, double u);
std::vector<double> f2(double p, double x, std::vector<double> u);
double rk(double& p, double& x, double& u, double& h, double (*f)(double, double, double));
double rkh(double& p, double& x, double& u, double& h, double (*f)(double, double, double));
double rkv(double& p, double& x, double& u, double& h, double& ele, double& epsilon, int& C1, int& C2, double (*f)(double, double, double));
std::vector<double> rk(double& p, double& x, std::vector<double>& u, double& h, std::vector<double> (*f)(double, double, std::vector<double>));
std::vector<double> rkh(double& p, double& x, std::vector<double>& u, double& h, std::vector<double>(*f)(double, double, std::vector<double>));
std::vector<double> rkv(double& p, double& x, std::vector<double>& u, double& h, double& ele, double& epsilon, int& C1, int& C2, std::vector<double>(*f)(double, double, std::vector<double>));
std::vector<test_result> calculateTask1(double a, double b, double u, int Nmax, double epsilon, double h, bool conststep, summary& sum);
std::vector<first_result> calculateTask2(double a, double b, double u, int Nmax, double epsilon, double h, bool conststep, summary& sum);
std::vector<second_result> calculateTask3(double parameter, double a, double b, double u, double du, int Nmax, double epsilon, double h, bool conststep, summary& sum);


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QWidget *topLeftWidget;
    QWidget *topRightWidget;
    QWidget *bottomLeftWidget;
    QWidget *bottomRightWidget;
    QWidget *centralWidget;
    bool method = 1;
    int task = 0;
    int tabCounter = 1;

    QLineEdit *begin;
    QLineEdit *end;
    QLineEdit *step;
    QLineEdit *epsilon;
    QLineEdit *epsilonb;
    QLineEdit *max_step;
    QLineEdit *u_0;
    QLineEdit *du_0;
    QLineEdit *param;

    QTabWidget *tabWidget;
    QTableWidget *tableWidget;
    QTableWidget *sumWidget;
    QChart *chart;
    QChart *pchart;
    QChartView *chartView;
    QChartView *pchartView;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QMainWindow *chartWindow;

    summary sum;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onRadioButtonClicked();
    void onDrawButtonClicked();
    void onClearPlotButtonClicked();
    void UpdateSumTable();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
