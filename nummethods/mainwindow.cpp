#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QHeaderView>

#include "mainwindow.h"
#include "ui_mainwindow.h"

std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size()) {
        throw std::runtime_error("Vector sizes do not match for addition.");
    }

    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

std::vector<double> operator+(const std::vector<double>& v1, const double& r) {
    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] + r;
    }
    return result;
}

std::vector<double> operator*(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        result[i] = v[i] * scalar;
    }
    return result;
}

double f0(double p, double x, double u) {
    return -u / 2.0;
}

double f0sol(double x, double u_0) {
    return exp(-x / 2 + log(u_0));
}

double f1(double p, double x, double u) {
    return u * u / (1.0 + pow(x, 4))  + u - u * u * u * sin(10 * x);
}

std::vector<double> f2(double p, double x, std::vector<double> u) {
    std::vector<double> result(2);
    result[0] = - p * sin(u[1]);
    result[1] = u[0];
    return result;
}

double rk(double& p, double& x, double& u, double& h, double (*f)(double, double, double)) {
    double k1, k2, k3, k4;

    k1 = f(p, x, u);
    k2 = f(p, x + h / 2.0, u + h * k1 / 2.0);
    k3 = f(p, x + h / 2.0, u + h * k2 / 2.0);
    k4 = f(p, x + h, u + h * k3);

    return u + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}

double rkh(double& p, double& x, double& u, double& h, double (*f)(double, double, double)) {
    double hh = h / 2.0;
    double u1 = rk(p, x, u, hh, f);
    double x1 = x + hh;
    return rk(p, x1, u1, hh, f);
}

double rkv(double& p, double& x, double& u, double& h, double& ele, double& epsilon, int& C1, int& C2, double (*f)(double, double, double)) {
    double u1 = rk(p, x, u, h, f);
    double u2 = rkh(p, x, u, h, f);
    ele = abs(u2 - u1) / 15;
    if (ele > epsilon) {
        h /= 2;
        C1++;
        return rkv(p, x, u, h, ele, epsilon, C1, C2, f);
    }
    else if (ele < epsilon / 32) {
        h *= 2;
        C2++;
        ele *= 16;
        return u1;
    }
    else {
        ele *= 16;
        return u1;
    }
}

std::vector<double> rk(double& p, double& x, std::vector<double>& u, double& h, std::vector<double> (*f)(double, double, std::vector<double>)) {
    std::vector<double> k1, k2, k3, k4;
    k1 = f(p, x, u);
    k2 = f(p, x + h / 2, u + k1 * (h / 2.0));
    k3 = f(p, x + h / 2, u + k2 * (h / 2.0));
    k4 = f(p, x + h, u + k3 * h);

    return u + (k1 + k2 * 2 + k3 * 2 + k4) * (h / 6.0);
}


std::vector<double> rkh(double& p, double& x, std::vector<double>& u, double& h, std::vector<double>(*f)(double, double, std::vector<double>)) {
    double hh = h / 2;
    std::vector<double> u1 = rk(p, x, u, hh, f);
    double x1 = x + hh;
    return rk(p, x1, u1, hh, f);
}

std::vector<double> rkv(double& p, double& x, std::vector<double>& u, double& h, std::vector<double>& ele, double& epsilon, int& C1, int& C2, std::vector<double>(*f)(double, double, std::vector<double>)) {
    std::vector<double> u1 = rk(p, x, u, h, f);
    std::vector<double> u2 = rkh(p, x, u, h, f);
    double dele = 0;
    for (int i = 0; i < u1.size(); i++) {
        ele[i] = abs(u2[i] - u1[i]) / 15;
        if (dele < ele[i]) dele = ele[i];
    }
    if (dele > epsilon) {
        h /= 2;
        C1++;
        return rkv(p, x, u, h, ele, epsilon, C1, C2, f);
    }
    else if (dele < epsilon / 32) {
        h *= 2;
        C2++;
        ele = ele * 16;
        return u1;
    }
    else {
        ele = ele * 16;
        return u1;
    }
}

