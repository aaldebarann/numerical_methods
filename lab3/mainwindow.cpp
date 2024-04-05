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
    eps2 = new QLineEdit(this);
    eps2->setText("0.001");
    QLabel* label5 = new QLabel("Интервал для таблицы:", controlsWidget);
    intervals = new QLineEdit(this);
    intervals->setText("1000");
    QLabel* label7 = new QLabel("Контроль точности ε2:", controlsWidget);
    maxsteps2 = new QLineEdit(this);
    maxsteps2->setText("10000");
    QLabel* label8 = new QLabel("Макс. число шагов N2:", controlsWidget);
    w2 = new QLineEdit(this);
    w2->setText("1.5");
    QLabel* label9 = new QLabel("Параметр ω2:", controlsWidget);

    QPushButton *graphButton = new QPushButton("Численное решение", this);
    QPushButton *trueGraphButton = new QPushButton("Аналитическое решение", this);
    QPushButton *pGraphButton = new QPushButton("Разность решений", this);
    QPushButton *tableButton = new QPushButton("Таблица", this);
    QPushButton *summaryButton = new QPushButton("Справка", this);
    QPushButton *removegraphButton = new QPushButton("Скрыть численное решение", this);
    QPushButton *removetrueGraphButton = new QPushButton("Скрыть аналитическое решение", this);
    QPushButton *removepGraphButton = new QPushButton("Скрыть разность решений", this);
    QRadioButton *methVariant1 = new QRadioButton("Зейдель", this);
    QRadioButton *methVariant2 = new QRadioButton("МВР", this);
    QRadioButton *taskVariant1 = new QRadioButton("Тестовая", this);
    QRadioButton *taskVariant2 = new QRadioButton("Основная", this);

    QButtonGroup *taskbuttonGroup1 = new QButtonGroup(this);
    taskbuttonGroup1->addButton(taskVariant1);
    taskbuttonGroup1->addButton(taskVariant2);
    taskVariant1->setChecked(true);

    QButtonGroup *taskbuttonGroup2 = new QButtonGroup(this);
    taskbuttonGroup2->addButton(methVariant1);
    taskbuttonGroup2->addButton(methVariant2);
    methVariant1->setChecked(true);

    connect(graphButton, &QPushButton::clicked, this, &MainWindow::showGraph);
    connect(trueGraphButton, &QPushButton::clicked, this, &MainWindow::showTrueGraph);
    connect(pGraphButton, &QPushButton::clicked, this, &MainWindow::showPGraph);
    connect(removegraphButton, &QPushButton::clicked, this, &MainWindow::removeGraph);
    connect(removetrueGraphButton, &QPushButton::clicked, this, &MainWindow::removeTrueGraph);
    connect(removepGraphButton, &QPushButton::clicked, this, &MainWindow::removePGraph);
    connect(tableButton, &QPushButton::clicked, this, &MainWindow::showTable);
    connect(summaryButton, &QPushButton::clicked, this, &MainWindow::showSummary);
    connect(taskVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(taskVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(methVariant1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(methVariant2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(10);
    slider->setValue(10);
    valueLabel = new QLabel("Снимок: 10");

    connect(slider, &QSlider::valueChanged, this, [=](int value) {
        valueLabel->setText("Снимок: " + QString::number(value));
        setT(value - 1);
    });

    controlLayout->addWidget(taskVariant1);
    controlLayout->addWidget(taskVariant2);
    controlLayout->addWidget(methVariant1);
    controlLayout->addWidget(methVariant2);
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
    controlLayout->addWidget(label5);
    controlLayout->addWidget(intervals);
    controlLayout->addWidget(label7);
    controlLayout->addWidget(eps2);
    controlLayout->addWidget(label8);
    controlLayout->addWidget(maxsteps2);
    controlLayout->addWidget(label9);
    controlLayout->addWidget(w2);
    controlLayout->addWidget(graphButton);
    controlLayout->addWidget(trueGraphButton);
    controlLayout->addWidget(pGraphButton);
    controlLayout->addWidget(tableButton);
    controlLayout->addWidget(summaryButton);
    controlLayout->addWidget(removegraphButton);
    controlLayout->addWidget(removetrueGraphButton);
    controlLayout->addWidget(removepGraphButton);
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
    type_d h = abs(a - b) / Xn;
    type_d k = abs(c - d) / Yn;
    type_d (*true_u)(type_d, type_d);
    lastPlotButton = true;
    int skipx = 1;
    int skipy = 1;
    if (selectedTask == Functions::test){
        true_u = u_test::u;
        if (view->hasSeries(dataTrueSeries)) view->removeSeries(dataTrueSeries);
        dataTrueSeries = new QSurface3DSeries;
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        if (Xn < 100 && Yn < 100) dataTrueSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataTrueSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + h * i * skipx, true_u(a + h * i * skipx, c + k * j * skipy), c + k * j * skipy)));
            }
            dataTrueSeries->dataProxy()->addRow(row);
        }
        dataTrueSeries->setItemLabelFormat("True solution @xLabel @yLabel @zLabel");
    } else {
        if (view->hasSeries(dataTrueSeries)) view->removeSeries(dataTrueSeries);
        dataTrueSeries = new QSurface3DSeries;
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        if (Xn < 100 && Yn < 100) dataTrueSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataTrueSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= 2 * Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= 2 * Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + (h / 2) * i * skipx, v2[9][i * skipx][j * skipy], c + (k / 2) * j * skipy)));
            }
            dataTrueSeries->dataProxy()->addRow(row);
        }
        dataTrueSeries->setItemLabelFormat("Solution 2N * 2M @xLabel @yLabel @zLabel");
    }
    //dataTrueSeries->setBaseColor(QColor(255, 0, 0, 255));
    //dataTrueSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    dataTrueSeries->setTextureFile("iceberg.jpg");
    dataTrueSeries->setWireframeColor(QColor::fromString("#ff00ff"));
    view->addSeries(dataTrueSeries);
    view->axisX()->setRange(a, b);
    view->axisZ()->setRange(c, d);
    view->axisY()->setRange(-1, 1);
}

