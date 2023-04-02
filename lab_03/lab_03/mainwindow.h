#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "staskpool.h"
#include "line.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    STaskPool *pool;

public:
    MainWindow(STaskPool *pool, QWidget *parent = nullptr);
    ~MainWindow();
    algoType curAlgorithm();

private slots:
    void on_pushButton_line_clicked();

    void on_pushButton_pen_clicked();

    void on_pushButton_bg_clicked();

    void on_pushButton_spectrum_clicked();

    void on_pushButton_time_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
