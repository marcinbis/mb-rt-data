#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::pwmSpinValueChanged(int val)
{
    emit pwmChanged(val);
}

void MainWindow::messageHit(QString msg)
{
    m3 = m2;
    m2 = m1;
    m1 = msg;
    emit labelSetMsg(m3+"\n"+m2+"\n"+m1);
}