std::vector<test_result> calculateTask1(double a, double b, double u, int Nmax, double epsilon, double epsilonb, double h, bool conststep, summary& sum) {
    std::vector<test_result> result;
    test_result temp;
    temp.x = a;
    temp.stepsize = h;
    double u_0 = u;
    double parameter = 0;
    temp.v1 = u;
    temp.v2 = u;
    temp.ve = u;
    temp.ele = 0;
    temp.divc = 0;
    temp.mulc = 0;
    temp.istep = 0;
    if (conststep == 1) {
        for (;temp.istep < Nmax; temp.istep++) {
            temp.v1 = rk(parameter, temp.x, u, h, f0);
            temp.v2 = rkh(parameter, temp.x, u, h, f0);
            temp.ve = f0sol(temp.x + h, u_0);

            temp.divu = temp.v1 - temp.v2;
            temp.divve = temp.ve - temp.v1;
            temp.ele = 16 * abs(temp.divu) / 15;
            if (sum.maxele < temp.ele) { sum.maxele = temp.ele; }
            if (sum.maxdev < abs(temp.divve)) { sum.maxdev = abs(temp.divve); sum.xmaxdev = temp.x; }
            if (temp.x + h > b  - epsilonb) {
                temp.stepsize = b - temp.x - epsilonb / 2;
                temp.v1 = rk(parameter, temp.x, u, temp.stepsize, f0);
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f0);
                temp.ve = f0sol(temp.x + temp.stepsize, u_0);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            }
            temp.x += h;
            u = temp.v1;

            result.push_back(temp);
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    else {
        for (;temp.istep < Nmax; temp.istep++) {
            int c1 = temp.divc;
            int c2 = temp.mulc;
            temp.v1 = rkv(parameter, temp.x, u, h, temp.ele, epsilon, temp.divc, temp.mulc, f0);

            if (sum.maxele < temp.ele) { sum.maxele = temp.ele; }
            if (sum.maxh < h) { sum.maxh = h; sum.xmaxh = temp.x; }
            if (sum.minh > h) { sum.minh = h; sum.xminh = temp.x; }
            if (temp.x + h > b - epsilonb) {
                temp.stepsize = b - temp.x - epsilonb / 2;
                temp.v1 = rk(parameter, temp.x, u, temp.stepsize, f0);
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f0);
                temp.ve = f0sol(temp.x + temp.stepsize, u_0);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            } else if (c1 == (temp.divc - 1) && c2 == (temp.mulc - 1)) {
                temp.stepsize = h / 2;
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f0);
                temp.ve = f0sol(temp.x + temp.stepsize, u_0);
                temp.divu = temp.v1 - temp.v2;
                temp.divve = temp.ve - temp.v1;
                temp.x += temp.stepsize;
            } else if (temp.stepsize < h) {
                temp.ve = f0sol(temp.x + temp.stepsize, u_0);
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f0);
                temp.divu = temp.v1 - temp.v2;
                temp.divve = temp.ve - temp.v1;
                temp.x += temp.stepsize;
            } else  {
                temp.ve = f0sol(temp.x + h, u_0);
                temp.v2 = rkh(parameter, temp.x, u, h, f0);
                temp.divu = temp.v1 - temp.v2;
                temp.divve = temp.ve - temp.v1;
                temp.x += h;
                temp.stepsize = h;
            }
            if (sum.maxdev < abs(temp.divve)) { sum.maxdev = abs(temp.divve); sum.xmaxdev = temp.x; }
            u = temp.v1;

            result.push_back(temp);
            temp.stepsize = h;
            if (h < 0.0000001) break;
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    return result;
}

