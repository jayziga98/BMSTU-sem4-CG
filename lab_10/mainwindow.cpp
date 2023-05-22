#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1, 1);
    ui->graphicsView->setScene(scene);
    for (auto it: funcs)
        ui->functionComboBox->addItem(it.name);

//    scene->addLine(200, 200, 300, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buildButton_clicked()
{
    try
    {
        _cx = 0;
        _cy = 0;
        _cz = 0;
        size_data s_data = get_size_data();
        if (s_data.xs >= s_data.xe || s_data.zs >= s_data.ze)
            throw UIError("Диапазон значений переменных x или z указан неверно");
        int i = ui->functionComboBox->currentIndex();
        transform_data t_data = get_transform_data(s_data.xe - s_data.xs, s_data.ze - s_data.zs);
        floating_horizon(scene, funcs[i].funk, s_data, t_data, ui->graphicsView->size());
    }
    catch (Error &e)
    {
        show_error("Ошибка", e.get_message());
    }
}


void MainWindow::on_rotateButton_clicked()
{
    try
    {
        scene->clear();
        size_data s_data = get_size_data();
        if (s_data.xs >= s_data.xe || s_data.zs >= s_data.ze)
            throw UIError("Диапазон значений переменных x или z указан неверно");
        int i = ui->functionComboBox->currentIndex();
        transform_data t_data = get_transform_data(s_data.xe - s_data.xs, s_data.ze - s_data.zs);
        floating_horizon(scene, funcs[i].funk, s_data, t_data, ui->graphicsView->size());
    }
    catch (Error &e)
    {
        show_error("Ошибка", e.get_message());
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    scene->clear();
}

transform_data MainWindow::get_transform_data(double dx, double dz)
{
    _cx += fmod(ui->tx->value(), 360);
    _cy += fmod(ui->ty->value(), 360);
    _cz += fmod(ui->tz->value(), 360);
    double scale = get_scale(dx, dz);
    double _dx = ui->graphicsView->size().width() / 2, _dy = ui->graphicsView->size().height() / 2;
    return {_cx, _cy, _cz, 3, _dx, _dy, 0, _dx, _dy, 0};
}

size_data MainWindow::get_size_data()
{
    double xs = ui->xs->value(), xe = ui->xe->value(), x_step = ui->xd->value();
    double zs = ui->zs->value(), ze = ui->ze->value(), z_step = ui->zd->value();
    return {xs, xe, x_step, zs, ze, z_step};
}

double MainWindow::get_scale(double dx, double dz)
{
    double sx = double(ui->graphicsView->size().width()) / (1.5 * abs(dx)),
            sz = double(ui->graphicsView->size().height()) / (1.5 * abs(dz));
    return (sx > sz) ? sx : sz;
}

void MainWindow::show_error(QString title, QString message)
{
    QErrorMessage error_msg;
    error_msg.showMessage(message);
    error_msg.setWindowTitle(title);
    error_msg.exec();
}

