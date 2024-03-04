#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    view = new Q3DSurface;
    view->activeTheme()->setType(Q3DTheme::ThemeIsabelle);
    view->activeTheme()->setLabelBorderEnabled(false);
    view->setShadowQuality(Q3DSurface::ShadowQualityNone);
    view->activeTheme()->setLabelBackgroundEnabled(true);
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.3));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *controlsWidget = new QWidget();
    controlsWidget->setMaximumWidth(210);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlsWidget);

    xn = new QLineEdit(this);
    xn->setText("3");
    QLabel* label1 = new QLabel("Разбиений по x:", controlsWidget);
    yn = new QLineEdit(this);
    yn->setText("3");
    QLabel* label2 = new QLabel("Разбиений по y:", controlsWidget);
    eps = new QLineEdit(this);
    eps->setText("0.001");
    QLabel* label3 = new QLabel("Контроль точности ε:", controlsWidget);
    maxsteps = new QLineEdit(this);
    maxsteps->setText("10000");
    QLabel* label4 = new QLabel("Макс. число шагов:", controlsWidget);
    w = new QLineEdit(this);
    w->setText("1.5");
    QLabel* label6 = new QLabel("Параметр ω:", controlsWidget);
    label5 = new QLabel("", controlsWidget);

    QPushButton *graphButton = new QPushButton("Численное решение", this);
    QPushButton *trueGraphButton = new QPushButton("Аналитическое решение", this);
    QPushButton *tableButton = new QPushButton("Таблица", this);
    QPushButton *removegraphButton = new QPushButton("Скрыть численное решение", this);
    QPushButton *removetrueGraphButton = new QPushButton("Скрыть аналитическое решение", this);
    QRadioButton *taskVariant1 = new QRadioButton("Зейдель", this);
    QRadioButton *taskVariant2 = new QRadioButton("МВР", this);
    taskVariant1->setChecked(true);

    connect(graphButton, &QPushButton::clicked, this, &MainWindow::showGraph);
    connect(trueGraphButton, &QPushButton::clicked, this, &MainWindow::showTrueGraph);
    connect(removegraphButton, &QPushButton::clicked, this, &MainWindow::removeGraph);
    connect(removetrueGraphButton, &QPushButton::clicked, this, &MainWindow::removeTrueGraph);
    connect(tableButton, &QPushButton::clicked, this, &MainWindow::showTable);
    connect(taskVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(taskVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(10);
    slider->setValue(10);
    valueLabel = new QLabel("Снимок: 10");

    connect(slider, &QSlider::valueChanged, this, [=](int value) {
        valueLabel->setText("Снимок: " + QString::number(value));
        setT(value);
    });

    controlLayout->addWidget(taskVariant1);
    controlLayout->addWidget(taskVariant2);
    controlLayout->addWidget(label1);
    controlLayout->addWidget(xn);
    controlLayout->addWidget(label2);
    controlLayout->addWidget(yn);
    controlLayout->addWidget(label3);
    controlLayout->addWidget(eps);
    controlLayout->addWidget(label4);
    controlLayout->addWidget(maxsteps);
    controlLayout->addWidget(label6);
    controlLayout->addWidget(w);
    controlLayout->addWidget(graphButton);
    controlLayout->addWidget(trueGraphButton);
    controlLayout->addWidget(tableButton);
    controlLayout->addWidget(removegraphButton);
    controlLayout->addWidget(removetrueGraphButton);
    controlLayout->addWidget(label5);
    controlLayout->addWidget(valueLabel);
    controlLayout->addWidget(slider);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(controlsWidget);
    mainLayout->addWidget(container);

    view->axisX()->setTitle(QStringLiteral("X"));
    view->axisY()->setTitle(QStringLiteral("U"));
    view->axisZ()->setTitle(QStringLiteral("Y"));
    view->axisX()->setTitleVisible(true);
    view->axisY()->setTitleVisible(true);
    view->axisZ()->setTitleVisible(true);
    slv = solver();
    a = 0;
    b = 1;
    c = 0;
    d = 1;
};

MainWindow::~MainWindow()
{
}


void MainWindow::showTrueGraph(){
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    double h = abs(a - b) / Xn;
    double k = abs(c - d) / Yn;
    double (*true_u)(double, double);
    true_u = u_real::u;
    if (view->hasSeries(dataTrueSeries)) view->removeSeries(dataTrueSeries);

    dataTrueSeries = new QSurface3DSeries;
    dataTrueSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    for (int j = 0; j <= Yn; j++) {
        QSurfaceDataRow* row = new QSurfaceDataRow;
        for (int i = 0; i <= Xn; i++) {
            row->append(QSurfaceDataItem(QVector3D(a + h * i, true_u(a + h * i, c + k * j), c + k * j)));
        }
        dataTrueSeries->dataProxy()->addRow(row);
    }
    dataTrueSeries->setItemLabelFormat("True solution @xLabel @yLabel @zLabel");
    //dataTrueSeries->setBaseColor(QColor(255, 0, 0, 255));
    //dataTrueSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    dataTrueSeries->setTextureFile("iceberg.jpg");
    view->addSeries(dataTrueSeries);
    view->axisX()->setRange(a, b);
    view->axisZ()->setRange(c, d);
    view->axisY()->setRange(-1, 1);
}

void MainWindow::showGraph() {
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    int maxN = maxsteps->text().toInt();
    double ww = w->text().toDouble();
    double epsilon = eps->text().toDouble();
    if (Xn != slv.N || Yn != slv.M || maxN != slv.max_it || epsilon != slv.epsilon || selectedTask != slv.task || slv.w != ww) {
        slv.task = selectedTask;
        slv.w = ww;
        slv.solve(Xn, Yn, a, b, c, d, epsilon, maxN, v, z);
    }
    else if (!view->hasSeries(dataSeries)) {
    }
    else return;
    if (view->hasSeries(dataSeries)) view->removeSeries(dataSeries);

    dataSeries = new QSurface3DSeries;
    dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    for (int j = 0; j <= slv.M; j++) {
        QSurfaceDataRow* row = new QSurfaceDataRow;
        for (int i = 0; i <= slv.N; i++) {
            row->append(QSurfaceDataItem(QVector3D(a + slv.h * i, v[9][i][j], c + slv.k * j)));
        }
        dataSeries->dataProxy()->addRow(row);
    }
    dataSeries->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
    dataSeries->setColorStyle(Q3DTheme::ColorStyleObjectGradient);
    //dataSeries->setTextureFile("iceberg.jpg");
    dataSeries->setBaseColor(QColor(0, 0, 255, 255));
    view->addSeries(dataSeries);
    view->axisX()->setRange(a, b);
    view->axisZ()->setRange(c, d);
    view->axisY()->setRange(-1, 1);

    label5->setText("Норма общей погрешности\nравна " + QString::number(slv.max_z) + ", достигнута\nза " + QString::number(slv.it) + " итераций\n" +
                    "Точность на выходе: " + QString::number(slv.achieved_accuracy) +
                    "\nНорма невязки: " + QString::number(slv.max_r));
    for(size_t i = 0; i < 10; i++){
        iterations[i] = new QSurface3DSeries;
    }
    for(size_t t = 0; t < 10; t++){
        for (int j = 0; j <= slv.M; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= slv.N; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + slv.h * i, v[t][i][j], c + slv.k * j)));
            }
            iterations[t]->dataProxy()->addRow(row);
        }
        iterations[t]->setItemLabelFormat(QStringLiteral("(@xLabel @yLabel @zLabel)"));
        iterations[t]->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        iterations[t]->setFlatShadingEnabled(false);
        iterations[t]->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
        iterations[t]->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    }
    slider->setValue(10);
    valueLabel->setText("Снимок: 10");
}

