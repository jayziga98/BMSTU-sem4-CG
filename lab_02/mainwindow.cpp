#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qaction.h>
#include <QFileDialog>
#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

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
    this->pool->setWidget(ui->listWidget);
    QList<QAction*> list;
    list.push_back(pool->getUndoCommand());
    list.push_back(pool->getRedoCommand());
    this->ui->menuEdit->addActions(list);

    this->ui->graphicsView->scale(1, -1);

    drawGrid();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::removeGrid()
{
    if (this->grid)
        ui->graphicsView->scene()->removeItem(this->grid);

    this->grid = nullptr;
}

void MainWindow::drawGrid()
{
    this->grid = new QGraphicsItemGroup();

    QRectF sceneBounds = ui->graphicsView->scene()->itemsBoundingRect();

    double width = fmax(500, sceneBounds.width());
    double height = fmax(500, sceneBounds.height());

    int delim = (int)(fmin(width, height) / 10);

    QPen penGrid = QPen(Qt::lightGray);
    QPen penDelim = QPen(Qt::red);
    QColor textColor = QColor(Qt::black);
    QFont textFont = QFont("Helvetica", 14 + (int)(2 * (delim / 50)), QFont::Bold);

    for (int i = delim; (double)i <= width; i += delim)
    {
        auto lline = new QGraphicsLineItem(i, -height, i, height);
        auto rline = new QGraphicsLineItem(-i, -height, -i, height);
        lline->setPen(penGrid);
        rline->setPen(penGrid);

        this->grid->addToGroup(lline);
        this->grid->addToGroup(rline);

        auto ldelim = new QGraphicsLineItem(i, -delim / 2, i, delim / 2);
        auto rdelim = new QGraphicsLineItem(-i, -delim / 2, -i, delim / 2);
        ldelim->setPen(penDelim);
        rdelim->setPen(penDelim);

        this->grid->addToGroup(ldelim);
        this->grid->addToGroup(rdelim);

        auto ltext = new QGraphicsTextItem(QString::number(i));
        auto rtext = new QGraphicsTextItem(QString::number(-i));
//        ltext->setPos(i - ltext->boundingRect().width() / 2, -delim / 2 + ltext->boundingRect().height() / 2);
//        rtext->setPos(-i - rtext->boundingRect().width() / 2, delim / 2 + rtext->boundingRect().height() / 2);
        ltext->setPos(i - ltext->boundingRect().width() / 2, ltext->boundingRect().height() / 2);
        rtext->setPos(-i - rtext->boundingRect().width() / 2, rtext->boundingRect().height() / 2);
        ltext->setFont(textFont);
        rtext->setFont(textFont);
        ltext->setDefaultTextColor(textColor);
        rtext->setDefaultTextColor(textColor);
        ltext->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        rtext->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        this->grid->addToGroup(ltext);
        this->grid->addToGroup(rtext);
    }

    for (int i = delim; (double)i <= height; i += delim)
    {
        auto uline = new QGraphicsLineItem(-width, i, width, i);
        auto dline = new QGraphicsLineItem(-width, -i, width, -i);
        uline->setPen(penGrid);
        dline->setPen(penGrid);

        this->grid->addToGroup(dline);
        this->grid->addToGroup(uline);

        auto udelim = new QGraphicsLineItem(-delim / 2, i, delim / 2, i);
        auto ddelim = new QGraphicsLineItem(-delim / 2, -i, delim / 2, -i);
        udelim->setPen(penDelim);
        ddelim->setPen(penDelim);

        this->grid->addToGroup(udelim);
        this->grid->addToGroup(ddelim);

        auto utext = new QGraphicsTextItem(QString::number(i));
        auto dtext = new QGraphicsTextItem(QString::number(-i));
//        utext->setPos(delim / 2 + utext->boundingRect().width() / 2, i + utext->boundingRect().height() / 2);
//        dtext->setPos(-delim - dtext->boundingRect().width() / 2, -i + dtext->boundingRect().height() / 2);
        utext->setPos(-utext->boundingRect().width() / 2, i + utext->boundingRect().height() / 2);
        dtext->setPos(-dtext->boundingRect().width() / 2, -i + dtext->boundingRect().height() / 2);
        utext->setFont(textFont);
        dtext->setFont(textFont);
        utext->setDefaultTextColor(textColor);
        dtext->setDefaultTextColor(textColor);
        utext->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        dtext->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        this->grid->addToGroup(utext);
        this->grid->addToGroup(dtext);
    }

    this->grid->addToGroup(new QGraphicsLineItem(0, -height, 0, height));
    this->grid->addToGroup(new QGraphicsLineItem(-width, 0, width, 0));

    ui->graphicsView->scene()->addItem(this->grid);
}