std::vector<first_result> calculateTask2(double a, double b, double u, int Nmax, double epsilon, double epsilonb, double h, bool conststep, summary& sum) {
    std::vector<first_result> result;
    first_result temp;
    temp.x = a;
    temp.stepsize = h;
    double parameter = 0;
    temp.v1 = u;
    temp.v2 = u;
    temp.ele = 0;
    temp.divc = 0;
    temp.mulc = 0;
    temp.istep = 0;
    if (conststep == 1) {
        for (; temp.istep < Nmax; temp.istep++) {
            temp.v1 = rk(parameter, temp.x, u, h, f1);
            temp.v2 = rkh(parameter, temp.x, u, h, f1);
            temp.divu = temp.v1 - temp.v2;
            temp.ele = 16 * abs(temp.divu) / 15;
            if (sum.maxele < temp.ele) { sum.maxele = temp.ele; }
            if (temp.x + h > b  - epsilonb) {
                temp.stepsize = b - temp.x - epsilonb / 2;
                temp.v1 = rk(parameter, temp.x, u, temp.stepsize, f1);
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f1);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            }
            temp.x += h;
            u = temp.v1;

            result.push_back(temp);
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    else {
        for (;temp.istep < Nmax; temp.istep++) {
            int c1 = temp.divc;
            int c2 = temp.mulc;
            temp.v1 = rkv(parameter, temp.x, u, h, temp.ele, epsilon, temp.divc, temp.mulc, f1);

            if (sum.maxele < temp.ele) { sum.maxele = temp.ele; }
            if (sum.maxh < h) { sum.maxh = h; sum.xmaxh = temp.x; }
            if (sum.minh > h) { sum.minh = h; sum.xminh = temp.x; }
            if (temp.x + h > b - epsilonb) {
                temp.stepsize  = b - temp.x - epsilonb / 2;
                temp.v1 = rk(parameter, temp.x, u, temp.stepsize, f1);
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f1);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            } else if (c1 == (temp.divc - 1) && c2 == (temp.mulc - 1)) {
                temp.stepsize = h / 2;
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f1);
                temp.divu = temp.v1 - temp.v2;
                temp.x += temp.stepsize;
            } else if (temp.stepsize < h) {
                temp.v2 = rkh(parameter, temp.x, u, temp.stepsize, f1);
                temp.divu = temp.v1 - temp.v2;
                temp.x += temp.stepsize;
            } else  {
                temp.v2 = rkh(parameter, temp.x, u, h, f1);
                temp.divu = temp.v1 - temp.v2;
                temp.x += h;
                temp.stepsize = h;
            }
            u = temp.v1;

            result.push_back(temp);
            temp.stepsize = h;
            if (h < 0.0000001) break;
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    return result;
}

std::vector<second_result> calculateTask3(double parameter, double a, double b, double u, double du, int Nmax, double epsilon, double epsilonb, double h, bool conststep, summary& sum) {
    std::vector<second_result> result;
    second_result temp;
    temp.x = a;
    temp.stepsize = h;
    temp.ele = {0, 0};
    temp.divc = 0;
    temp.mulc = 0;
    temp.istep = 0;
    std::vector<double> uv = { u, du };
    temp.uv = { u, du };
    if (conststep == 1) {
        for (;temp.istep < Nmax; temp.istep++) {
            temp.uv = rk(parameter, temp.x, uv, h, f2);
            temp.uv1 = rkh(parameter, temp.x, uv, h, f2);
            temp.div = temp.uv + temp.uv1 * (-1);
            for (int i = 0; i < temp.uv.size(); i++) {
                temp.ele[i] = 16 * abs(temp.uv[i] - temp.uv[i]) / 15;
                if (sum.maxele < temp.ele[i]) { sum.maxele = temp.ele[i]; }
                if (sum.maxele < temp.ele[i]) { sum.maxele = temp.ele[i]; }
            }
            if (temp.x + h > b - epsilonb) {
                temp.stepsize = b - temp.x - epsilonb / 2;
                temp.uv = rk(parameter, temp.x, uv, temp.stepsize, f2);
                temp.uv1 = rkh(parameter, temp.x, uv, temp.stepsize, f2);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            }
            temp.x += h;
            uv = temp.uv;

            result.push_back(temp);
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    else {
        for (;temp.istep < Nmax; temp.istep++) {
            int c1 = temp.divc;
            int c2 = temp.mulc;
            temp.uv = rkv(parameter, temp.x, uv, h, temp.ele, epsilon, temp.divc,  temp.mulc,  f2);
            for (int i = 0; i < temp.uv.size(); i++) {
                if (sum.maxele < temp.ele[i]) { sum.maxele = temp.ele[i]; }
            }
            if (sum.maxh < h) { sum.maxh = h; sum.xmaxh = temp.x; }
            if (sum.minh > h) { sum.minh = h; sum.xminh = temp.x; }
            if (temp.x + h > b - epsilonb) {
                temp.stepsize = b - temp.x - epsilonb / 2;
                temp.uv = rk(parameter, temp.x, uv, temp.stepsize, f2);
                temp.uv1 = rkh(parameter, temp.x, uv, temp.stepsize, f2);
                temp.x += temp.stepsize;
                result.push_back(temp);
                break;
            } else if (c1 == (temp.divc - 1) && c2 == (temp.mulc - 1)) {
                temp.stepsize = h / 2;
                temp.uv1 = rkh(parameter, temp.x, uv, temp.stepsize, f2);
                temp.div = temp.uv + temp.uv1 * (-1);
                temp.x += temp.stepsize;
            } else if (temp.stepsize < h) {
                temp.uv1 = rkh(parameter, temp.x, uv, temp.stepsize, f2);
                temp.div = temp.uv + temp.uv1 * (-1);
                temp.x += temp.stepsize;
            } else  {
                temp.uv1 = rkh(parameter, temp.x, uv, h, f2);
                temp.div = temp.uv + temp.uv1 * (-1);
                temp.x += h;
                temp.stepsize = h;
            }
            uv = temp.uv;

            result.push_back(temp);

            if (h < 0.0000001) break;
            temp.stepsize = h;
        }
        sum.boundary = temp.x;
        sum.totalsteps = temp.istep;
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(1600, 1000);
    tabCounter = 1;

    topLeftWidget = new QWidget(this);
    topRightWidget = new QWidget(this);
    bottomLeftWidget = new QWidget(this);
    bottomRightWidget = new QWidget(this);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(topLeftWidget, 0, 0);
    gridLayout->addWidget(topRightWidget, 0, 1);
    gridLayout->addWidget(bottomLeftWidget, 1, 0);
    gridLayout->addWidget(bottomRightWidget, 1, 1);
    topLeftWidget->setFixedSize(600, 320);
    bottomLeftWidget->setFixedSize(750, 680);
    centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    QLabel *label1 = new QLabel("Начало интервала:", topLeftWidget);
    QLabel *label2 = new QLabel("Конец интервала:", topLeftWidget);
    QLabel *label3 = new QLabel("Шаг интегрирования:", topLeftWidget);
    QLabel *label4 = new QLabel("Точность интегрирования:", topLeftWidget);
    QLabel *label9 = new QLabel("Эпсилон граничное:", topLeftWidget);
    QLabel *label5 = new QLabel("Максимальное количество шагов:", topLeftWidget);
    QLabel *label6 = new QLabel("Начальное условие для u:", topLeftWidget);
    QLabel *label7 = new QLabel("Начальное условие для u':", topLeftWidget);
    QLabel *label8 = new QLabel("Значение параметра:", topLeftWidget);

    begin = new QLineEdit(topLeftWidget);
    begin->setText("0.0");
    begin->setFixedWidth(150);
    end = new QLineEdit(topLeftWidget);
    end->setText("10.0");
    end->setFixedWidth(150);
    step = new QLineEdit(topLeftWidget);
    step->setText("0.01");
    step->setFixedWidth(150);
    epsilon = new QLineEdit(topLeftWidget);
    epsilon->setText("0.0001");
    epsilon->setFixedWidth(150);
    epsilonb = new QLineEdit(topLeftWidget);
    epsilonb->setText("0.001");
    epsilonb->setFixedWidth(150);
    max_step = new QLineEdit(topLeftWidget);
    max_step->setText("10000");
    max_step->setFixedWidth(150);
    u_0 = new QLineEdit(topLeftWidget);
    u_0->setText("1");
    u_0->setFixedWidth(150);
    du_0 = new QLineEdit(topLeftWidget);
    du_0->setText("0");
    du_0->setFixedWidth(150);
    param = new QLineEdit(topLeftWidget);
    param->setText("1");
    param->setFixedWidth(150);

    QRadioButton *taskButton1 = new QRadioButton("Тестовая задача", topLeftWidget);
    QRadioButton *taskButton2 = new QRadioButton("Первая задача", topLeftWidget);
    QRadioButton *taskButton3 = new QRadioButton("Вторая задача", topLeftWidget);
    QRadioButton *stepButton1 = new QRadioButton("С постоянным шагом", topLeftWidget);
    QRadioButton *stepButton2 = new QRadioButton("С переменным шагом", topLeftWidget);

    QPushButton *drawButton = new QPushButton("Отрисовать", this);
    drawButton->setFixedWidth(150);
    QPushButton *clearPlotButton = new QPushButton("Очистить график", this);
    clearPlotButton->setFixedWidth(150);

    QGridLayout *topLeftLayout = new QGridLayout;
    QButtonGroup *taskbuttonGroup = new QButtonGroup(topLeftWidget);
    taskbuttonGroup->addButton(taskButton1);
    taskbuttonGroup->addButton(taskButton2);
    taskbuttonGroup->addButton(taskButton3);
    taskButton1->setChecked(true);

    QButtonGroup *stepbuttonGroup = new QButtonGroup(topLeftWidget);
    stepbuttonGroup->addButton(stepButton1);
    stepbuttonGroup->addButton(stepButton2);
    stepButton1->setChecked(true);

    topLeftLayout->addWidget(taskButton1, 0, 0);
    topLeftLayout->addWidget(taskButton2, 0, 1);
    topLeftLayout->addWidget(taskButton3, 0, 2);
    topLeftLayout->addWidget(stepButton1, 1, 0);
    topLeftLayout->addWidget(stepButton2, 1, 1);
    topLeftLayout->addWidget(label1, 2, 0);
    topLeftLayout->addWidget(begin, 2, 1);
    topLeftLayout->addWidget(label2, 3, 0);
    topLeftLayout->addWidget(end, 3, 1);
    topLeftLayout->addWidget(label3, 4, 0);
    topLeftLayout->addWidget(step, 4, 1);
    topLeftLayout->addWidget(label4, 5, 0);
    topLeftLayout->addWidget(epsilon, 5, 1);
    topLeftLayout->addWidget(label9, 6, 0);
    topLeftLayout->addWidget(epsilonb, 6, 1);
    topLeftLayout->addWidget(label5, 7, 0);
    topLeftLayout->addWidget(max_step, 7, 1);
    topLeftLayout->addWidget(label6, 8, 0);
    topLeftLayout->addWidget(u_0, 8, 1);
    topLeftLayout->addWidget(label7, 9, 0);
    topLeftLayout->addWidget(du_0, 9, 1);
    topLeftLayout->addWidget(label8, 10, 0);
    topLeftLayout->addWidget(param, 10, 1);
    topLeftLayout->addWidget(drawButton, 11, 0);
    topLeftLayout->addWidget(clearPlotButton, 11, 1);

    topLeftWidget->setLayout(topLeftLayout);

    tabWidget = new QTabWidget(bottomLeftWidget);
    QVBoxLayout *bottomLeftLayout = new QVBoxLayout(bottomLeftWidget);
    bottomLeftLayout->addWidget(tabWidget);
    bottomLeftWidget->setLayout(bottomLeftLayout);

    sumWidget = new QTableWidget(topRightWidget);
    QHeaderView *sheader = sumWidget->horizontalHeader();
    sheader->setSectionResizeMode(QHeaderView::ResizeToContents);
    QVBoxLayout *topRightLayout = new QVBoxLayout(topRightWidget);
    sumWidget->setColumnCount(9);
    sumWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Всего шагов"));
    sumWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Правая граница"));
    sumWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Макс. ОЛП"));
    sumWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Наиб. шаг"));
    sumWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("x наиб."));
    sumWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Наим. шаг"));
    sumWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("x наим. "));
    sumWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("max |u-v|"));
    sumWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("x |u-v|"));

    topRightLayout->addWidget(sumWidget);


    chart = new QChart();
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    axisX = new QValueAxis;
    axisX->setTitleText("X");
    chart->addAxis(axisX, Qt::AlignBottom);
    axisX->setGridLineVisible(true);

    axisY = new QValueAxis;
    axisY->setTitleText("U");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setGridLineVisible(true);

    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setInteractive(true);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    chartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    QVBoxLayout *bottomRightLayout = new QVBoxLayout(bottomRightWidget);
    bottomRightLayout->addWidget(chartView);
    bottomRightWidget->setLayout(bottomRightLayout);

    connect(taskButton1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(taskButton2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(taskButton3, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(stepButton1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(stepButton2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(drawButton, &QPushButton::clicked, this, &MainWindow::onDrawButtonClicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::onClearPlotButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRadioButtonClicked()
{
    QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "Тестовая задача") {
        task = 0;
    } else if (selectedButton->text() == "Первая задача") {
        task = 1;
    } else if (selectedButton->text() == "Вторая задача") {
        task = 2;
    }
    if (selectedButton->text() == "С постоянным шагом") {
        method = 1;
    } else if (selectedButton->text() == "С переменным шагом") {
        method = 0;
    }
}

void MainWindow::onDrawButtonClicked()
{
    double beginValue = begin->text().toDouble();
    double endValue = end->text().toDouble();
    double u_0Value = u_0->text().toDouble();
    double du_0Value = du_0->text().toDouble();
    double max_stepValue = max_step->text().toInt();
    double epsilonValue = epsilon->text().toDouble();
    double epsilonbValue = epsilonb->text().toDouble();
    double stepValue = step->text().toDouble();
    double paramValue = param->text().toDouble();
    QSplineSeries *series = new QSplineSeries();
    if (task == 0) {
        auto curve = calculateTask1(beginValue, endValue, u_0Value, max_stepValue, epsilonValue, epsilonbValue, stepValue, method, sum);
        QSplineSeries *seriesve = new QSplineSeries();
        tableWidget = new QTableWidget(tabWidget);
        QHeaderView *header = tableWidget->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableWidget->setSortingEnabled(false);
        tableWidget->setColumnCount(10);
        tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
        tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
        tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
        tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));
        tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("ОЛП"));
        tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Шаг"));
        tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Делений"));
        tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Умножений"));
        tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("u"));
        tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("|u-v|"));
        for (const test_result &point : curve) {
            if (std::isnan(point.x) || std::isnan(point.v1) || std::isnan(point.ve)) {
                continue;
            }
            series->append(point.x, point.v1);
            seriesve->append(point.x, point.ve);
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(point.x)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(point.v1)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(point.v2)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(point.divu)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(point.ele)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(point.stepsize)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(point.divc)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(point.mulc)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 8, new QTableWidgetItem(QString::number(point.ve)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 9, new QTableWidgetItem(QString::number(point.divve)));
        }
        series->setName("Тестовая" + QString::number(tabCounter));
        seriesve->setName("Тестовая истинная" + QString::number(tabCounter));
        chart->addSeries(seriesve);
        tabWidget->addTab(tableWidget, "Тестовая задача " + QString::number(tabCounter));
        tabCounter++;
    }
    else if (task == 1) {
        auto curve = calculateTask2(beginValue, endValue, u_0Value, max_stepValue, epsilonValue, epsilonbValue, stepValue, method, sum);
        tableWidget = new QTableWidget(tabWidget);
        QHeaderView *header = tableWidget->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableWidget->setSortingEnabled(false);
        tableWidget->setColumnCount(8);
        tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
        tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
        tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
        tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));
        tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("ОЛП"));
        tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Шаг"));
        tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Делений"));
        tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Умножений"));
        for (const first_result &point : curve) {
            if (std::isnan(point.x) || std::isnan(point.v1)) {
                continue;
            }
            series->append(point.x, point.v1);
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(point.x)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(point.v1)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(point.v2)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(point.divu)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(point.ele)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(point.stepsize)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(point.divc)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(point.mulc)));
        }
        series->setName("Первая " + QString::number(tabCounter));
        tabWidget->addTab(tableWidget, "Первая задача " + QString::number(tabCounter));
        tabCounter++;
    }
    else if (task == 2) {
        auto curve = calculateTask3(paramValue, beginValue, endValue, u_0Value, du_0Value, max_stepValue, epsilonValue, epsilonbValue, stepValue, method, sum);
        QSplineSeries *seriesdu = new QSplineSeries();
        tableWidget = new QTableWidget(tabWidget);
        QHeaderView *header = tableWidget->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableWidget->setSortingEnabled(false);
        tableWidget->setColumnCount(8);
        tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
        tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("dv"));
        tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("dv2"));
        tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("dv-dv2"));
        tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("ОЛП"));
        tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Шаг"));
        tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Делений"));
        tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Умножений"));
        for (const second_result &point : curve) {
            if (std::isnan(point.x) || std::isnan(point.uv[0])) {
                continue;
            }
            seriesdu->append(point.x, point.uv[0]);
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(point.x)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(point.uv[0])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(point.uv1[0])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(point.div[0])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(point.ele[0])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(point.stepsize)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(point.divc)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(point.mulc)));
        }
        seriesdu->setName("Вторая, производная " + QString::number(tabCounter));
        chart->addSeries(seriesdu);
        tabWidget->addTab(tableWidget, "Вторая задача производная " + QString::number(tabCounter));
        tableWidget = new QTableWidget(tabWidget);
        header = tableWidget->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableWidget->setSortingEnabled(false);
        tableWidget->setColumnCount(8);
        tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
        tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
        tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
        tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));
        tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("ОЛП"));
        tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Шаг"));
        tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Делений"));
        tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Умножений"));
        for (const second_result &point : curve) {
            if (std::isnan(point.x) || std::isnan(point.uv[1])) {
                continue;
            }
            series->append(point.x, point.uv[1]);
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(point.x)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(point.uv[1])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(point.uv1[1])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(point.div[1])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(point.ele[1])));
            tableWidget->setItem(tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(point.stepsize)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(point.divc)));
            tableWidget->setItem(tableWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(point.mulc)));
        }
        series->setName("Вторая " + QString::number(tabCounter));
        tabWidget->addTab(tableWidget, "Вторая задача " + QString::number(tabCounter));
        if(!chartWindow){
            chartWindow = new QMainWindow();
            chartWindow->setWindowTitle("Фазовый портрет");
            chartWindow->setMinimumSize(800, 600);
            pchart = new QChart();
            pchart->setAnimationOptions(QChart::AllAnimations);
            pchartView = new QChartView(pchart);
            pchartView->setRenderHint(QPainter::Antialiasing, true);
            pchartView->setInteractive(true);
            pchartView->setRubberBand(QChartView::RectangleRubberBand);
            pchartView->setDragMode(QGraphicsView::ScrollHandDrag);
            pchartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            chartWindow->setCentralWidget(pchartView);
            chartWindow->show();
        }
        QSplineSeries *pseries = new QSplineSeries();
        for (const second_result &point : curve) {
            pseries->append(point.uv[0], point.uv[1]);
        }
        pseries->setName("Фазовая траектория " + QString::number(tabCounter));
        pchart->addSeries(pseries);
        pchart->createDefaultAxes();
        pchart->zoomReset();
        tabCounter++;
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    UpdateSumTable();
    chart->zoomReset();
}

void MainWindow::UpdateSumTable(){
    sumWidget->insertRow(sumWidget->rowCount());

    sumWidget->setItem(sumWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(sum.totalsteps + 1)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(sum.boundary)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(sum.maxele)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(sum.maxh)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(sum.xmaxh)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(sum.minh)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(sum.xminh)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(sum.maxdev)));
    sumWidget->setItem(sumWidget->rowCount() - 1, 8, new QTableWidgetItem(QString::number(sum.xmaxdev)));
}

void MainWindow::onClearPlotButtonClicked()
{
    chart->removeAllSeries();
    chart->createDefaultAxes();
    if(pchart){
        pchart->removeAllSeries();
        pchart->createDefaultAxes();
    }
    sumWidget->clearContents();
    sumWidget->setRowCount(0);
    tabWidget->clear();
    tabCounter = 1;
}
