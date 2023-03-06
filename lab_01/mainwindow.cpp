#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qmessagebox.h>
#include <qmovie.h>
#include <qwidget.h>
#include "drawer.h"
#include "vector.h"
#include <QResource>
#include <QDirIterator>

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

void MainWindow::on_actionAboutProgram_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Компьютетная графика\nЛабоаторная работа №1");
    msgBox.setInformativeText("Вариант №12\n"
                              "На плоскости заданы множество точек М и круг. Выбрать из М две\n"
                              "различные точки так, чтобы наименьшим образом различались количества точек в\n"
                              "круге, лежащие по разные стороны от прямой, проходящей через эти точки.</p>");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::warningMessage(QString title, QString text)
{
    QMovie *mov = new QMovie(":/headslap.gif");

    QMessageBox msgBox = QMessageBox();
    msgBox.setWindowTitle(title);
    msgBox.setWindowIcon(QIcon(QPixmap(":/headslap.gif")));
    msgBox.setText(text);
    msgBox.addButton(QString("Закрыть"), QMessageBox::AcceptRole);

    QLabel *label = new QLabel(&msgBox);
    label->setMovie(mov);
    mov->start();

    msgBox.exec();
}

void MainWindow::infoMessage(QString title, QString text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.addButton(QString("Закрыть"), QMessageBox::AcceptRole);

    msgBox.exec();
}

int MainWindow::askMessage(QString title, QString text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.addButton(QString("Нет"), QMessageBox::RejectRole);
    msgBox.addButton(QString("Да"), QMessageBox::AcceptRole);

    return msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    if (askMessage("Завершение работы", "Вы уверены что хотите выйти?"))
        event->ignore();
    else
        event->accept();
}

void MainWindow::on_actionSideMenu_changed()
{
    ui->widgetSideMenu->setVisible(!ui->widgetSideMenu->isVisible());
}

void MainWindow::on_actionClear_triggered()
{
    ui->graphicsview->clearScene();
}

void MainWindow::on_actionExec_triggered()
{
    MainWindow::on_pushButtonLine_clicked();
}

circle_t MainWindow::getlineEditCircleData(bool *ok)
{
    circle_t c;

    QString xText = ui->lineEditCircleX->text();
    double x = xText.toDouble(ok);

    if (ok && !(*ok))
    {
        warningMessage("Ошибка! Некорректный ввод!", "Координата X должна быть вещественным числом!\nПримеры: 1.23, -1.23, 1");
        return c;
    }

    QString yText = ui->lineEditCircleY->text();
    double y = yText.toDouble(ok);

    if (ok && !(*ok))
    {
        warningMessage("Ошибка! Некорректный ввод!", "Координата Y должна быть вещественным числом!\nПримеры: 1.23, -1.23, 1");
        return c;
    }

    QString rText = ui->lineEditCircleRadius->text();
    double r = rText.toDouble(ok);

    if (ok && !(*ok))
    {
        warningMessage("Ошибка! Некорректный ввод!", "Радиус R должен быть вещественным числом!\nПримеры: 1.23, -1.23, 1");
        return c;
    }

    init(c, x, y, r);

    return c;
}

point_t MainWindow::getlineEditPointData(bool *ok)
{
    point_t p;

    QString xText = ui->lineEditPointX->text();
    double x = xText.toDouble(ok);

    if (ok && !(*ok))
    {
        warningMessage("Ошибка! Некорректный ввод!", "Координата X должна быть вещественным числом!\nПримеры: 1.23, -1.23, 1");
        return p;
    }

    QString yText = ui->lineEditPointY->text();
    double y = yText.toDouble(ok);

    if (ok && !(*ok))
    {
        warningMessage("Ошибка! Некорректный ввод!", "Координата Y должна быть вещественным числом!\nПримеры: 1.23, -1.23, 1");
        return p;
    }

    init(p, x, y);

    return p;
}

void MainWindow::on_pushButtonAddCircle_clicked()
{
    bool ok = 1;
    circle_t c = getlineEditCircleData(&ok);
    if (!ok)
        return;

    drawer_t drawer;
    init(ui->graphicsview->scene(), drawer, Qt::black, Qt::black);

    draw_circle(drawer, c);
}