void MainWindow::showGraph() {
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    int maxN = maxsteps->text().toInt();
    int maxN2 = maxsteps2->text().toInt();
    int interval = intervals->text().toInt();
    int skipx = 1;
    int skipy = 1;
    type_d ww = w->text().toDouble();
    type_d epsilon = eps->text().toDouble();
    type_d ww2 = w2->text().toDouble();
    type_d epsilon2 = eps2->text().toDouble();
    lastPlotButton = false;
    if (selectedTask == Functions::test){
        if (Xn != slv.N || Yn != slv.M || maxN != slv.max_it || epsilon != slv.epsilon || selectedTask != slv.task || slv.w != ww || selectedMeth != slv.meth || slv.interval != interval) {
            slv.task = selectedTask;
            slv.meth = selectedMeth;
            slv.interval = interval;
            slv.w = ww;
            slv.solve(Xn, Yn, a, b, c, d, epsilon, maxN, v, z);
        }
        else if (!view->hasSeries(dataSeries)) {
        }
        else return;
        if (view->hasSeries(dataSeries)) view->removeSeries(dataSeries);
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        dataSeries = new QSurface3DSeries;
        if (Xn < 100 && Yn < 100) dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + slv.h * i * skipx, v[9][i * skipx][j * skipy], c + slv.k * j * skipy)));
            }
            dataSeries->dataProxy()->addRow(row);
        }
        dataSeries->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
        dataSeries->setColorStyle(Q3DTheme::ColorStyleObjectGradient);
        dataSeries->setWireframeColor(QColor::fromString("#daa520"));
        //dataSeries->setTextureFile("iceberg.jpg");
        dataSeries->setBaseColor(QColor(0, 0, 255, 255));
        view->addSeries(dataSeries);
        view->axisX()->setRange(a, b);
        view->axisZ()->setRange(c, d);
        view->axisY()->setRange(-1, 1);

        if (Xn < 100 && Yn < 100) {
            slider->setEnabled(true);
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
                iterations[t]->setItemLabelFormat("Solution @xLabel @yLabel @zLabel");
                iterations[t]->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
            }
        } else {
            slider->setDisabled(true);
        }
        for (int j = 0; j <= slv.M; j++) {
            for (int i = 0; i <= slv.N; i++) {
                if(abs(v[9][i][j] - u_test::u(a + slv.h * i, c + slv.k * j)) >= slv.max_z){
                    slv.max_z = abs(v[9][i][j] - u_test::u(a + slv.h * i, c + slv.k * j));
                    slv.max_x = a + slv.h * i;
                    slv.max_y = c + slv.k * j;
                }
            }
        }
    }
    if (selectedTask == Functions::tmain){
        if (Xn != slv.N || Yn != slv.M || maxN != slv.max_it || epsilon != slv.epsilon || selectedTask != slv.task || slv.w != ww || epsilon2 != slv2.epsilon || slv2.w != ww2 || maxN2 != slv2.max_it || selectedMeth != slv.meth || slv.interval != interval) {
            slv.task = selectedTask;
            slv2.task = selectedTask;
            slv.meth = selectedMeth;
            slv2.meth = selectedMeth;
            slv.interval = interval;
            slv2.interval = interval;
            slv.w = ww;
            slv2.w = ww2;
            slv.solve(Xn, Yn, a, b, c, d, epsilon, maxN, v);
            slv2.solve(2 * Xn, 2 * Yn, a, b, c, d, epsilon2, maxN2, v2);
        }
        else if (!view->hasSeries(dataSeries)) {
        }
        else return;
        if (view->hasSeries(dataSeries)) view->removeSeries(dataSeries);

        dataSeries = new QSurface3DSeries;
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        if (Xn < 100 && Yn < 100) dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + slv.h * i * skipx, v[9][i * skipx][j * skipy], c + slv.k * j * skipy)));
            }
            dataSeries->dataProxy()->addRow(row);
        }
        dataSeries->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
        dataSeries->setColorStyle(Q3DTheme::ColorStyleObjectGradient);
        dataSeries->setWireframeColor(QColor::fromString("#daa520"));
        //dataSeries->setTextureFile("iceberg.jpg");
        dataSeries->setBaseColor(QColor(0, 0, 255, 255));
        view->addSeries(dataSeries);
        view->axisX()->setRange(a, b);
        view->axisZ()->setRange(c, d);
        view->axisY()->setRange(-1, 1);
        if (Xn < 100 && Yn < 100) {
            slider->setEnabled(true);
            for(size_t i = 0; i < 10; i++){
                iterations[i] = new QSurface3DSeries;
                iterations2[i] = new QSurface3DSeries;
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
                iterations[t]->setItemLabelFormat("Solution @xLabel @yLabel @zLabel");
                iterations[t]->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
                iterations[t]->setWireframeColor(QColor::fromString("#daa520"));
            }
            for(size_t t = 0; t < 10; t++){
                for (int j = 0; j <= slv2.M; j++) {
                    QSurfaceDataRow* row = new QSurfaceDataRow;
                    for (int i = 0; i <= slv2.N; i++) {
                        row->append(QSurfaceDataItem(QVector3D(a + slv2.h * i, v2[t][i][j], c + slv2.k * j)));
                    }
                    iterations2[t]->dataProxy()->addRow(row);
                }
                iterations2[t]->setItemLabelFormat(QStringLiteral("(@xLabel @yLabel @zLabel)"));
                iterations2[t]->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
                iterations2[t]->setFlatShadingEnabled(false);
                iterations2[t]->setItemLabelFormat("Solution 2 @xLabel @yLabel @zLabel");
                iterations2[t]->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
                iterations2[t]->setWireframeColor(QColor::fromString("#ff00ff"));
            }
        } else {
            slider->setDisabled(true);
        }
        for (int j = 0; j <= slv.M; j++) {
            for (int i = 0; i <= slv.N; i++) {
                if(abs(v[9][i][j] - v2[9][2 * i][2 * j]) > slv.max_z){
                    slv.max_z = abs(v[9][i][j] - v2[9][2 * i][2 * j]);
                    slv.max_x = a + slv.h * i;
                    slv.max_y = c + slv.k * j;
                }
            }
        }
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
        selectedMeth = Methods::zeidel;
    } else if (selectedButton->text() == "МВР") {
        selectedMeth = Methods::mvr;
    } else if (selectedButton->text() == "Тестовая") {
        selectedTask = Functions::test;
        slv = solver(Functions::test);
    } else if (selectedButton->text() == "Основная") {
        selectedTask = Functions::tmain;
        slv = solver(Functions::tmain);
        slv2 = solver(Functions::tmain);
    }
}