void MainWindow::fitAll()
{
    removeGrid();

    QRectF sceneBounds = ui->graphicsView->scene()->itemsBoundingRect();
    QPointF center = QPointF(sceneBounds.x() + sceneBounds.width() / 2, sceneBounds.y() + sceneBounds.height() / 2);
    sceneBounds.setWidth(sceneBounds.width()*0.9);         // to tighten-up margins
    sceneBounds.setHeight(sceneBounds.height()*0.9);
    ui->graphicsView->fitInView(sceneBounds, Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(center);

    drawGrid();
}

void MainWindow::on_action_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Выберите фигуру"), "../../../../lab_02/data", tr("TXT Files (*.txt)"));

    FILE *f = freopen(filename.toLocal8Bit().data(), "r", stdin);
    if (f == NULL)
    {
        warningMessage("Что-то пошло не так", "Не удалось открыть выбранный файл.\nПопробуйте еще раз или выберите другой файл");
        return;
    }

    bool ok;
    figure_t figure = figure_scan(f, ok);
    if (!ok)
    {
        warningMessage("Что-то пошло не так", "Кажется, выбранный файл некорректно задает описание рисунка.\nПопробуйте выбрать другой файл или изменить содержимое");
        return;
    }

    if (this->item)
        ui->graphicsView->scene()->removeItem(this->item);

    this->item = figure_item_create();
    figure_item_init(this->item, figure);

    figure_item_scale(this->item, 1, -1, 0, 0, 0);

    ui->graphicsView->scene()->addItem(this->item);
    fitAll();
}

void MainWindow::on_pushButton_rotate_clicked()
{
    if (this->item == nullptr)
    {
        warningMessage("Ошибка! Выберите фигуру", "Прежде чем выполнить поворот требуется выбрать фигуру. \n Для этого можно воспользоваться сочетанием клавиш «Ctrl+O» («Cmd+O»)");
        return;
    }

    double angle = -ui->doubleSpinBox_angle->value();

    double basex = ui->doubleSpinBox_baseX->value();
    double basey = ui->doubleSpinBox_baseY->value();
    double base = ui->checkBox_base->isChecked() ? 1.0 : 0.0;

    //figure_item_rotate(this->item, angle, 0, basex, basey);

    this->pool->allocAndSend(ROTATE, this->item, std::vector<double> {angle, base, basex, basey});

    fitAll();
}


void MainWindow::on_pushButton_move_clicked()
{
    if (this->item == nullptr)
    {
        warningMessage("Ошибка! Выберите фигуру", "Прежде чем выполнить перемещение требуется выбрать фигуру. \n Для этого можно воспользоваться сочетанием клавиш «Ctrl+O» («Cmd+O»)");
        return;
    }

    double dx = ui->doubleSpinBox_dx->value();
    double dy = ui->doubleSpinBox_dy->value();

    //figure_item_move(this->item, dx, dy);

    this->pool->allocAndSend(MOVE, this->item, std::vector<double> {dx, dy});

    fitAll();
}

void MainWindow::on_pushButton_scale_clicked()
{
    if (this->item == nullptr)
    {
        warningMessage("Ошибка! Выберите фигуру", "Прежде чем выполнить масштабирование требуется выбрать фигуру. \n Для этого можно воспользоваться сочетанием клавиш «Ctrl+O» («Cmd+O»)");
        return;
    }

    double kx = ui->doubleSpinBox_kx->value();
    double ky = ui->doubleSpinBox_ky->value();

    double basex = ui->doubleSpinBox_baseX->value();
    double basey = ui->doubleSpinBox_baseY->value();
    double base = ui->checkBox_base->isChecked() ? 0.0 : 1.0;

    //figure_item_scale(this->item, kx, ky, 0, basex, basey);

    this->pool->allocAndSend(SCALE, this->item, std::vector<double> {kx, ky, base, basex, basey});

    fitAll();
}


