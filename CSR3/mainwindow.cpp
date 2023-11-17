#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1300, 800);

    topLeftWidget = new QWidget(this);
    topRightWidget = new QWidget(this);
    bottomRightWidget = new QWidget(this);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(topLeftWidget, 0, 0);
    gridLayout->addWidget(topRightWidget, 0, 1);
    gridLayout->addWidget(bottomRightWidget, 1, 1);
    topLeftWidget->setFixedSize(400, 270);
    topRightWidget->setFixedSize(900, 270);
    bottomRightWidget->setFixedSize(900, 530);
    centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    QLabel* label1 = new QLabel("Шаг:", topLeftWidget);
    label1->setAlignment(Qt::AlignRight);

    step = new QLineEdit(topLeftWidget);
    step->setText("10");
    step->setFixedWidth(50);

    QLabel* label2 = new QLabel("Левая граница:", topLeftWidget);
    label2->setAlignment(Qt::AlignRight);

    x0 = new QLineEdit(topLeftWidget);
    x0->setText("0");
    x0->setFixedWidth(50);

    QLabel* label3 = new QLabel("Правая граница:", topLeftWidget);
    label3->setAlignment(Qt::AlignRight);

    xn = new QLineEdit(topLeftWidget);
    xn->setText("10");
    xn->setFixedWidth(50);

    QLabel* label4 = new QLabel("Коэффициенты системы:", topLeftWidget);
    label4->setAlignment(Qt::AlignRight);

    QLabel* labelai1 = new QLabel(QString("a<sub>i1</sub>"), topLeftWidget);
    QLabel* labelai2 = new QLabel(QString("a<sub>i2</sub>"), topLeftWidget);
    QLabel* labela1j = new QLabel(QString("a<sub>1j</sub>"), topLeftWidget);
    QLabel* labela2j = new QLabel(QString("a<sub>2j</sub>"), topLeftWidget);
    labela1j->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    labela2j->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    labelai1->setAlignment(Qt::AlignRight);
    labelai2->setAlignment(Qt::AlignRight);

    a11 = new QLineEdit(topLeftWidget);
    a11->setText("1");
    a11->setFixedWidth(70);

    a12 = new QLineEdit(topLeftWidget);
    a12->setText("1");
    a12->setFixedWidth(70);

    a21 = new QLineEdit(topLeftWidget);
    a21->setText("1");
    a21->setFixedWidth(70);

    a22 = new QLineEdit(topLeftWidget);
    a22->setText("1");
    a22->setFixedWidth(70);

    QLabel* label5 = new QLabel("Начальные условия:", topLeftWidget);
    QLabel* labelu1 = new QLabel(QString("u<sub>1</sub>"), topLeftWidget);
    QLabel* labelu2 = new QLabel(QString("u<sub>2</sub>"), topLeftWidget);
    label5->setAlignment(Qt::AlignRight);
    labelu1->setAlignment(Qt::AlignRight);
    labelu2->setAlignment(Qt::AlignRight);

    u1 = new QLineEdit(topLeftWidget);
    u1->setText("0");
    u1->setFixedWidth(70);

    u2 = new QLineEdit(topLeftWidget);
    u2->setText("10");
    u2->setFixedWidth(70);

    QPushButton* taskButton1 = new QPushButton("График", this);
    QPushButton* taskButton2 = new QPushButton("Таблица", this);
    QPushButton* clearPlotButton = new QPushButton("Очистить график", this);

    taskButton1->setFixedHeight(30);
    taskButton2->setFixedHeight(30);
    clearPlotButton->setFixedHeight(30);

    QGridLayout* topLeftLayout = new QGridLayout;
    topLeftLayout->addWidget(label1, 0, 0);
    topLeftLayout->addWidget(step, 0, 1);
    topLeftLayout->addWidget(label2, 1, 0);
    topLeftLayout->addWidget(x0, 1, 1);
    topLeftLayout->addWidget(label3, 2, 0);
    topLeftLayout->addWidget(xn, 2, 1);
    topLeftLayout->addWidget(label4, 3, 0);
    topLeftLayout->addWidget(labela1j, 3, 1);
    topLeftLayout->addWidget(labela2j, 3, 2);
    topLeftLayout->addWidget(labelai1, 4, 0);
    topLeftLayout->addWidget(a11, 4, 1);
    topLeftLayout->addWidget(a12, 4, 2);
    topLeftLayout->addWidget(labelai2, 5, 0);
    topLeftLayout->addWidget(a21, 5, 1);
    topLeftLayout->addWidget(a22, 5, 2);
    topLeftLayout->addWidget(label5, 6, 0);
    topLeftLayout->addWidget(labelu1, 7, 0);
    topLeftLayout->addWidget(u1, 7, 1);
    topLeftLayout->addWidget(labelu2, 8, 0);
    topLeftLayout->addWidget(u2, 8, 1);
    topLeftLayout->addWidget(taskButton1, 9, 0);
    topLeftLayout->addWidget(taskButton2, 9, 1);
    topLeftLayout->addWidget(clearPlotButton, 9, 2);
    topLeftWidget->setLayout(topLeftLayout);

    QVBoxLayout* topRightLayout = new QVBoxLayout(topRightWidget);
    pchart = new QChart();
    pchart->createDefaultAxes();
    pchart->setAnimationOptions(QChart::AllAnimations);
    paxisX = new QValueAxis;
    paxisX->setTitleText("Точность");
    pchart->addAxis(paxisX, Qt::AlignBottom);
    paxisX->setGridLineVisible(true);

    paxisY = new QValueAxis;
    paxisY->setTitleText("X");
    pchart->addAxis(paxisY, Qt::AlignLeft);
    paxisY->setGridLineVisible(true);

    paxisX->setGridLineVisible(true);
    paxisY->setGridLineVisible(true);

    pchart->setMargins(QMargins(0, 0, 0, 0));

    pchartView = new QChartView(pchart);
    pchartView->setRenderHint(QPainter::Antialiasing, true);

    topRightLayout->addWidget(pchartView);
    topRightWidget->setLayout(topRightLayout);

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
    axisY->setTitleText("V");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setGridLineVisible(true);

    chart->setMargins(QMargins(0, 0, 0, 0));

    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setInteractive(true);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    chartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    QVBoxLayout* bottomRightLayout = new QVBoxLayout(bottomRightWidget);
    bottomRightLayout->addWidget(chartView);
    bottomRightWidget->setLayout(bottomRightLayout);

    connect(taskButton1, &QPushButton::clicked, this, &MainWindow::ShowGraph);
    connect(taskButton2, &QPushButton::clicked, this, &MainWindow::ShowTable);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::ClearGraph);
}

