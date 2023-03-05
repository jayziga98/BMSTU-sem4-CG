#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwidget.h>
#include "drawer.h"
#include "vector.h"

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


void MainWindow::on_pushButtonLine_clicked()
{
    QList selected = ui->graphicsview->scene()->selectedItems();
    QMap selectedCircles = QMap<QGraphicsItem*, QList<QGraphicsItem*>> ();
    for (auto item: selected)
        if (!(item->boundingRect().width() == POINT_RADIUS + 1))
            selectedCircles[item] = QList<QGraphicsItem*> ();

    if (selectedCircles.empty())
        return;

    if (ui->graphicsview->getlinesGroup() == nullptr)
        ui->graphicsview->createlinesGroup();

    QList items = ui->graphicsview->items();
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
                    selectedCircles[circle].append(item);
            }
        }
    }

    for (const auto &circle: qAsConst(selectedCircles))
    {
        qsizetype diff = circle.size();
        int leftFinal = 0;
        int rightFinal = 0;
        vector_t p1;
        vector_t p2;
        bool lineExist = false;
        for (auto pointInCirclei = circle.begin(); pointInCirclei != circle.end(); pointInCirclei++)
            for (auto pointInCirclej = pointInCirclei + 1; pointInCirclej != circle.end(); pointInCirclej++)
            {
                vector_t pi;
                QRectF pointBoundsi = (*pointInCirclei)->boundingRect();
                init(pi, pointBoundsi.x() + POINT_RADIUS / 2, pointBoundsi.y() + POINT_RADIUS / 2);

                vector_t pj;
                QRectF pointBoundsj = (*pointInCirclej)->boundingRect();
                init(pj, pointBoundsj.x() + POINT_RADIUS / 2, pointBoundsj.y() + POINT_RADIUS / 2);

                int left = 0, right = 0;
                for (auto point: circle)
                {
                    if (point == *pointInCirclej || point == *pointInCirclei)
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

                if (abs(right - left) < diff)
                {
                    diff = abs(right - left);
                    p1 = pi;
                    p2 = pj;
                    leftFinal = left;
                    rightFinal = right;
                    lineExist = true;
                }
            }

        if (lineExist)
        {
            ui->graphicsview->setupScene();
            drawer_t drawer;
            init(ui->graphicsview->scene(), drawer, Qt::red, Qt::red);
            qDebug() << ui->graphicsview->getlinesGroup();
            ui->graphicsview->addlinesGroup(draw_line(drawer, p1, p2, QString(" [left: %1").arg(leftFinal) + QString("|%1 :right]").arg(rightFinal)));
        }
    }
}

