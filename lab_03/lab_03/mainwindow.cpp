#include "mainwindow.h"
#include "circle.h"
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

int MainWindow::curAlgorithmInd()
{
    return ui->comboBox_algoType->currentIndex();
}

void MainWindow::on_pushButton_circle_clicked()
{
    QRectF qrect = QRectF(ui->circle_x->value(), ui->circle_y->value(), ui->circle_width->value(), ui->circle_height->value());

    circle_params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = algoTypeCircle(curAlgorithmInd());

    this->pool->allocAndSend(ui->graphicsView->scene(), circle(qrect, params, nullptr));
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
    //todo
}


void MainWindow::on_pushButton_time_clicked()
{
    //todo
}


void MainWindow::on_pushButton_grad_clicked()
{
    //todo
}

