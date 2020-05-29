#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QTextEdit>
#include <QDateTime>
#include <QLabel>

#include "../filePlugin/plugin.h"

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
    bool cd(QString);

    bool loadPlugin(QString);


    void runFindAndFilter();

    void appendFiles(QFileInfoList);

private:
    Ui::MainWindow *ui;
    QPushButton run;
    QFileDialog select_dir;
    QFileDialog select_plug;
    QPushButton plug;
    QComboBox dir;
    QPushButton select;
    QDir curr;
    QTextEdit text;
    QLineEdit filter;
    QSpinBox c_size;
    QSpinBox c_age;
    QCheckBox use_s;
    QCheckBox use_a;
    QComboBox more_s;
    QComboBox more_a;
    QLabel count;
    QLabel filter_label;
    int files = 0;


    Plugin* plugin;
};

#endif // MAINWINDOW_H