void MainWindow::setT(int t){
    if (slv.valid == 0) return;
    for (auto it : view->seriesList()) {
        if (it != dataTrueSeries) view->removeSeries(it);
    }
    if (!lastPlotButton) {view->addSeries(iterations[t]);}
    else {view->addSeries(iterations2[t]);}
}

void MainWindow::showTable() {
    if (!view->hasSeries(dataSeries)) return;
    int skipx = 1;
    int skipy = 1;
    int Xn = slv.N;
    int Yn = slv.M;
    if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
    QTabWidget *tabWidget = new QTabWidget;
    QTableWidget *tableWidget1 = new QTableWidget((Yn + 1), (Xn + 1));
    QTableWidget *tableWidget2 = new QTableWidget((Yn + 1), (Xn + 1));
    QTableWidget *tableWidget3 = new QTableWidget((Yn + 1), (Xn + 1));
    QStringList headerlistx;
    QStringList headerlisty;
    for (int i = 0; i <= Xn; i++) {
        headerlistx << "X" + QString::number(i);
    }
    for (int i = 0; i <= Yn; i++) {
        headerlisty << "Y" + QString::number(i);
    }
    tableWidget1->setHorizontalHeaderLabels(headerlistx);
    tableWidget1->setVerticalHeaderLabels(headerlisty);
    tableWidget2->setHorizontalHeaderLabels(headerlistx);
    tableWidget2->setVerticalHeaderLabels(headerlisty);
    tableWidget3->setHorizontalHeaderLabels(headerlistx);
    tableWidget3->setVerticalHeaderLabels(headerlisty);
    tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(slv.task == Functions::test) {
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget1->setItem(j, i, new QTableWidgetItem(QString::number(u_test::u(a + slv.h * i * skipx, c + slv.k * j * skipy))));
            }
        }
        tabWidget->addTab(tableWidget1, "Значения точного решения");
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget2->setItem(j, i, new QTableWidgetItem(QString::number(v[9][i * skipx][j * skipy])));
            }
        }
        tabWidget->addTab(tableWidget2, "Значения численного решения");
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget3->setItem(j, i, new QTableWidgetItem(QString::number(z[9][i * skipx][j * skipy])));
            }
        }
        tabWidget->addTab(tableWidget3, "Значения погрешности");
    } else {
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget1->setItem(j, i, new QTableWidgetItem(QString::number(v[9][i * skipx][j * skipy])));
            }
        }
        tabWidget->addTab(tableWidget1, "Значения численного решения v");
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget2->setItem(j, i, new QTableWidgetItem(QString::number(v2[9][2 * i * skipx][2 * j * skipy])));
            }
        }
        tabWidget->addTab(tableWidget2, "Значения численного решения v2");
        for (int i = 0; i <= Xn; i++) {
            for (int j = 0; j <= Yn; j++) {
                tableWidget3->setItem(j, i, new QTableWidgetItem(QString::number(abs(v[9][i * skipx][j * skipy] - v2[9][2 * i * skipx][2 * j * skipy]))));
            }
        }
        tabWidget->addTab(tableWidget3, "Значения разности v - v2");
    }
    tabWidget->setMinimumSize(400, 400);
    tabWidget->setWindowTitle("Таблица");
    tabWidget->show();
}