void MainWindow::removeGraph(){
    if (view->hasSeries(dataSeries)) view->removeSeries(dataSeries);
    for (auto it : view->seriesList()) {
        if (it != dataTrueSeries) view->removeSeries(it);
    }
}

void MainWindow::removeTrueGraph(){
    if (view->hasSeries(dataTrueSeries)) view->removeSeries(dataTrueSeries);
}

void MainWindow::onRadioButtonClicked(){
    QRadioButton *selectedButton = qobject_cast<QRadioButton*>(sender());
    if (selectedButton->text() == "Зейдель") {
        selectedTask = 0;
    } else if (selectedButton->text() == "МВР") {
        selectedTask = 1;
    }
}

void MainWindow::setT(int t){
    if (slv.valid == 0) return;
    for (auto it : view->seriesList()) {
        if (it != dataTrueSeries) view->removeSeries(it);
    }
    view->addSeries(iterations[t]);
}

void MainWindow::showTable() {
    if (!view->hasSeries(dataSeries)) return;
    QTabWidget *tabWidget = new QTabWidget;
    QTableWidget *tableWidget1 = new QTableWidget((slv.N + 1) * (slv.M + 1), 4);
    tableWidget1->setHorizontalHeaderLabels({"X", "Y", "V", "V - U"});
    for (int i = 0; i <= slv.N; i++) {
        for (int j = 0; j <= slv.M; j++) {
            tableWidget1->setItem(i * (slv.M + 1) + j, 0, new QTableWidgetItem(QString::number(slv.h * i)));
            tableWidget1->setItem(i * (slv.M + 1) + j, 1, new QTableWidgetItem(QString::number(slv.k * j)));
            tableWidget1->setItem(i * (slv.M + 1) + j, 2, new QTableWidgetItem(QString::number(v[0][i][j])));
            tableWidget1->setItem(i * (slv.M + 1) + j, 3, new QTableWidgetItem(QString::number(z[0][i][j])));
        }
    }
    tabWidget->addTab(tableWidget1, "Шаг 1");
    QTableWidget *tableWidget2 = new QTableWidget((slv.N + 1) * (slv.M + 1), 4);
    tableWidget2->setHorizontalHeaderLabels({"X", "Y", "V", "V - U"});
    for (int i = 0; i <= slv.N; i++) {
        for (int j = 0; j <= slv.M; j++) {
            tableWidget2->setItem(i * (slv.M + 1) + j, 0, new QTableWidgetItem(QString::number(slv.h * i)));
            tableWidget2->setItem(i * (slv.M + 1) + j, 1, new QTableWidgetItem(QString::number(slv.k * j)));
            tableWidget2->setItem(i * (slv.M + 1) + j, 2, new QTableWidgetItem(QString::number(v[1][i][j])));
            tableWidget2->setItem(i * (slv.M + 1) + j, 3, new QTableWidgetItem(QString::number(z[1][i][j])));
        }
    }
    tabWidget->addTab(tableWidget2, "Шаг 2");
    QTableWidget *tableWidget3 = new QTableWidget((slv.N + 1) * (slv.M + 1), 4);
    tableWidget3->setHorizontalHeaderLabels({"X", "Y", "V", "V - U"});
    for (int i = 0; i <= slv.N; i++) {
        for (int j = 0; j <= slv.M; j++) {
            tableWidget3->setItem(i * (slv.M + 1) + j, 0, new QTableWidgetItem(QString::number(slv.h * i)));
            tableWidget3->setItem(i * (slv.M + 1) + j, 1, new QTableWidgetItem(QString::number(slv.k * j)));
            tableWidget3->setItem(i * (slv.M + 1) + j, 2, new QTableWidgetItem(QString::number(v[9][i][j])));
            tableWidget3->setItem(i * (slv.M + 1) + j, 3, new QTableWidgetItem(QString::number(z[9][i][j])));
        }
    }
    tabWidget->addTab(tableWidget3, "Последний шаг ");
    tabWidget->setMinimumSize(400, 400);
    tabWidget->setWindowTitle("Таблица");
    tabWidget->show();
}
