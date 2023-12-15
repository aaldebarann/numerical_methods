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
    controlsWidget->setMaximumWidth(200);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlsWidget);

    xn = new QLineEdit(this);
    xn->setText("3");
    QLabel* label1 = new QLabel("Разбиений по x:", controlsWidget);
    yn = new QLineEdit(this);
    yn->setText("3");
    QLabel* label2 = new QLabel("Разбиений по y:", controlsWidget);
    eps = new QLineEdit(this);
    eps->setText("0.001");
    QLabel* label3 = new QLabel("Требуемая точность:", controlsWidget);
    maxsteps = new QLineEdit(this);
    maxsteps->setText("10000");
    QLabel* label4 = new QLabel("Макс. число шагов:", controlsWidget);

    QPushButton *graphButton = new QPushButton("График", this);
    QPushButton *tableButton = new QPushButton("Таблица", this);

    connect(graphButton, &QPushButton::clicked, this, &MainWindow::showGraph);
    connect(tableButton, &QPushButton::clicked, this, &MainWindow::showTable);


    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(10);
    valueLabel = new QLabel("Шаг: 0");

    connect(slider, &QSlider::valueChanged, this, [=](int value) {
        valueLabel->setText("Шаг: " + QString::number(value));
        setT(value);
    });
    QLabel* label6 = new QLabel(" \n ", controlsWidget);
    controlLayout->addWidget(label1);
    controlLayout->addWidget(xn);
    controlLayout->addWidget(label2);
    controlLayout->addWidget(yn);
    controlLayout->addWidget(label3);
    controlLayout->addWidget(eps);
    controlLayout->addWidget(label4);
    controlLayout->addWidget(maxsteps);
    controlLayout->addWidget(graphButton);
    controlLayout->addWidget(tableButton);
    controlLayout->addWidget(valueLabel);
    controlLayout->addWidget(slider);
    controlLayout->addWidget(label6);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(controlsWidget);
    mainLayout->addWidget(container);

    view->axisX()->setTitle(QStringLiteral("X"));
    view->axisY()->setTitle(QStringLiteral("Y"));
    view->axisZ()->setTitle(QStringLiteral("U"));
    view->axisX()->setTitleVisible(true);
    view->axisY()->setTitleVisible(true);
    view->axisZ()->setTitleVisible(true);
};

MainWindow::~MainWindow()
{
}

void MainWindow::showGraph() {
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    int maxN = maxsteps->text().toInt();
    double epsilon = eps->text().toInt();

    if (!sphereAdded) {
        dataSeries = new QSurface3DSeries;
        dataSeries->setItemLabelFormat(QStringLiteral("(@xLabel @yLabel @zLabel)"));
        dataSeries->setMesh(QAbstract3DSeries::MeshSphere);
        dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        dataSeries->setFlatShadingEnabled(false);

        QSurfaceDataArray *data = new QSurfaceDataArray;
        QSurfaceDataRow *row = new QSurfaceDataRow;
        QSurfaceDataItem item;

        double radius = 1.0f;
        double skipphi = 360 / Xn;
        double skiptheta = 180 / Yn;
        for (double phi = 0; phi <= 360; phi += skipphi) {
            row = new QSurfaceDataRow;
            for (double theta = 0; theta <= 180; theta += skiptheta) {
                double x = radius * cos(qDegreesToRadians(phi)) * cos(qDegreesToRadians(theta));
                double y = radius * cos(qDegreesToRadians(phi)) * sin(qDegreesToRadians(theta));
                double z = radius * sin(qDegreesToRadians(phi));
                item.setPosition(QVector3D(x, y, z));
                row->append(item);
            }
            data->append(row);
        }
        dataSeries->setName(QString("Сфера r = " + QString::number(radius)));
        dataSeries->dataProxy()->resetArray(data);
        dataSeries->setItemLabelFormat("Sphere 1 @xLabel @yLabel @zLabel");
        dataSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
        view->addSeries(dataSeries);
        view->axisX()->setRange(-2 * radius, 2 * radius);
        view->axisY()->setRange(-2 * radius, 2 * radius);
        view->axisZ()->setRange(-2 * radius, 2 * radius);
        sphereAdded = true;
    }
}

void MainWindow::setT(int t){
    int Xn = xn->text().toInt();
    int Yn = yn->text().toInt();
    view->removeSeries(dataSeries);
    dataSeries = new QSurface3DSeries;
    dataSeries->setItemLabelFormat(QStringLiteral("(@xLabel @yLabel @zLabel)"));
    dataSeries->setMesh(QAbstract3DSeries::MeshSphere);
    dataSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    dataSeries->setFlatShadingEnabled(false);

    QSurfaceDataArray *data = new QSurfaceDataArray;
    QSurfaceDataRow *row = new QSurfaceDataRow;
    QSurfaceDataItem item;

    double radius = t;
    double skipphi = 360 / Xn;
    double skiptheta = 180 / Yn;
    for (double phi = 0; phi <= 360; phi += skipphi) {
        row = new QSurfaceDataRow;
        for (double theta = 0; theta <= 180; theta += skiptheta) {
            double x = radius * cos(qDegreesToRadians(phi)) * cos(qDegreesToRadians(theta));
            double y = radius * cos(qDegreesToRadians(phi)) * sin(qDegreesToRadians(theta));
            double z = radius * sin(qDegreesToRadians(phi));
            item.setPosition(QVector3D(x, y, z));
            row->append(item);
        }
        data->append(row);
    }
    dataSeries->dataProxy()->resetArray(data);
    dataSeries->setItemLabelFormat("Sphere 1 @xLabel @yLabel @zLabel");
    dataSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    view->axisX()->setRange(-2 * radius, 2 * radius);
    view->axisY()->setRange(-radius, radius);
    view->axisZ()->setRange(-2 * radius, 2 * radius);
    view->addSeries(dataSeries);
}

void MainWindow::showTable() {
    QTabWidget *tabWidget = new QTabWidget;
    for(int j = 0; j < 3; j++){
        QTableWidget *tableWidget = new QTableWidget(dataSeries->dataProxy()->array()->count(), 3);
        tableWidget->setHorizontalHeaderLabels({"X", "Y", "Z"});
        for (int i = 0; i < dataSeries->dataProxy()->array()->count(); ++i) {
            QSurfaceDataRow *row = dataSeries->dataProxy()->array()->at(i);
            tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(row->at(0).position().x())));
            tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(row->at(0).position().y())));
            tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(row->at(0).position().z())));
        }
        tabWidget->addTab(tableWidget, "Координаты сферы");
    }
    tabWidget->setMinimumSize(400, 400);
    tabWidget->setWindowTitle("Таблица");
    tabWidget->show();
}
