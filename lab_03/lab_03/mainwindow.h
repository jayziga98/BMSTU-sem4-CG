#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "staskpool.h"

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
    int curAlgorithmInd();

private slots:
    void on_pushButton_circle_clicked();

    void on_pushButton_pen_clicked();

    void on_pushButton_bg_clicked();

    void on_pushButton_ellipse_clicked();

    void on_pushButton_spectrum_ellipse_clicked();

    void on_pushButton_circle_spectrum_clicked();

    void on_checkBox_r1_clicked();

    void on_checkBox_r2_clicked();

    void on_checkBox_step_clicked();

    void on_checkBox_amount_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