void MainWindow::showSummary(){
    QDialog *summaryBox = new QDialog();
    QIcon icon = QIcon("icon.ico");
    summaryBox->setWindowIcon(icon);
    summaryBox->setWindowTitle("Справка ☭");
    summaryBox->setMinimumSize(700, 600);
    summary = new QLabel("Справка:", this);
    summary->setAlignment(Qt::AlignCenter);
    summary->setWordWrap(true);
    summary->setTextFormat(Qt::RichText);
    QTableWidget *tableWidget4 = new QTableWidget(this);
    if(slv.task == Functions::test){
        summary->setText("<center>"
                         "<h3>"
                         "<p> Для решения тестовой задачи использованы сетка с числом разбиений по x N = " + QString::number(slv.N) + ", и числом разбиений по y m = " + QString::number(slv.M) +
                         ", метод верхней релаксации с параметром ω = " + QString::number(slv.w) + ", применены критерии" +
                         " остановки по точности ε<sub>мет</sub> = " + QString::number(slv.epsilon) + " и по числу итераций N<sub>max</sub> = " + QString::number(slv.max_it) +
                         "</p><p> На решение схемы (СЛАУ) затрачено итераций N = " + QString::number(slv.it) +
                         " и достигнута точность итерационного метода ε<sup>(N)</sup> = " + QString::number(slv.achieved_accuracy) +
                         "</p><p> Схема (СЛАУ) решена с невязкой || R<sup>(N)</sup>|| = " + QString::number(slv.max_r) +
                         ", для невязки СЛАУ использована евклидова норма" +
                         "</p><p> Тестовая задача должна быть решена с погрешностью не более ε = 0.5⋅10<sup>-6</sup>" +
                         "</p><p> Задача решена с погрешностью ε<sub>1</sub> = " + QString::number(slv.max_z) +
                         "</p><p> Максимальное отклонение точного и численного решений наблюдается в узле x = " + QString::number(slv.max_x) + ", y = " + QString::number(slv.max_y) +
                         "</p><p> В качестве начального приближения использована линейная интерполяция по x" +
                         "</p></h3>"
                         "</center>");
        QStringList headerlist2{"It", "Eps", "||R||", "||Z||"};
        tableWidget4->setColumnCount(slv.iter.size() + 1);
        tableWidget4->setRowCount(4);
        tableWidget4->setVerticalHeaderLabels(headerlist2);
        for (int i = 0; i <= slv.iter.size(); i++) {
            tableWidget4->setItem(0, i, new QTableWidgetItem(QString::number((slv.iter[i]))));
            tableWidget4->setItem(1, i, new QTableWidgetItem(QString::number((slv.ACCURACY[i]))));
            tableWidget4->setItem(2, i, new QTableWidgetItem(QString::number((slv.MAX_R[i]))));
            tableWidget4->setItem(3, i, new QTableWidgetItem(QString::number((slv.MAX_Z[i]))));
        }
    } else if(slv.task == Functions::tmain){
        summary->setText("<center>"
                         "<h3>"
                         "<p> Для решения основной задачи использованы сетка с числом разбиений по x N = " + QString::number(slv.N) + ", и числом разбиений по y m = " + QString::number(slv.M) +
                         ", метод верхней релаксации с параметром ω = " + QString::number(slv.w) + ", применены критерии" +
                         " остановки по точности ε<sub>мет</sub> = " + QString::number(slv.epsilon) + " и по числу итераций N<sub>max</sub> = " + QString::number(slv.max_it) +
                         "</p><p> На решение схемы (СЛАУ) затрачено итераций N = " + QString::number(slv.it) +
                         ", и достигнута точность итерационного метода ε<sup>(N)</sup> = " + QString::number(slv.achieved_accuracy) +
                         "</p><p> Схема (СЛАУ) решена с невязкой || R<sup>(N)</sup>|| = " + QString::number(slv.max_r) +
                         ", для невязки СЛАУ использована евклидова норма" +
                         "</p><p> Для контроля точности решения использована сетка с половинным шагом, метод верхней релаксации с параметром ω<sub>2</sub> = " + QString::number(slv2.w) +
                         ", применены критерии остановки по точности ε <sub>мет-2</sub> = " + QString::number(slv2.epsilon) + " и по числу итераций N<sub>max-2</sub> = " + QString::number(slv2.max_it) +
                         "</p><p> На решение задачи (СЛАУ) затрачено итераций N2 = " + QString::number(slv2.it) +
                         ", и достигнута точность итерационного метода ε<sup>(N2)</sup> = " + QString::number(slv2.achieved_accuracy) +
                         "</p><p> Схема (СЛАУ) на сетке с половинным шагом решена с невязкой\n" + " || R<sup>(N2)</sup>|| = " + QString::number(slv2.max_r) +
                         ", для невязки СЛАУ использована евклидова норма" +
                         "</p><p> Основная задача должна быть решена с погрешностью не более ε = 0.5⋅10<sup>-6</sup>" +
                         "</p><p> Задача решена с погрешностью ε<sub>2</sub> = " + QString::number(slv.max_z) +
                         "</p><p> Максимальное отклонение численных решений на основной сетке и сетке с половинным шагом наблюдается в узле x = " + QString::number(slv.max_x) + ", y = " + QString::number(slv.max_y) +
                         "</p><p> В качестве начального приближения на обоих сетках использована линейная интерполяция по x" +
                         "</p></h3>"
                         "</center>");
        QStringList headerlist2{"It", "Eps", "||R||", "It2", "Eps2", "||R2||"};
        tableWidget4->setColumnCount(slv2.iter.size() > slv.iter.size() ?  slv2.iter.size() + 1 : slv.iter.size() + 1);
        tableWidget4->setRowCount(6);
        tableWidget4->setVerticalHeaderLabels(headerlist2);
        for (int i = 0; i <= slv.iter.size(); i++) {
            tableWidget4->setItem(0, i, new QTableWidgetItem(QString::number((slv.iter[i]))));
            tableWidget4->setItem(1, i, new QTableWidgetItem(QString::number(static_cast<double>(slv.ACCURACY[i]))));
            tableWidget4->setItem(2, i, new QTableWidgetItem(QString::number(static_cast<double>(slv.MAX_R[i]))));
        }
        for(int i = 0; i <= slv2.iter.size(); i++){
            tableWidget4->setItem(3, i, new QTableWidgetItem(QString::number((slv2.iter[i]))));
            tableWidget4->setItem(4, i, new QTableWidgetItem(QString::number(static_cast<double>(slv2.ACCURACY[i]))));
            tableWidget4->setItem(5, i, new QTableWidgetItem(QString::number(static_cast<double>(slv2.MAX_R[i]))));
        }
    }
    tableWidget4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(summary);
    layout->addWidget(tableWidget4);
    summaryBox->setLayout(layout);
    summaryBox->setBaseSize(tableWidget4->size() + summary->size());
    summaryBox->show();
}

