#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1300, 800);

    topLeftWidget = new QWidget(this);
    topRightWidget = new QWidget(this);
    bottomLeftWidget = new QWidget(this);
    bottomRightWidget = new QWidget(this);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(topLeftWidget, 0, 0);
    gridLayout->addWidget(topRightWidget, 0, 1);
    gridLayout->addWidget(bottomLeftWidget, 1, 0);
    gridLayout->addWidget(bottomRightWidget, 1, 1);
    topLeftWidget->setFixedSize(355, 270);
    bottomLeftWidget->setFixedSize(355, 530);
    centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    QLabel* label1 = new QLabel("Число разбиений сетки:", topLeftWidget);

    grid = new QLineEdit(topLeftWidget);
    grid->setText("10");
    grid->setFixedWidth(150);

    QPushButton* taskButton1 = new QPushButton("Тестовая задача", this);
    QPushButton* taskButton2 = new QPushButton("Основная задача", this);
    QPushButton* clearPlotButton = new QPushButton("Очистить график", this);

    summary = new QTextEdit();
    summary->setPlainText("Справка");
    summary->setReadOnly(true);

    QGridLayout* topLeftLayout = new QGridLayout;

    topLeftLayout->addWidget(taskButton1, 0, 0);
    topLeftLayout->addWidget(taskButton2, 0, 1);
    topLeftLayout->addWidget(label1, 1, 0);
    topLeftLayout->addWidget(grid, 1, 1);
    topLeftLayout->addWidget(clearPlotButton, 2, 0);
    topLeftLayout->addWidget(summary, 3, 0, 1, 2);
    topLeftWidget->setLayout(topLeftLayout);

    tabWidget = new QTabWidget(bottomLeftWidget);
    QVBoxLayout* bottomLeftLayout = new QVBoxLayout(bottomLeftWidget);
    bottomLeftLayout->addWidget(tabWidget);
    bottomLeftWidget->setLayout(bottomLeftLayout);

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

    connect(taskButton1, &QPushButton::clicked, this, &MainWindow::onTask1Clicked);
    connect(taskButton2, &QPushButton::clicked, this, &MainWindow::onTask2Clicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::onClearPlotButtonClicked);
}

MainWindow::~MainWindow()
{
}


