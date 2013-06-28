#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void pwmSpinValueChanged(int);
    void messageHit(QString);

signals:
    int pwmChanged(int);
    int labelSetMsg(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QString m1, m2, m3;
};

#endif // MAINWINDOW_H
