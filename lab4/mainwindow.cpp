#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    chart = new QChart();
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTheme(QChart::ChartThemeDark);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setInteractive(true);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    chartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QSize screenSize = chartView->screen()->size();
    chartView->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.3));
    chartView->setMaximumSize(screenSize);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *controlsWidget = new QWidget();
    controlsWidget->setMaximumWidth(215);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlsWidget);

    n = new QLineEdit(this);
    n->setText("3");
    QLabel* label1 = new QLabel("Число разбиений:", controlsWidget);
    bca = new QLineEdit(this);
    bca->setText("0");
    QLabel* label2 = new QLabel("Граничное условие в a:", controlsWidget);
    bcb = new QLineEdit(this);
    bcb->setText("0");
    QLabel* label3 = new QLabel("Граничное условие в b:", controlsWidget);
    eps = new QLineEdit(this);
    eps->setText("0.001");
    QLabel* label4 = new QLabel("Контроль точности ε:", controlsWidget);

    QPushButton *graphButton = new QPushButton("Численное решение", this);
    QPushButton *trueGraphButton = new QPushButton("Аналитическое решение", this);
    QPushButton *errorButton = new QPushButton("Погрешность решения", this);
    QPushButton *tableButton = new QPushButton("Таблица", this);
    QPushButton *removegraphButton = new QPushButton("Скрыть численное решение", this);
    QPushButton *removetrueGraphButton = new QPushButton("Скрыть аналитическое решение", this);
    QPushButton *removeerrorGraphButton = new QPushButton("Скрыть погрешность решения", this);
    QPushButton *summaryButton = new QPushButton("Справка", this);
    QRadioButton *taskVariant0 = new QRadioButton("Тестовая функция", this);
    QRadioButton *taskVariant1 = new QRadioButton("Основная функция", this);
    QRadioButton *taskVariant2 = new QRadioButton("Периодическая функция", this);
    QRadioButton *funcVariant0 = new QRadioButton("Основная функция 1", this);
    QRadioButton *funcVariant1 = new QRadioButton("Основная функция 2", this);
    QRadioButton *funcVariant2 = new QRadioButton("Основная функция 3", this);
    QRadioButton *graphVariant0 = new QRadioButton("График функции", this);
    QRadioButton *graphVariant1 = new QRadioButton("График производной", this);
    QRadioButton *graphVariant2 = new QRadioButton("График второй производной", this);

    QButtonGroup *taskbuttonGroup1 = new QButtonGroup(this);
    taskbuttonGroup1->addButton(taskVariant0);
    taskbuttonGroup1->addButton(taskVariant1);
    taskbuttonGroup1->addButton(taskVariant2);
    taskVariant0->setChecked(true);

    QButtonGroup *taskbuttonGroup2 = new QButtonGroup(this);
    taskbuttonGroup2->addButton(funcVariant0);
    taskbuttonGroup2->addButton(funcVariant1);
    taskbuttonGroup2->addButton(funcVariant2);
    funcVariant0->setChecked(true);

    QButtonGroup *taskbuttonGroup3 = new QButtonGroup(this);
    taskbuttonGroup3->addButton(graphVariant0);
    taskbuttonGroup3->addButton(graphVariant1);
    taskbuttonGroup3->addButton(graphVariant2);
    graphVariant0->setChecked(true);

    connect(graphButton, &QPushButton::clicked, this, &MainWindow::showGraph);
    connect(trueGraphButton, &QPushButton::clicked, this, &MainWindow::showTrueGraph);
    connect(errorButton, &QPushButton::clicked, this, &MainWindow::showErrorGraph);
    connect(removegraphButton, &QPushButton::clicked, this, &MainWindow::removeGraph);
    connect(removetrueGraphButton, &QPushButton::clicked, this, &MainWindow::removeTrueGraph);
    connect(removeerrorGraphButton, &QPushButton::clicked, this, &MainWindow::removeErrorGraph);
    connect(tableButton, &QPushButton::clicked, this, &MainWindow::showTable);
    connect(summaryButton, &QPushButton::clicked, this, &MainWindow::showSummary);

    connect(taskVariant0, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked1);
    connect(taskVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked1);
    connect(taskVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked1);
    connect(funcVariant0, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(funcVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(funcVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(graphVariant0, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    connect(graphVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    connect(graphVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    \
    controlLayout->addWidget(taskVariant0);
    controlLayout->addWidget(taskVariant1);
    controlLayout->addWidget(taskVariant2);
    controlLayout->addWidget(funcVariant0);
    controlLayout->addWidget(funcVariant1);
    controlLayout->addWidget(funcVariant2);
    controlLayout->addWidget(graphVariant0);
    controlLayout->addWidget(graphVariant1);
    controlLayout->addWidget(graphVariant2);
    controlLayout->addWidget(label1);
    controlLayout->addWidget(n);
    controlLayout->addWidget(label2);
    controlLayout->addWidget(bca);
    controlLayout->addWidget(label3);
    controlLayout->addWidget(bcb);
    controlLayout->addWidget(label4);
    controlLayout->addWidget(eps);
    controlLayout->addWidget(graphButton);
    controlLayout->addWidget(trueGraphButton);
    controlLayout->addWidget(errorButton);
    controlLayout->addWidget(tableButton);
    controlLayout->addWidget(removegraphButton);
    controlLayout->addWidget(removetrueGraphButton);
    controlLayout->addWidget(removeerrorGraphButton);
    controlLayout->addWidget(summaryButton);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(controlsWidget);
    mainLayout->addWidget(chartView);

}

MainWindow::~MainWindow() {}

void MainWindow::onRadioButtonClicked1(){ // поменяешь логику как будет удобнее
    QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "Тестовая функция") {
        task_choice = 0;
    } else if (selectedButton->text() == "Основная функция") {
        task_choice = 1;
    } else if (selectedButton->text() == "Периодическая функция") {
        task_choice = 2;
    }
}

void MainWindow::onRadioButtonClicked2(){ // поменяешь логику как будет удобнее
    QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "Основная функция 1") {
        function_choice = 0;
    } else if (selectedButton->text() == "Основная функция 2") {
        function_choice = 1;
    } else if (selectedButton->text() == "Основная функция 3") {
        function_choice = 2;
    }
}

void MainWindow::onRadioButtonClicked3(){ // поменяешь логику как будет удобнее
    QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "График функции") {
        graph_choice = 0;
    } else if (selectedButton->text() == "График производной") {
        graph_choice = 1;
    } else if (selectedButton->text() == "График второй производной") {
        graph_choice = 2;
    }
}

void MainWindow::showGraph(){
    int N = n->text().toInt();
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();
    double epsilon = eps->text().toDouble();

    QLineSeries* series = new QLineSeries();
    for (int i = 0; i < N + 1; i++) {
        //series->append(x_i, (y[i]));
    }
    series->setName("Численная функция, n = " + QString::number(N));
    seriesStack.push(series);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::showTrueGraph(){
    int N = n->text().toInt();
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();
    double epsilon = eps->text().toDouble();

    QLineSeries* seriesve = new QLineSeries();
    for (int i = 0; i < N + 1; i++) {
        //seriesve->append(x_i, (y[i]));
    }
    seriesve->setName("Истинная, n = " + QString::number(N));
    solutionStack.push(seriesve);
    chart->addSeries(seriesve);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::showErrorGraph(){
    int N = n->text().toInt();
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();
    double epsilon = eps->text().toDouble();

    QLineSeries* pseries = new QLineSeries();
    for (int i = 0; i < N + 1; i++) {
        //pseries->append(x_i, (y[i]));
    }
    pseries->setName("Погр., n = " + QString::number(N));
    errorStack.push(pseries);
    chart->addSeries(pseries);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::removeGraph(){
    if(seriesStack.top()){
        chart->removeSeries(seriesStack.top());
        seriesStack.pop();
    }
}

void MainWindow::removeTrueGraph(){
    if(solutionStack.top()){
        chart->removeSeries(solutionStack.top());
        solutionStack.pop();
    }
}

void MainWindow::removeErrorGraph(){
    if(errorStack.top()){
        chart->removeSeries(errorStack.top());
        errorStack.pop();
    }
}

void MainWindow::showTable(){
    int N = n->text().toInt();
    QTabWidget *tabWidget = new QTabWidget;
    QTableWidget *tableWidget1 = new QTableWidget((2 * N + 1), 10);
    tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget1->setHorizontalHeaderLabels({"X", "F(x)", "S(x)", "F(x) - S(x)", "F'(x)", "S'(x)", "F'(x) - S'(x)", "F''(x)", "S''(x)", "F''(x) - S''(x)"});
    for (int i = 0; i <= 2 * N + 1; i++) {
        tableWidget1->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 1, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 2, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 3, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 4, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 5, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 6, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 7, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 8, new QTableWidgetItem(QString::number(i)));
        tableWidget1->setItem(i, 9, new QTableWidgetItem(QString::number(i)));
    }
    tabWidget->addTab(tableWidget1, "Таблица проверки " + QString::number(tabWidget->count() / 2 + 1));
    QTableWidget *tableWidget2 = new QTableWidget((N + 1), 6);
    tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget2->setHorizontalHeaderLabels({"xᵢ₋₁", "xᵢ", "aᵢ", "bᵢ", "cᵢ", "dᵢ"});
    for (int i = 0; i <= N + 1; i++) {
        tableWidget2->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        tableWidget2->setItem(i, 1, new QTableWidgetItem(QString::number(i)));
        tableWidget2->setItem(i, 2, new QTableWidgetItem(QString::number(i)));
        tableWidget2->setItem(i, 3, new QTableWidgetItem(QString::number(i)));
        tableWidget2->setItem(i, 4, new QTableWidgetItem(QString::number(i)));
        tableWidget2->setItem(i, 5, new QTableWidgetItem(QString::number(i)));
    }
    tabWidget->addTab(tableWidget2, "Таблица коэффициентов " + QString::number(tabWidget->count() / 2 + 1));
    tabWidget->setMinimumSize(800, 400);
    tabWidget->setWindowTitle("Таблица");
    QIcon icon = QIcon("icon.ico");
    tabWidget->setWindowIcon(icon);
    tabWidget->setGeometry(100, 100, tableWidget1->width(), 600);
    tabWidget->show();
}

void MainWindow::showSummary(){
    QDialog *summaryBox = new QDialog();
    QIcon icon = QIcon("icon.ico");
    summaryBox->setWindowIcon(icon);
    summaryBox->setWindowTitle("Справка ☭");
    summaryBox->setMinimumSize(400, 250);
    int N = n->text().toInt();
    summary = new QLabel("Справка:", summaryBox);
    summary->setAlignment(Qt::AlignCenter);
    summary->setWordWrap(true);
    summary->setTextFormat(Qt::RichText);
    summary->setText("<center>"
                     "<h3>"
                     "<p>Сетка сплайна: n = " + QString::number(N) + "</p><p>Контрольная сетка: N = " + QString::number(2 * N) +
                     "</p><p>Погрешность сплайна на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F(x<sub>j</sub>)-S(x<sub>j</sub>)| = " + QString::number(1) + " при x = " + QString::number(1) +
                     "</p><p>Погрешность производной на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F'(x<sub>j</sub>)-S'(x<sub>j</sub>)| = " + QString::number(1) + " при x = " + QString::number(1) +
                     "</p><p>Погрешность второй производной на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F''(x<sub>j</sub>)-S''(x<sub>j</sub>)| = " + QString::number(1) + " при x = " + QString::number(1) +
                     "</p></h3>"
                     "</center>");
    summaryBox->show();
}