void MainWindow::onTask1Clicked() {
    n = grid->text().toInt();
    n++;
    QLineSeries* series = new QLineSeries();
    QLineSeries* seriesve = new QLineSeries();
    QLineSeries* pseries = new QLineSeries();
    tableWidget = new QTableWidget(tabWidget);
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("u"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-u"));

    Functions::set_task("test");

    epsilon1 = 0;
    xerr = 0;
    high_precision_type ae1 = 0;
    high_precision_type mu1 = 0;
    high_precision_type ae2 = 0;
    high_precision_type mu2 = 1;
    high_precision_type x_0 = 0;
    high_precision_type x_n = 1;
    std::vector<high_precision_type> y(n);
    std::vector<high_precision_type> a;
    std::vector<high_precision_type> b;
    std::vector<high_precision_type> c;
    std::vector<high_precision_type> phi;

    buildLES(n, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y);

    a.clear();
    b.clear();
    c.clear();
    phi.clear();

    high_precision_type h = (high_precision_type("1.0") / (high_precision_type)(n - 1));
    for (int i = 0; i < n; i++) {
        high_precision_type x_i = x_0 + i * h;
        high_precision_type u = Functions::tr_f(x_i);
        if (epsilon1 < mp::abs(y[i] - u)) {
            epsilon1 = mp::abs(y[i] - u);
            xerr = x_i;
        }
    }
    std::cout << "eps: " << epsilon1.convert_to<std::string>() << std::endl;
    std::cout << "x: " << xerr.convert_to<std::string>() << "n: " << n - 1 << std::endl;
    if (n <= 10001){
        for (int i = 0; i < y.size(); i++) {
            high_precision_type x_i = x_0 + i * h;
            high_precision_type u = Functions::tr_f(x_i);
            series->append(static_cast<double>(x_i), static_cast<double>(y[i]));
            seriesve->append(static_cast<double>(x_i), static_cast<double>(u));
            pseries->append(static_cast<double>(x_i), static_cast<double>(mp::abs(y[i] - u)));
        }
        for(int i = 0; i < y.size(); i++){
            high_precision_type x_i = x_0 + i * (1.0 / (high_precision_type)(n - 1));
            high_precision_type u = Functions::tr_f(x_i);
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(static_cast<double>(x_i))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(u))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[i] - u)))));
        }
    } else {
        int skp = n / 1000;
        for (int i = 0; i < y.size(); i += skp) {
            high_precision_type x_i = x_0 + i * h;
            high_precision_type u = Functions::tr_f(x_i);
            series->append(static_cast<double>(x_i), static_cast<double>(y[i]));
            seriesve->append(static_cast<double>(x_i), static_cast<double>(u));
            pseries->append(static_cast<double>(x_i), static_cast<double>(mp::abs(y[i] - u)));
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(static_cast<double>(x_i))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(u))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[i] - u)))));
        }
        if(y.size() % skp != 1){
            high_precision_type u = Functions::tr_f(x_n);
            series->append(static_cast<double>(x_n), static_cast<double>(y[y.size() - 1]));
            seriesve->append(static_cast<double>(x_n), static_cast<double>(u));
            pseries->append(static_cast<double>(x_n), static_cast<double>(mp::abs(y[y.size() - 1] - u)));
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(static_cast<double>(x_n))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[y.size() - 1]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(u))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[y.size()- 1] - u)))));
        }
    }
    pseries->setName("Погр., n = " + QString::number(n - 1));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Тест., n = " + QString::number(n - 1));
    seriesve->setName("Истинная, n = " + QString::number(n - 1));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Тест., n = " + QString::number(n - 1));

    QString message = QString("Справка:  \n"
        "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
        "Задача должна быть решена с погрешностью не более ε = 10e-6 \n"
        "Задача решена с погрешностью ε1 = %2 \n"
        "Максимальное отклонение аналитического и численного решений наблюдается в точке x = %3 ")
        .arg(n - 1)
        .arg(static_cast<double>(epsilon1))
        .arg(static_cast<double>(xerr));
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onTask2Clicked() {
    n = grid->text().toInt();
    n++;
    QLineSeries* series = new QLineSeries();
    QLineSeries* seriesve = new QLineSeries();
    QLineSeries* pseries = new QLineSeries();
    tableWidget = new QTableWidget(tabWidget);
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));

    Functions::set_task("main");

    epsilon1 = 0;
    xerr = 0;
    high_precision_type ae1 = 0;
    high_precision_type mu1 = 0;
    high_precision_type ae2 = 0;
    high_precision_type mu2 = 1;
    high_precision_type x_0 = 0;
    high_precision_type x_n = 1;
    std::vector<high_precision_type> epss;
    std::vector<high_precision_type> y(n);
    std::vector<high_precision_type> y2(n * 2 - 1);
    std::vector<high_precision_type> a;
    std::vector<high_precision_type> b;
    std::vector<high_precision_type> c;
    std::vector<high_precision_type> phi;

    buildLES(n, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y);

    a.clear();
    b.clear();
    c.clear();
    phi.clear();

    buildLES(n * 2 - 1, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y2);

    high_precision_type h1 = high_precision_type("1.0") / (high_precision_type)(n - 1);
    for (int i = 0; i < y.size(); i++) {
        if (epsilon1 < mp::abs(y[i] - y2[2 * i])) {
            epsilon1 = mp::abs(y[i] - y2[2 * i]);
            xerr = x_0 + i * h1;
        }
    }
    std::cout << "eps: " << epsilon1.convert_to<std::string>() << std::endl;
    std::cout << "x: " << xerr.convert_to<std::string>() << "n: " << n - 1 << std::endl;
    if (n <= 10001){
        for (int i = 0; i < y.size(); i++) {
            high_precision_type x_i = x_0 + i * h1;
            series->append(static_cast<double>(x_i), static_cast<double>(y[i]));
            seriesve->append(static_cast<double>(x_i), static_cast<double>(y2[2 * i]));
            pseries->append(static_cast<double>(x_i), static_cast<double>(mp::abs(y[i] - y2[2 * i])));
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new  QTableWidgetItem(QString::number(static_cast<double>(x_i))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(y2[2 * i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[i] - y2[2 * i])))));
        }
    } else {
        int skp = n / 1000;
        for (int i = 0; i < y.size(); i += skp) {
            high_precision_type x_i = x_0 + i * h1;
            series->append(static_cast<double>(x_i), static_cast<double>(y[i]));
            seriesve->append(static_cast<double>(x_i), static_cast<double>(y2[2 * i]));
            pseries->append(static_cast<double>(x_i), static_cast<double>(mp::abs(y[i] - y2[2 * i])));
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new  QTableWidgetItem(QString::number(static_cast<double>(x_i))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(y2[2 * i]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[i] - y2[2 * i])))));
        }
        if (y.size() % skp != 1) {
            series->append(static_cast<double>(x_n), static_cast<double>(y[y.size() - 1]));
            seriesve->append(static_cast<double>(x_n), static_cast<double>(y2[y2.size() - 1]));
            pseries->append(static_cast<double>(x_n), static_cast<double>(mp::abs(y[y.size() - 1] - y2[y2.size() - 1])));
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1, 0, new  QTableWidgetItem(QString::number(static_cast<double>(x_n))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(static_cast<double>(y[y.size() - 1]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(static_cast<double>(y2[y2.size() - 1]))));
            tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(static_cast<double>(mp::abs(y[y.size() - 1] - y2[y2.size() - 1])))));

        }
    }
    pseries->setName("Погр., n = " + QString::number(n - 1));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Числ., n = " + QString::number(n - 1));
    seriesve->setName("Числ. дв., n = " + QString::number(n - 1));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Осн., n = " + QString::number(n - 1));


    QString message = QString("Справка:  \n"
        "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
        "Задача должна быть решена с заданной точностью ε = 10e-6 \n"
        "Задача решена с точностью ε2 = %2 \n"
        "Максимальная разность численных решений в общих узлах сетки наблюдается в точке x = %3 ")
        .arg(n - 1)
        .arg(static_cast<double>(epsilon1))
        .arg(static_cast<double>(xerr));
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onClearPlotButtonClicked() {
    chart->removeAllSeries();
    chart->createDefaultAxes();
    pchart->removeAllSeries();
    pchart->createDefaultAxes();
    summary->setPlainText("Справка");
    tabWidget->clear();
}
