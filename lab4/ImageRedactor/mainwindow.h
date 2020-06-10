#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QApplication>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void start();
    void matrix();


    void update();
    void end();
    void set(QString);


private:
    Ui::MainWindow *ui;
    Controller* _c = nullptr;
    QFileDialog* _dialog1 = new QFileDialog;
    QFileDialog* _dialog2 = new QFileDialog;

    QPushButton _start;
    QPushButton _chose;
    QPushButton _stop;
    QPushButton _save;
    QPushButton _matrix;

    QLineEdit _file;

    QLabel _img1;
    QLabel _img2;

    QSpinBox _threads;
    QSpinBox _radius;
    QCheckBox _net;

    QProgressBar _progress;

    QVector<QVector<int>> mat;
    std::size_t mat_rad = 0;

    QImage img1;
    QImage img2;

    QLabel _time;
    QLabel _bytes;
};

#endif // MAINWINDOW_H
