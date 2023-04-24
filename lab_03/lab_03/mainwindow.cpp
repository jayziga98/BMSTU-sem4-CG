#include "mainwindow.h"
#include "circle.h"
#include "ellipse.h"
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
    QRectF c = QRectF(ui->circle_x->value(), ui->circle_y->value(), ui->circle_height->value(), ui->circle_height->value());
    QRectF qrect = QRectF(c.x() - c.width(), c.y() - c.height(), c.width() * 2, c.height() * 2);

    params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = algoType(curAlgorithmInd());

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


void MainWindow::on_pushButton_ellipse_clicked()
{
    QRectF e = QRectF(ui->ellipse_x->value(), ui->ellipse_y->value(), ui->ellipse_width->value(), ui->ellipse_height->value());
    QRectF qrect = QRectF(e.x() - e.width(), e.y() - e.height(), e.width() * 2, e.height() * 2);

    params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = algoType(curAlgorithmInd());

    this->pool->allocAndSend(ui->graphicsView->scene(), ellipse(qrect, params, nullptr));
}


void MainWindow::on_pushButton_spectrum_ellipse_clicked()
{
    QRectF e = QRectF(ui->spectrum_ellipse_x->value(), ui->spectrum_ellipse_y->value(), ui->spectrum_ellipse_width->value(), ui->spectrum_ellipse_height->value());
    QRectF qrect = QRectF(e.x() - e.width(), e.y() - e.height(), e.width() * 2, e.height() * 2);
    double step = ui->spinBox_ellipse_step->value();
    int amount = ui->spinBox_ellipse_amount->value();

    params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = algoType(curAlgorithmInd());

    this->pool->allocAndSend(ui->graphicsView->scene(), ellipse_spectrum(qrect, params, step, amount));
}


void MainWindow::on_pushButton_circle_spectrum_clicked()
{
    QPointF c = QPointF(ui->spectrum_circle_x->value(), ui->spectrum_circle_y->value());
    double step = 0;
    int amount = 0;
    double r1 = 0;
    double r2 = 0;
    if (ui->checkBox_r1->isChecked())
        r1 = ui->spectrum_circle_r1->value();
    if (ui->checkBox_r2->isChecked())
        r2 = ui->spectrum_circle_r2->value();
    if (ui->checkBox_step->isChecked())
        step = ui->spinBox_step->value();
    if (ui->checkBox_amount->isChecked())
        amount = ui->spinBox_amount->value();

    if ((int)ui->checkBox_r1->isChecked() + (int)ui->checkBox_r2->isChecked() + (int)ui->checkBox_step->isChecked() + (int)ui->checkBox_amount->isChecked() < 3)
        return;

    if (!ui->checkBox_r1->isChecked())
        r1 = r2 - step * amount;
    if (!ui->checkBox_step->isChecked())
        step = (r2 - r1) / amount;
    if (!ui->checkBox_amount->isChecked())
        amount = (r2 - r1) / step;

    QRectF qrect = QRectF(c.x() - r1, c.y() - r1, r1 * 2, r1 * 2);

    qDebug() << c << r1 << r2 << step << amount;

    params_t params;
    params.color = ui->pushButton_pen->palette().color(QWidget::backgroundRole());
    params.type = algoType(curAlgorithmInd());

    this->pool->allocAndSend(ui->graphicsView->scene(), circle_spectrum(qrect, params, step, amount));
}


void MainWindow::on_checkBox_r1_clicked()
{
    if (ui->checkBox_r1->isChecked())
    {
        if (ui->checkBox_r2->isChecked())
            ui->checkBox_r2->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_step->isChecked())
            ui->checkBox_step->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_amount->isChecked())
            ui->checkBox_amount->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_r2->setCheckState(Qt::Checked);
        ui->checkBox_step->setCheckState(Qt::Checked);
        ui->checkBox_amount->setCheckState(Qt::Checked);
    }
}


void MainWindow::on_checkBox_r2_clicked()
{
    if (ui->checkBox_r2->isChecked())
    {
        if (ui->checkBox_r1->isChecked())
            ui->checkBox_r1->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_step->isChecked())
            ui->checkBox_step->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_amount->isChecked())
            ui->checkBox_amount->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_r1->setCheckState(Qt::Checked);
        ui->checkBox_step->setCheckState(Qt::Checked);
        ui->checkBox_amount->setCheckState(Qt::Checked);
    }
}


void MainWindow::on_checkBox_step_clicked()
{
    if (ui->checkBox_step->isChecked())
    {
        if (ui->checkBox_r2->isChecked())
            ui->checkBox_r2->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_r1->isChecked())
            ui->checkBox_r1->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_amount->isChecked())
            ui->checkBox_amount->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_r1->setCheckState(Qt::Checked);
        ui->checkBox_r2->setCheckState(Qt::Checked);
        ui->checkBox_amount->setCheckState(Qt::Checked);
    }
}


void MainWindow::on_checkBox_amount_clicked()
{
    if (ui->checkBox_amount->isChecked())
    {
        if (ui->checkBox_r2->isChecked())
            ui->checkBox_r2->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_r1->isChecked())
            ui->checkBox_r1->setCheckState(Qt::Unchecked);
        else if (ui->checkBox_amount->isChecked())
            ui->checkBox_amount->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_r2->setCheckState(Qt::Checked);
        ui->checkBox_step->setCheckState(Qt::Checked);
        ui->checkBox_r1->setCheckState(Qt::Checked);
    }
}