void MainWindow::on_PushButtonAddPoint_clicked()
{
    bool ok = 1;
    point_t p = getlineEditPointData(&ok);
    if (!ok)
        return;

    drawer_t drawer;
    init(ui->graphicsview->scene(), drawer, Qt::black, Qt::black);

    draw_point(drawer, p);
}

void MainWindow::on_pushButtonLine_clicked()
{
    QList selected = ui->graphicsview->scene()->selectedItems();
    QMap selectedCircles = QMap<QGraphicsItem*, QList<QGraphicsItem*>> ();
    for (auto item: selected)
        if (!(item->boundingRect().width() == POINT_RADIUS + 1))
            selectedCircles[item] = QList<QGraphicsItem*> ();

    if (selectedCircles.empty())
    {
        infoMessage("Необходимо выделить окружности!", "Чтобы построить линию требуется выделить окружность с помощью CMD+ЛКМ");
        return;
    }

    if (ui->graphicsview->getlinesGroup() == nullptr)
        ui->graphicsview->createlinesGroup();

    QList items = ui->graphicsview->items();
    QList points = QList<QGraphicsItem*> ();
    for (auto item: items)
    {
        if ((item->boundingRect().width() == POINT_RADIUS + 1))
        {
            point_t p;
            QRectF pointBounds = item->boundingRect();
            init(p, pointBounds.x() + POINT_RADIUS, pointBounds.y() + POINT_RADIUS);
            for (auto circle: selectedCircles.keys())
            {
                circle_t c;
                QRectF circleBounds = circle->boundingRect();
                double rad = circleBounds.width() / 2;
                init(c, circleBounds.x() + rad, circleBounds.y() + rad, rad);
                if (contains(c, p))
                {
                    selectedCircles[circle].append(item);
                    qDebug() << "Contains";
                }
            }
            points.append(item);
        }
    }

    if (points.size() < 2)
    {
        infoMessage("", "Для построения линии требуется не менее двух точек");
        return;
    }

    auto curCircle = selectedCircles.begin();

    qsizetype diff = curCircle.value().size() + 1;
    int leftFinal = 0;
    int rightFinal = 0;
    vector_t p1;
    vector_t p2;
    bool lineExist = false;
    auto content = curCircle.value();

    for (auto pointI = points.begin(); pointI != points.end(); pointI++)
        for (auto pointJ = pointI + 1; pointJ != points.end(); pointJ++)
        {
            vector_t pi;
            QRectF pointBoundsi = (*pointI)->boundingRect();
            init(pi, pointBoundsi.x() + POINT_RADIUS / 2, pointBoundsi.y() + POINT_RADIUS / 2);

            vector_t pj;
            QRectF pointBoundsj = (*pointJ)->boundingRect();
            init(pj, pointBoundsj.x() + POINT_RADIUS / 2, pointBoundsj.y() + POINT_RADIUS / 2);

            int left = 0, right = 0;
            for (auto point: content)
            {
                if (point == *pointJ || point == *pointI)
                    continue;

                point_t p;
                QRectF pointBounds = point->boundingRect();
                init(p, pointBounds.x() + POINT_RADIUS, pointBounds.y() + POINT_RADIUS);
                int d = direction(pi, pj, p);

                if (d > 0)
                    left++;
                else if (d < 0)
                    right++;
            }

            if (abs(right - left) < diff || (abs(right - left) == diff && left + right > leftFinal + rightFinal))
            {
                diff = abs(right - left);
                p1 = pi;
                p2 = pj;
                leftFinal = left;
                rightFinal = right;
                lineExist = true;
            }
        }

    circle_t c;
    QRectF circleBounds = curCircle.key()->boundingRect();
    double rad = circleBounds.width() / 2;
    init(c, circleBounds.x() + rad, circleBounds.y() + rad, rad);

    if (lineExist)
    {
        ui->graphicsview->setupScene();
        drawer_t drawer;
        init(ui->graphicsview->scene(), drawer, Qt::red, Qt::red);
        QGraphicsLineItem *line = draw_line(drawer, p1, p2, QString(" [left: %1").arg(leftFinal) + QString("|%1 :right]").arg(rightFinal));
        ui->graphicsview->addlinesGroup(line);
    }
}

void MainWindow::on_actionAboutAuthor_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Козлитин Максим ИУ7-44Б");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

