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
    n->setText("4");
    QLabel* label1 = new QLabel("Число разбиений:", controlsWidget);
    bca = new QLineEdit(this);
    bca->setText("0");
    QLabel* label2 = new QLabel("Граничное условие в a:", controlsWidget);
    bcb = new QLineEdit(this);
    bcb->setText("0");
    QLabel* label3 = new QLabel("Граничное условие в b:", controlsWidget);

    QPushButton *graphButton = new QPushButton("Численное решение", this);
    QPushButton *trueGraphButton = new QPushButton("Аналитическое решение", this);
    QPushButton *errorButton = new QPushButton("Погрешность решения", this);
    QPushButton *tableButton = new QPushButton("Таблица", this);
    QPushButton *removegraphButton = new QPushButton("Скрыть численное решение", this);
    QPushButton *removetrueGraphButton = new QPushButton("Скрыть аналитическое решение", this);
    QPushButton *removeerrorGraphButton = new QPushButton("Скрыть погрешность решения", this);
    QPushButton *summaryButton = new QPushButton("Справка", this);
    QRadioButton *taskVariant1 = new QRadioButton("Основная функция", this);
    QRadioButton *taskVariant2 = new QRadioButton("Периодическая функция", this);
    QRadioButton *funcVariant0 = new QRadioButton("|x³|+3x²", this);
    QRadioButton *funcVariant1 = new QRadioButton("√(x²-1)/x", this);
    QRadioButton *funcVariant2 = new QRadioButton("∛(1+x²)", this);
    QRadioButton *funcVariant3 = new QRadioButton("sin(x+1)/(x+1)", this);
    QRadioButton *graphVariant0 = new QRadioButton("График функции", this);
    QRadioButton *graphVariant1 = new QRadioButton("График производной", this);
    QRadioButton *graphVariant2 = new QRadioButton("График второй производной", this);

    QButtonGroup *taskbuttonGroup1 = new QButtonGroup(this);
    taskbuttonGroup1->addButton(taskVariant1);
    taskbuttonGroup1->addButton(taskVariant2);
    taskVariant1->setChecked(true);

    QButtonGroup *taskbuttonGroup2 = new QButtonGroup(this);
    taskbuttonGroup2->addButton(funcVariant0);
    taskbuttonGroup2->addButton(funcVariant1);
    taskbuttonGroup2->addButton(funcVariant2);
    taskbuttonGroup2->addButton(funcVariant3);
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

    connect(taskVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked1);
    connect(taskVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked1);
    connect(funcVariant0, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(funcVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(funcVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(funcVariant3, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked2);
    connect(graphVariant0, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    connect(graphVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    connect(graphVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked3);
    \
    controlLayout->addWidget(taskVariant1);
    controlLayout->addWidget(taskVariant2);
    controlLayout->addWidget(funcVariant0);
    controlLayout->addWidget(funcVariant1);
    controlLayout->addWidget(funcVariant2);
    controlLayout->addWidget(funcVariant3);
    controlLayout->addWidget(graphVariant0);
    controlLayout->addWidget(graphVariant1);
    controlLayout->addWidget(graphVariant2);
    controlLayout->addWidget(label1);
    controlLayout->addWidget(n);
    controlLayout->addWidget(label2);
    controlLayout->addWidget(bca);
    controlLayout->addWidget(label3);
    controlLayout->addWidget(bcb);
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

    sl = solver();
    Functions::init();
}

MainWindow::~MainWindow() {}

void MainWindow::onRadioButtonClicked1(){
  QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "Основная функция") {
        sl.valid = false;
        task_choice = 1;
        Functions::per = false;
    } else if (selectedButton->text() == "Периодическая функция") {
        sl.valid = false;
        Functions::per = true;
    }
}

void MainWindow::onRadioButtonClicked2(){
  QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "|x³|+3x²") {
        sl.valid = false;
        Functions::set_task(Functions::TEST);
    } else if (selectedButton->text() == "√(x²-1)/x") {
        sl.valid = false;
        Functions::set_task(Functions::FIRST);
    } else if (selectedButton->text() == "∛(1+x²)") {
        sl.valid = false;
        Functions::set_task(Functions::SECOND);
    } else if (selectedButton->text() == "sin(x+1)/(x+1)") {
        sl.valid = false;
        Functions::set_task(Functions::THIRD);
    }
}

void MainWindow::onRadioButtonClicked3(){
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

    if (!sl.valid || N != sl.n || borda != sl.prev_bord_a || bordb != sl.prev_bord_b)
      sl.solve(borda, bordb, N);

    QLineSeries* series = new QLineSeries();
    int k = 1000;
    int i;
    
    switch (graph_choice) {
    case 0:
      for (i = 0; i < k + 1; i++) {
        series->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), sl.S(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      series->setName("Численная функция, n = " + QString::number(N));
      break;
    case 1:
      for (i = 0; i < k + 1; i++) {
        series->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), sl.S_der(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      series->setName("Производная численной функции, n = " + QString::number(N));
      break;
    case 2:
      for (i = 0; i < k + 1; i++) {
        series->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), sl.S_der_der(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      series->setName("Вторая производная численной функции, n = " + QString::number(N));
      break;
    }
    seriesStack.push(series);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::showTrueGraph(){
    int N = n->text().toInt();

    QLineSeries* seriesve = new QLineSeries();
    int k = 1000;
    int i;

    switch (graph_choice) {
    case 0:
      for (i = 0; i < k + 1; i++) {
        seriesve->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), Functions::f(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      seriesve->setName("Истинная функция, n = " + QString::number(N));
      break;
    case 1:
      for (i = 0; i < k + 1; i++) {
        seriesve->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), Functions::f_der(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      seriesve->setName("Производная истинной функции, n = " + QString::number(N));
      break;
    case 2:
      for (i = 0; i < k + 1; i++) {
        seriesve->append(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i), Functions::f_der_der(Functions::a + (Functions::b - Functions::a) / (double)(k) * (double)(i)));
      }
      seriesve->setName("Вторая производная истинной функции, n = " + QString::number(N));
      break;
    }
    solutionStack.push(seriesve);
    chart->addSeries(seriesve);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::showErrorGraph(){
    int N = n->text().toInt();
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();

    if (!sl.valid || N != sl.n || borda != sl.prev_bord_a || bordb != sl.prev_bord_b)
      sl.solve(borda, bordb, N);

    QLineSeries* pseries = new QLineSeries();
    int k = 1000;
    int i;

    switch (graph_choice) {
    case 0:
      for (i = 0; i < k + 1; i++) {
        pseries->append(Functions::a + (Functions::b - Functions::a) / k * i, (Functions::f(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S(Functions::a + (Functions::b - Functions::a) / k * i)));
      }
      pseries->setName("Погр., n = " + QString::number(N));
      break;
    case 1:
      for (i = 0; i < k + 1; i++) {
        pseries->append(Functions::a + (Functions::b - Functions::a) / k * i, (Functions::f_der(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S_der(Functions::a + (Functions::b - Functions::a) / k * i)));
      }
      pseries->setName("Погр., n = " + QString::number(N));
      break;
    case 2:
      for (i = 0; i < k + 1; i++) {
        pseries->append(Functions::a + (Functions::b - Functions::a) / k * i, (Functions::f_der_der(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S_der_der(Functions::a + (Functions::b - Functions::a) / k * i)));
      }
      pseries->setName("Погр., n = " + QString::number(N));
      break;
    }
    errorStack.push(pseries);
    chart->addSeries(pseries);
    chart->createDefaultAxes();
    chart->zoomReset();
}

void MainWindow::removeGraph(){
    if(!seriesStack.empty()){
        chart->removeSeries(seriesStack.top());
        seriesStack.pop();
    }
}

void MainWindow::removeTrueGraph(){
    if(!solutionStack.empty()){
        chart->removeSeries(solutionStack.top());
        solutionStack.pop();
    }
}

void MainWindow::removeErrorGraph(){
    if(!errorStack.empty()){
        chart->removeSeries(errorStack.top());
        errorStack.pop();
    }
}

void MainWindow::showTable(){
    int N = n->text().toInt();
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();
    QTabWidget *tabWidget = new QTabWidget;
    QTableWidget *tableWidget1 = new QTableWidget((2 * N + 1), 10);
    tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget1->setHorizontalHeaderLabels({"X", "F(x)", "S(x)", "F(x) - S(x)", "F'(x)", "S'(x)", "F'(x) - S'(x)", "F''(x)", "S''(x)", "F''(x) - S''(x)"});

    if (!sl.valid || N != sl.n || borda != sl.prev_bord_a || bordb != sl.prev_bord_b)
      sl.solve(borda, bordb, N);

    int k = 2 * N;
    for (int i = 0; i <= k; i++) {
        tableWidget1->setItem(i, 0, new QTableWidgetItem(QString::number(Functions::a + (Functions::b - Functions::a) / k * i)));
        tableWidget1->setItem(i, 1, new QTableWidgetItem(QString::number(Functions::f(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 2, new QTableWidgetItem(QString::number(sl.S(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 3, new QTableWidgetItem(QString::number(Functions::f(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 4, new QTableWidgetItem(QString::number(Functions::f_der(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 5, new QTableWidgetItem(QString::number(sl.S_der(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 6, new QTableWidgetItem(QString::number(Functions::f_der(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S_der(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 7, new QTableWidgetItem(QString::number(Functions::f_der_der(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 8, new QTableWidgetItem(QString::number(sl.S_der_der(Functions::a + (Functions::b - Functions::a) / k * i))));
        tableWidget1->setItem(i, 9, new QTableWidgetItem(QString::number(Functions::f_der_der(Functions::a + (Functions::b - Functions::a) / k * i) - sl.S_der_der(Functions::a + (Functions::b - Functions::a) / k * i))));
    }
    tabWidget->addTab(tableWidget1, "Таблица проверки");
    QTableWidget *tableWidget2 = new QTableWidget((N), 6);
    tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget2->setHorizontalHeaderLabels({"xᵢ₋₁", "xᵢ", "aᵢ", "bᵢ", "cᵢ", "dᵢ"});
    for (int i = 0; i < N; i++) {
        tableWidget2->setItem(i, 0, new QTableWidgetItem(QString::number(Functions::a + (Functions::b - Functions::a) / N * (i))));
        tableWidget2->setItem(i, 1, new QTableWidgetItem(QString::number(Functions::a + (Functions::b - Functions::a) / N * (i + 1))));
        tableWidget2->setItem(i, 2, new QTableWidgetItem(QString::number(sl.a[i + 1])));
        tableWidget2->setItem(i, 3, new QTableWidgetItem(QString::number(sl.b[i + 1])));
        tableWidget2->setItem(i, 4, new QTableWidgetItem(QString::number(sl.c[i + 1])));
        tableWidget2->setItem(i, 5, new QTableWidgetItem(QString::number(sl.d[i + 1])));
    }
    tabWidget->addTab(tableWidget2, "Таблица коэффициентов");
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
    double borda = bca->text().toDouble();
    double bordb = bcb->text().toDouble();

    if (!sl.valid || N != sl.n || borda != sl.prev_bord_a || bordb != sl.prev_bord_b)
      sl.solve(borda, bordb, N);

    int k = 2 * N + 1;
    double max_sub = 0;
    double max_der_sub = 0;
    double max_der_der_sub = 0;
    double max_sub_x = 0;
    double max_der_sub_x = 0;
    double max_der_der_sub_x = 0;
    double h = (Functions::b - Functions::a) / k;
    double x = Functions::a;
    for (int i = 1; i <= k; i++) {
      x += h;
      if (abs(Functions::f(x) - sl.S(x)) > max_sub) {
        max_sub = abs(Functions::f(x) - sl.S(x));
        max_sub_x = x;
      }
      if (abs(Functions::f_der(x) - sl.S_der(x)) > max_der_sub) {
        max_der_sub = abs(Functions::f_der(x) - sl.S_der(x));
        max_der_sub_x = x;
      }
      if (abs(Functions::f_der_der(x) - sl.S_der_der(x)) > max_der_der_sub) {
        max_der_der_sub = abs(Functions::f_der_der(x) - sl.S_der_der(x));
        max_der_der_sub_x = x;
      }
    }

    summary = new QLabel("Справка:", summaryBox);
    summary->setAlignment(Qt::AlignCenter);
    summary->setWordWrap(true);
    summary->setTextFormat(Qt::RichText);
    summary->setText("<center>"
                     "<h3>"
                     "<p>Сетка сплайна: n = " + QString::number(N) + "</p><p>Контрольная сетка: N = " + QString::number(2 * N) +
                     "</p><p>Погрешность сплайна на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F(x<sub>j</sub>)-S(x<sub>j</sub>)| = " + QString::number(max_sub) + " при x = " + QString::number(max_sub_x) +
                     "</p><p>Погрешность производной на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F'(x<sub>j</sub>)-S'(x<sub>j</sub>)| = " + QString::number(max_der_sub) + " при x = " + QString::number(max_der_sub_x) +
                     "</p><p>Погрешность второй производной на контрольной сетке</p>" +
                     "<p>max<sub>j=0..N</sub> |F''(x<sub>j</sub>)-S''(x<sub>j</sub>)| = " + QString::number(max_der_der_sub) + " при x = " + QString::number(max_der_der_sub_x) +
                     "</p></h3>"
                     "</center>");
    summaryBox->show();
}

