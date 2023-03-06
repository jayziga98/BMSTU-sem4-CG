#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "point.h"
#include "circle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    circle_t getlineEditCircleData(bool *ok = nullptr);
    point_t getlineEditPointData(bool *ok = nullptr);
    void warningMessage(QString title, QString text);
    void infoMessage(QString title, QString text);
    int askMessage(QString title, QString text);

private slots:
    void on_radioButtonPoint_clicked();
    void on_radioButtonCircle_clicked();

    void on_pushButtonLine_clicked();

    void on_actionAboutProgram_triggered();

    void closeEvent(QCloseEvent* event);

    void on_actionSideMenu_changed();

    void on_actionClear_triggered();

    void on_actionExec_triggered();

    void on_pushButtonAddCircle_clicked();
    void on_PushButtonAddPoint_clicked();

    void on_actionAboutAuthor_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
