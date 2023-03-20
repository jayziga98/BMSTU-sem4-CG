#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "figure_item.h"
#include "undoredo/staskpool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    figure_item_t *item = nullptr;
    QGraphicsItemGroup *grid = nullptr;

    STaskPool *pool;

public:
    explicit MainWindow(STaskPool *pool, QWidget *parent = nullptr);
    ~MainWindow();

    void fitAll();

    void removeGrid();
    void drawGrid();

    void warningMessage(QString title, QString text);
    void infoMessage(QString title, QString text);
    int askMessage(QString title, QString text);

private slots:
    void on_action_triggered();

    void on_pushButton_rotate_clicked();

    void on_pushButton_move_clicked();

    void on_pushButton_scale_clicked();

    void on_pushButton_toCenter_clicked();

    void on_actionAboutTask_triggered();

    void on_actionAboutProgram_triggered();

    void on_actionAboutAuthor_triggered();

    void showEvent(QShowEvent* event);

    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
