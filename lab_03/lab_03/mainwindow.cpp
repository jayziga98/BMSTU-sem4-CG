#include "mainwindow.h"
#include "spectrum.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>
#include <chrono>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

MainWindow::MainWindow(STaskPool *pool, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->graphicsView->setStyleSheet("QGraphicsView {background-color: white}");

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    this->pool = pool;
    QList<QAction*> list;
    list.push_back(pool->getUndoCommand());
    list.push_back(pool->getRedoCommand());
    this->ui->menuEdit->addActions(list);

    this->ui->graphicsView->scale(1, -1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

algoType MainWindow::curAlgorithm()
{
    return algoType(ui->comboBox_algoType->currentIndex());
}

void MainWindow::on_pushButton_line_clicked()
{
    QPointF p1 = QPointF(ui->line_x1->value(), ui->line_y1->value());
    QPointF p2 = QPointF(ui->line_x2->value(), ui->line_y2->value());
    QLineF qline = QLineF(p1, p2);

    line_params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = curAlgorithm();

    this->pool->allocAndSend(ui->graphicsView->scene(), line(qline, params, nullptr));
}


void MainWindow::on_pushButton_pen_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if( color.isValid() )
    {
        int r;
        int g;
        int b;
        color.getRgb(&r, &g, &b);
        ui->pushButton_pen->setStyleSheet(QString("background: rgb(%1,%2,%3); border: none;").arg(r).arg(g).arg(b));
    }
}


void MainWindow::on_pushButton_bg_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if( color.isValid() )
    {
        int r;
        int g;
        int b;
        color.getRgb(&r, &g, &b);
        ui->pushButton_bg->setStyleSheet(QString("background: rgb(%1,%2,%3); border: none;").arg(r).arg(g).arg(b));
        ui->graphicsView->setStyleSheet(QString("background: rgb(%1,%2,%3);").arg(r).arg(g).arg(b));
    }
}


void MainWindow::on_pushButton_spectrum_clicked()
{
    QPointF c = QPointF(ui->spectrum_x1->value(), ui->spectrum_y1->value());
    qreal len = ui->spectrum_len->value();
    qreal angle = ui->spectrum_angle->value();

    line_params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = curAlgorithm();

    this->pool->allocAndSend(ui->graphicsView->scene(), spectrum(c, len, angle, params));
}


void MainWindow::on_pushButton_time_clicked()
{
    qreal len = 10000;

    QStringList categories;
    categories << "ЦДА" << "Брезенхэм (вещественные)" << "Брезенхэм (целые)" << "Брезенхэм (устранение ступенчатости)" << "Ву" << "Библиотечная";

    QList <QBarSet *> sets = QList <QBarSet *> ();
    for (int curType = 0; curType <= algoType::BIBLIO; curType++)
        sets.append(new QBarSet(categories[curType]));

    for (int curType = 0; curType <= algoType::BIBLIO; curType++)
    {
        QPointF p1 = QPointF(0, 0);
        QPointF p2 = QPointF(len, len);
        QLineF qline = QLineF(p1, p2);

        line_params_t params;
        params.color = Qt::black;
        params.type = algoType(curType);


        auto t1 = std::chrono::high_resolution_clock::now();
        QGraphicsItem *item = line(qline, params, nullptr);
        ui->graphicsView->scene()->addItem(item);
        auto t2 = std::chrono::high_resolution_clock::now();
        ui->graphicsView->scene()->removeItem(item);

        std::chrono::duration<double, std::milli> ms_double = t2 - t1;


        sets[curType]->append(ms_double.count());
    }

    QBarSeries *series = new QBarSeries();
    series->append(sets);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Временные характеристики алгоритмов");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTickCount(20);
    axisY->setLabelFormat("%.lfms");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QStringList methods;
    methods << QString("Длина %1").arg(len);
    axisX->append(methods);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(QSize(900, 600));
    chartView->show();
}


void MainWindow::on_pushButton_grad_clicked()
{
    qreal len = 100;
    int interval = 15;

    QStringList categories;
    categories << "ЦДА" << "Брезенхэм (вещественные)" << "Брезенхэм (целые)" << "Брезенхэм (устранение ступенчатости)" << "Ву";

    QList <QLineSeries *> sets = QList <QLineSeries *> ();
    for (int curType = 0; curType < categories.length(); curType++)
    {
        QLineSeries *series = new QLineSeries();
        series->setName(categories[curType]);
        sets.append(series);
    }

    for (int angle = 0; angle <= 90; angle += interval)
        for (int curType = 0; curType < categories.length(); curType++)
        {
            qreal rad = angle * PI / 180;

            QPointF start = QPointF(0, 0);
            QPointF end = QPointF(start.x() + cos(rad) * len, start.y() - sin(rad) * len);

            QLineF qline = QLineF(start, end);

            line_params_t params;
            params.color = Qt::black;
            params.type = algoType(curType);

            int step = 0;
            line(qline, params, &step);


            sets[curType]->append(QPointF(angle, step));
        }

    QChart *chart = new QChart();

    for (int curType = 0; curType < categories.length(); curType++)
        chart->addSeries(sets[curType]);

    chart->setTitle("Ступенчатость отрезков");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(QSize(900, 600));
    chartView->show();
}