void MainWindow::showPGraph(){
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    int maxN = maxsteps->text().toInt();
    int maxN2 = maxsteps2->text().toInt();
    int skipx = 1;
    int skipy = 1;
    type_d ww = w->text().toDouble();
    type_d epsilon = eps->text().toDouble();
    type_d ww2 = w2->text().toDouble();
    type_d epsilon2 = eps2->text().toDouble();
    if (selectedTask == Functions::test){
        if (Xn != slv.N || Yn != slv.M || maxN != slv.max_it || epsilon != slv.epsilon || selectedTask != slv.task || slv.w != ww) {
            return;
        }
        else if (!view->hasSeries(dataPSeries)) {
        }
        else return;
        if (view->hasSeries(dataPSeries)) view->removeSeries(dataPSeries);

        dataPSeries = new QSurface3DSeries;
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        if(Xn < 100 && Yn < 100) dataPSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataPSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + slv.h * i * skipx, z[9][i * skipx][j * skipy], c + slv.k * j * skipy)));
            }
            dataPSeries->dataProxy()->addRow(row);
        }
        dataPSeries->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
        dataPSeries->setColorStyle(Q3DTheme::ColorStyleObjectGradient);
        dataPSeries->setWireframeColor(QColor::fromString("#daf520"));
        //dataSeries->setTextureFile("iceberg.jpg");
        dataPSeries->setBaseColor(QColor(0, 0, 255, 255));
        view->addSeries(dataPSeries);
        view->axisX()->setRange(a, b);
        view->axisZ()->setRange(c, d);
        view->axisY()->setRange(-1, 1);
    }
    if (selectedTask == Functions::tmain){
        if (Xn != slv.N || Yn != slv.M || maxN != slv.max_it || epsilon != slv.epsilon || selectedTask != slv.task || slv.w != ww || epsilon2 != slv2.epsilon || slv2.w != ww2 || maxN2 != slv2.max_it) {
            return;
        }
        else if (!view->hasSeries(dataPSeries)) {
        }
        else return;
        if (view->hasSeries(dataPSeries)) view->removeSeries(dataPSeries);

        dataPSeries = new QSurface3DSeries;
        if (Xn >= 1000 || Yn >= 1000) { skipx = Xn / 100; skipy = Yn / 100; Xn = 100; Yn = 100; }
        if(Xn < 100 && Yn < 100) dataPSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else dataPSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        for (int j = 0; j <= Yn; j++) {
            QSurfaceDataRow* row = new QSurfaceDataRow;
            for (int i = 0; i <= Xn; i++) {
                row->append(QSurfaceDataItem(QVector3D(a + slv.h * i * skipx, v[9][i * skipx][j * skipy] - v2[9][2 * i * skipx][2 * j * skipy], c + slv.k * j * skipy)));
            }
            dataPSeries->dataProxy()->addRow(row);
        }
        dataPSeries->setItemLabelFormat("solution @xLabel @yLabel @zLabel");
        dataPSeries->setColorStyle(Q3DTheme::ColorStyleObjectGradient);
        //dataSeries->setTextureFile("iceberg.jpg");
        dataPSeries->setBaseColor(QColor(0, 0, 255, 255));
        dataPSeries->setWireframeColor(QColor::fromString("#daf520"));
        view->addSeries(dataPSeries);
        view->axisX()->setRange(a, b);
        view->axisZ()->setRange(c, d);
        view->axisY()->setRange(-1, 1);
    }
    slider->setValue(10);
    valueLabel->setText("Снимок: 10");
}

void MainWindow::removePGraph(){
    if (view->hasSeries(dataPSeries)) view->removeSeries(dataPSeries);
}
