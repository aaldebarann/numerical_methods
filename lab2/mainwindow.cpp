#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1200, 800);

    topLeftWidget = new QWidget(this);
    topRightWidget = new QWidget(this);
    bottomLeftWidget = new QWidget(this);
    bottomRightWidget = new QWidget(this);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(topLeftWidget, 0, 0);
    gridLayout->addWidget(topRightWidget, 0, 1);
    gridLayout->addWidget(bottomLeftWidget, 1, 0);
    gridLayout->addWidget(bottomRightWidget, 1, 1);
    topLeftWidget->setFixedSize(300, 270);
    bottomLeftWidget->setFixedSize(300, 530);
    centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    QLabel *label1 = new QLabel("Число разбиений сетки:", topLeftWidget);

    grid = new QLineEdit(topLeftWidget);
    grid->setText("10");
    grid->setFixedWidth(150);

    QPushButton *taskButton1 = new QPushButton("Тестовая задача", this);
    QPushButton *taskButton2 = new QPushButton("Основная задача", this);
    QPushButton *clearPlotButton = new QPushButton("Очистить график", this);

    summary = new QTextEdit();
    summary->setPlainText("Справка");
    summary->setReadOnly(true);

    QGridLayout *topLeftLayout = new QGridLayout;

    topLeftLayout->addWidget(taskButton1, 0, 0);
    topLeftLayout->addWidget(taskButton2, 0, 1);
    topLeftLayout->addWidget(label1, 1, 0);
    topLeftLayout->addWidget(grid, 1, 1);
    topLeftLayout->addWidget(clearPlotButton, 2, 0);
    topLeftLayout->addWidget(summary, 3, 0, 1, 2);
    topLeftWidget->setLayout(topLeftLayout);

    tabWidget = new QTabWidget(bottomLeftWidget);
    QVBoxLayout *bottomLeftLayout = new QVBoxLayout(bottomLeftWidget);
    bottomLeftLayout->addWidget(tabWidget);
    bottomLeftWidget->setLayout(bottomLeftLayout);

    QVBoxLayout *topRightLayout = new QVBoxLayout(topRightWidget);
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
    QVBoxLayout *bottomRightLayout = new QVBoxLayout(bottomRightWidget);
    bottomRightLayout->addWidget(chartView);
    bottomRightWidget->setLayout(bottomRightLayout);

    connect(taskButton1, &QPushButton::clicked, this, &MainWindow::onTask1Clicked);
    connect(taskButton2, &QPushButton::clicked, this, &MainWindow::onTask2Clicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::onClearPlotButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onTask1Clicked(){
    n = grid->text().toInt();
    QSplineSeries *series = new QSplineSeries();
    QSplineSeries *seriesve = new QSplineSeries();
    QSplineSeries *pseries = new QSplineSeries();

    tableWidget = new QTableWidget(tabWidget);
    QHeaderView *header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("u"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-u"));

    // Сюда код решения задачи, в котором нужно в series, seriesve добавить точки решения и обновить epsilon1, xerr
    // Так же не забыть про погрешность, но её можно вывести из точек в series, seriesve
    // Шаблон для добавления точек и строк в таблицу:
    //for (int i = 0; i < n + 1; i++){
    //    series->append(point.x, point.v);
    //    seriesve->append(point.x, point.u);
    //    pseries->appedn(point.x, point.v - point.u);
    //    tableWidget->insertRow(tableWidget->rowCount());
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(point.x)));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(point.v)));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(point.u)));
    //    tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(point.v - point.u)));
    //}


    pseries->setName("Погрешность " + QString::number(tabCounter));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Тестовая" + QString::number(tabCounter));
    seriesve->setName("Тестовая истинная" + QString::number(tabCounter));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Тестовая задача " + QString::number(tabCounter));
    tabCounter++;

    QString message = QString("Справка:  \n"
                              "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
                              "Задача должна быть решена с погрешностью не более ε = 10e-6 \n"
                              "Задача решена с погрешностью ε1 = %2 \n"
                              "Максимальное отклонение аналитического и численного решений наблюдается в точке x = %3 \n")
                      .arg(n)
                      .arg(epsilon1)
                      .arg(xerr);
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onTask2Clicked(){
    n = grid->text().toInt();
    QSplineSeries *series = new QSplineSeries();
    QSplineSeries *seriesve = new QSplineSeries();
    QSplineSeries *pseries = new QSplineSeries();

    tableWidget = new QTableWidget(tabWidget);
    QHeaderView *header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));

    // Сюда код решения задачи, в котором нужно в series, seriesve добавить точки решения и обновить epsilon1, xerr
    // Так же не забыть про погрешность, но её можно вывести из точек в series, seriesve

    pseries->setName("Погрешность " + QString::number(tabCounter));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Численная" + QString::number(tabCounter));
    seriesve->setName("Численная двойная" + QString::number(tabCounter));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Основная задача " + QString::number(tabCounter));
    tabCounter++;

    QString message = QString("Справка:  \n"
                              "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
                              "Задача должна быть решена с заданной точностью ε = 10e-6 \n"
                              "Задача решена с точностью ε2 = %2 \n"
                              "Максимальная разность численных решений в общих узлах сетки наблюдается в точке x = %3 \n")
                          .arg(n)
                          .arg(epsilon1)
                          .arg(xerr);
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onClearPlotButtonClicked(){
    chart->removeAllSeries();
    chart->createDefaultAxes();
    pchart->removeAllSeries();
    pchart->createDefaultAxes();
    summary->setPlainText("Справка");
    tabWidget->clear();
    tabCounter = 1;
}