MainWindow::~MainWindow()
{
}

void  MainWindow::ShowGraph(){
    double h = step->text().toDouble();
    double x_0 = x0->text().toDouble();
    double x_n = xn->text().toDouble();
    double a_11 = a11->text().toDouble();
    double a_12 = a12->text().toDouble();
    double a_21 = a21->text().toDouble();
    double a_22 = a22->text().toDouble();
    double u_1 = u1->text().toDouble();
    double u_2 = u2->text().toDouble();

    QLineSeries* series1 = new QLineSeries();
    QLineSeries* series2 = new QLineSeries();
    QLineSeries* seriesve1 = new QLineSeries();
    QLineSeries* seriesve2 = new QLineSeries();
    QLineSeries* pseries1 = new QLineSeries();
    QLineSeries* pseries2 = new QLineSeries();

    // вызов функции решения c аргументами выше

    // Сюда ебашишь точки, только поменяй нахуй всё, я чисто ctrl c + ctrl v сделал
    //for(int i = 0; i < x.size(); i++){
    //    series1->append(x[i], y1[i]);
    //    series1->append(x[i], y2[i]);
    //    seriesve1->append(x[i], u1[i]);
    //    seriesve2->append(x[i], u2[i]);
    //    pseries1->append(x[i], abs(y1[i] - u1[i]));
    //    pseries2->append(x[i], abs(y2[i] - u2[i]));
    //}

    pseries1->setName("Погр. y1, h = " + QString::number(h));
    pseries2->setName("Погр. y2, h = " + QString::number(h));
    pchart->addSeries(pseries1);
    pchart->addSeries(pseries2);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series1->setName("Числ. y1, h = " + QString::number(h));
    series2->setName("Числ. y2, h = " + QString::number(h));
    seriesve1->setName("Ист. u1, h = " + QString::number(h));
    seriesve2->setName("Ист. u2, h = " + QString::number(h));
    chart->addSeries(seriesve1);
    chart->addSeries(seriesve2);
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void  MainWindow::ShowTable(){
    double h = step->text().toDouble();
    double x_0 = x0->text().toDouble();
    double x_n = xn->text().toDouble();
    double a_11 = a11->text().toDouble();
    double a_12 = a12->text().toDouble();
    double a_21 = a21->text().toDouble();
    double a_22 = a22->text().toDouble();
    double u_1 = u1->text().toDouble();
    double u_2 = u2->text().toDouble();

    QDialog* tableDialog = new QDialog(this);
    tableDialog->setWindowTitle("Таблица для h = " + QString::number(h));
    QTableWidget* tableWidget = new QTableWidget(10, 5, tableDialog);
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    // сюда напиши заголовки столбцов сколько нужно
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    // вызов функции решения c аргументами выше

    // Сюда ебашишь точки, только поменяй нахуй всё, я чисто ctrl c + ctrl v сделал

    //for(int i = 0; i < x.size(); i++){
    //    tableWidget->insertRow(tableWidget->rowCount());
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number()));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number()));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number()));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number()));
    //}

    QVBoxLayout* layout = new QVBoxLayout(tableDialog);
    layout->addWidget(tableWidget);
    tableDialog->setLayout(layout);
    tableDialog->setAttribute(Qt::WA_DeleteOnClose);
    tableDialog->show();
}

void  MainWindow::ClearGraph(){
    chart->removeAllSeries();
    chart->createDefaultAxes();
    pchart->removeAllSeries();
    pchart->createDefaultAxes();
}
