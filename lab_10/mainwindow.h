#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QErrorMessage>
#include <QVector>

#include "Alg.h"
#include "Errors.h"
#include "funcs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buildButton_clicked();

    void on_rotateButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    double _cx = 0, _cy = 0, _cz = 0;

    QVector<function> funcs= {{"x^2 + z^2", f1},
                              {"sin(x) + cos(z)", f2},
                              {"cos(x^2 + z^2)", f3}};

    transform_data get_transform_data(double dx, double dz);
    size_data get_size_data();
    double get_scale(double dx, double dz);
    void show_error(QString title, QString message);
};

#endif