void MainWindow::on_pushButton_toCenter_clicked()
{
    if (this->item == nullptr)
    {
        warningMessage("Ошибка! Выберите фигуру", "Прежде чем выполнить перемешение требуется выбрать фигуру. \n Для этого можно воспользоваться сочетанием клавиш «Ctrl+O» («Cmd+O»)");
        return;
    }

    QRectF childRect = item->childrenBoundingRect();
    double basex = childRect.x() + childRect.width() / 2;
    double basey = childRect.y() + childRect.height() / 2;

    this->pool->allocAndSend(MOVE, this->item, std::vector<double> {-basex, -basey});

    fitAll();
}

void MainWindow::on_actionAboutTask_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Компьютетная графика\nЛабоаторная работа №2");
    msgBox.setInformativeText("Вариант №12\n"
                              "Нарисовать исходный рисунок, затем его переместить, промасштабировать, повернуть.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionAboutProgram_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Дорогой пользователь!");
    msgBox.setInformativeText("Предлагаю ознакомиться со справкой о программе\n"
                              "Программа позволяет просматривать рисунок и выполнять операции масштабирования, поворота, перемещения над ним\n"
                              "Прежде чем выполнять операции требуется выбрать рисунок, чтобы это сделать воспользуйся верхним меню или сочетанием клавиш «Ctrl+O» («Cmd+O»).\n"
                              "После ты можешь выполнять перемещение, масштабирование и поворот исходного рисунка. Дополнительно ты можешь задать опорную точку масштабирования и поворота.\n"
                              "Программа позволяет отменять выполненные действия сочетанием клавиш «Ctrl+Z» («Cmd+Z»)  или с помощью меню «Правка» -> «Отменить»\n"
                              "Отмененные действия можно повторить сочетанием клавиш «Ctrl+Shift+Z» («Cmd+Shift+Z»)  или с помощью меню «Правка» -> «Повторить»\n"
                              "Данную информацию можно повторно просмотреть в меню «Справка» -> «О программе» или просмотреть подсказки при наведении на действия в левом боковом меню\n"
                              "Приятного использования!");

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionAboutAuthor_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Козлитин Максим ИУ7-44Б");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::warningMessage(QString title, QString text)
{
    QMessageBox msgBox = QMessageBox();
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.addButton(QString("Закрыть"), QMessageBox::AcceptRole);

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

void MainWindow::showEvent(QShowEvent* event)
{
    QMessageBox msgBox;
    msgBox.setText("Дорогой пользователь!");
    msgBox.setInformativeText("Предлагаю ознакомиться со справкой о программе\n"
                              "Программа позволяет просматривать рисунок и выполнять операции масштабирования, поворота, перемещения над ним\n"
                              "Прежде чем выполнять операции требуется выбрать рисунок, чтобы это сделать воспользуйся верхним меню или сочетанием клавиш «Ctrl+O» («Cmd+O»).\n"
                              "После ты можешь выполнять перемещение, масштабирование и поворот исходного рисунка. Дополнительно ты можешь задать опорную точку масштабирования и поворота.\n"
                              "Программа позволяет отменять выполненные действия сочетанием клавиш «Ctrl+Z» («Cmd+Z»)  или с помощью меню «Правка» -> «Отменить»\n"
                              "Отмененные действия можно повторить сочетанием клавиш «Ctrl+Shift+Z» («Cmd+Shift+Z»)  или с помощью меню «Правка» -> «Повторить»\n"
                              "Данную информацию можно повторно просмотреть в меню «Справка» -> «О программе» или просмотреть подсказки при наведении на действия в левом боковом меню\n"
                              "Приятного использования!");

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    if (askMessage("Завершение работы", "Вы уверены что хотите выйти?"))
        event->ignore();
    else
        event->accept();
}
