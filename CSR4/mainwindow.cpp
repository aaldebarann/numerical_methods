#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1000, 700);
    //Создадим два виджета и поделим окошко сплитом пополам по вертикали
    QWidget *leftWidget = new QWidget(this);
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *splitLayout = new QHBoxLayout;
    splitLayout->addWidget(leftWidget);
    splitLayout->addWidget(rightWidget);
    mainLayout->addLayout(splitLayout);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);

    //Создадим все поля ввода, текстовые обозначения и таблицу в левой части
    QLabel *label1 = new QLabel("Число разбиений по пространству:", leftWidget);
    QLabel *label2 = new QLabel("Число разбиений по времени:", leftWidget);
    Mn = new QLineEdit(leftWidget);
    Mn->setAlignment(Qt::AlignLeft);
    Mn->setText("100");
    Mn->setFixedWidth(150);
    Nn = new QLineEdit(leftWidget);
    Nn->setAlignment(Qt::AlignLeft);
    Nn->setText("100");
    Nn->setFixedWidth(150);
    QPushButton *drawButton = new QPushButton("Отрисовать", this);
    drawButton->setFixedWidth(150);
    QPushButton *tableButton  = new QPushButton("Таблица", this);
    drawButton->setFixedWidth(150);
    QPushButton *clearPlotButton = new QPushButton("Очистить", this);
    clearPlotButton->setFixedWidth(150);
    QPushButton *taskButton = new QPushButton("Задача", this);
    taskButton->setFixedWidth(150);
    QGridLayout *leftLayout = new QGridLayout;
    leftLayout->addWidget(label1, 0, 0);
    leftLayout->addWidget(Nn, 0, 1);
    leftLayout->addWidget(label2, 1, 0);
    leftLayout->addWidget(Mn, 1, 1);
    leftLayout->addWidget(drawButton, 2, 0);
    leftLayout->addWidget(tableButton, 2, 1);
    leftLayout->addWidget(clearPlotButton, 3, 0);
    leftLayout->addWidget(taskButton, 3, 1);
    tableWidget = new QTableWidget(leftWidget);
    leftLayout->addWidget(tableWidget, 4, 0, 1, 3);
    leftWidget->setLayout(leftLayout);


    ichart = new QChart();
    ichart->createDefaultAxes();
    ichart->setAnimationOptions(QChart::AllAnimations);
    ichart->legend()->setVisible(false);
    ichart->legend()->setAlignment(Qt::AlignBottom);

    iaxisX = new QValueAxis;
    iaxisX->setTitleText("X");
    ichart->addAxis(iaxisX, Qt::AlignBottom);
    iaxisX->setGridLineVisible(true);

    iaxisY = new QValueAxis;
    iaxisY->setTitleText("U");
    ichart->addAxis(iaxisY, Qt::AlignLeft);
    iaxisY->setGridLineVisible(true);

    iaxisX->setGridLineVisible(true);
    iaxisY->setGridLineVisible(true);

    ichart->setMargins(QMargins(0, 0, 0, 0));

    ichartView = new QChartView(ichart);
    ichartView->setRenderHint(QPainter::Antialiasing, true);
    ichartView->setInteractive(true);
    ichartView->setRubberBand(QChartView::RectangleRubberBand);
    ichartView->setDragMode(QGraphicsView::ScrollHandDrag);
    ichartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    jchart = new QChart();
    jchart->createDefaultAxes();
    jchart->setAnimationOptions(QChart::AllAnimations);
    jchart->legend()->setVisible(false);
    jchart->legend()->setAlignment(Qt::AlignBottom);

    jaxisX = new QValueAxis;
    jaxisX->setTitleText("X");
    jchart->addAxis(jaxisX, Qt::AlignBottom);
    jaxisX->setGridLineVisible(true);

    jaxisY = new QValueAxis;
    jaxisY->setTitleText("U");
    jchart->addAxis(jaxisY, Qt::AlignLeft);
    jaxisY->setGridLineVisible(true);

    jaxisX->setGridLineVisible(true);
    jaxisY->setGridLineVisible(true);

    jchart->setMargins(QMargins(0, 0, 0, 0));

    jchartView = new QChartView(jchart);
    jchartView->setRenderHint(QPainter::Antialiasing, true);
    jchartView->setInteractive(true);
    jchartView->setRubberBand(QChartView::RectangleRubberBand);
    jchartView->setDragMode(QGraphicsView::ScrollHandDrag);
    jchartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QGridLayout *rightLayout = new QGridLayout(rightWidget);
    QLabel *label3 = new QLabel("Срез по времени j = ", rightWidget);
    label3->setAlignment(Qt::AlignRight);
    QLabel *label4 = new QLabel("Судьба узла i = ", rightWidget);
    label4->setAlignment(Qt::AlignRight);
    in = new QLineEdit(leftWidget);
    in->setAlignment(Qt::AlignLeft);
    in->setText("0");
    in->setFixedWidth(160);
    jn = new QLineEdit(leftWidget);
    jn->setAlignment(Qt::AlignLeft);
    jn->setText("0");
    jn->setFixedWidth(160);

    rightLayout->addWidget(jchartView, 0, 0, 1, 2);
    rightLayout->addWidget(label3, 1, 0);
    rightLayout->addWidget(jn, 1, 1);
    rightLayout->addWidget(ichartView, 2, 0, 1, 2);
    rightLayout->addWidget(label4, 3, 0);
    rightLayout->addWidget(in, 3, 1);
    rightWidget->setLayout(rightLayout);

    connect(drawButton, &QPushButton::clicked, this, &MainWindow::onDrawButtonClicked);
    connect(tableButton, &QPushButton::clicked, this, &MainWindow::onTableButtonClicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::onClearPlotButtonClicked);
    connect(taskButton, &QPushButton::clicked, this, &MainWindow::onTaskButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onDrawButtonClicked(){
    int m = Mn->text().toInt();
    int n = Nn->text().toInt();
    int i = in->text().toInt();
    int j = jn->text().toInt();
    QLineSeries *iseries = new QLineSeries();
    QLineSeries *jseries = new QLineSeries();
    std::vector<std::vector<double>> curve(n+1);// затычка для теста
    for (std::vector<double> &point : curve) { // можно так
        point.resize(m + 1);// затычка для теста
        for(int k = 0; k < m+1; k++){
            iseries->append(point[k], point[j]);// затычка для теста
        }
        jseries->append(point[i], point[j]);// затычка для теста
    }
    ichart->addSeries(iseries);
    jchart->addSeries(jseries);
    ichart->createDefaultAxes();
    ichart->zoomReset();
    jchart->createDefaultAxes();
    jchart->zoomReset();
}

void MainWindow::onTableButtonClicked(){
    int m = Mn->text().toInt();
    int n = Nn->text().toInt();
    tableWidget->clearContents();
    tableWidget->clear();
    tableWidget->setColumnCount(0);
    tableWidget->setRowCount(0);
    tableWidget->verticalHeader()->hide();
    QHeaderView *header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(m + 2);
    QStringList columnHeaders;
    columnHeaders.append(QString("i, j"));
    for (int i = 0; i < m + 1; ++i) { // а можно и так
        columnHeaders.append(QString::number(i));
    }
    tableWidget->setHorizontalHeaderLabels(columnHeaders);
    //тут считай своё решение, m и n есть
    std::vector<std::vector<double>> point(n + 1); // затычка для теста
    for (int j = 0; j < n + 1; ++j) {
        tableWidget->insertRow(j);
        tableWidget->setItem(j, 0, new QTableWidgetItem(QString::number(j)));
        point[j].resize(m + 1); // чисто затычка для теста
        for (int i = 0; i < m + 1; ++i) {
            tableWidget->setItem(j, i + 1, new QTableWidgetItem(QString::number(point[j][i]))); // point[j][i] на твое решение
        }
    }
}

void MainWindow::onClearPlotButtonClicked(){
    tableWidget->clear();
    tableWidget->setColumnCount(0);
    ichart->removeAllSeries();
    ichart->createDefaultAxes();
    jchart->removeAllSeries();
    jchart->createDefaultAxes();
}

void MainWindow::onTaskButtonClicked(){
    QDialog *imageDialog= new QDialog(this);
    imageDialog->setWindowTitle("Задача");
    QLabel *imageLabel = new QLabel(imageDialog);
    imageLabel->setPixmap(QPixmap("task1.png"));
    QVBoxLayout *layout = new QVBoxLayout(imageDialog);
    layout->addWidget(imageLabel);
    imageLabel->setMaximumSize(800, 800);
    imageLabel->setScaledContents(true);
    layout->addWidget(imageLabel);
    imageDialog->exec();
}
