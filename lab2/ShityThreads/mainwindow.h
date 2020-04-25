#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QFile>
#include <QSpinBox>
#include <QTextStream>
#include <QDebug>
#include <QLabel>
#include <QList>

#include "buffer.h"
#include "producer.h"
#include "consumer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadFile(const QString&);

    void boxesChanged(int);

    void start();
    void finish();

    void threadFinished();

private:

    Ui::MainWindow *ui;

    Buffer boxes;

    QList<QWidget*> toEnable;

    QList<Producer*> prod;
    QList<Consumer*> cons;

    static const uint textCount = 3;
    QPlainTextEdit textPrime[textCount];
    QPlainTextEdit textChanged[textCount];
    QFileDialog fileDialog[textCount];
};

#endif // MAINWINDOW_H
