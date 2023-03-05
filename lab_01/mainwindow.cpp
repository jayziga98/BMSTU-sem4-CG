#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwidget.h>
#include "drawer.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButtonPoint_clicked()
{
    ui->graphicsview->setToDraw(POINT);
}


void MainWindow::on_radioButtonCircle_clicked()
{
    ui->graphicsview->setToDraw(CIRCLE);
}

