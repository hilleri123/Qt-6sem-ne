#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QLineEdit>
#include <QProcess>
#include <QString>

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
    void on_start();
    void on_stop();
    void on_exit();
    void read_err();
    void read_out();

    void running();
    void staying();

private:
    Ui::MainWindow *ui;

    QProcess prog;
    QLineEdit file;
    QLabel args_text;
    QLineEdit args;
    QFileDialog selector;
    QPushButton select;
    QPlainTextEdit out;
    QPlainTextEdit err;
    QPushButton start;
    QPushButton stop;
    QLabel text;
    QLabel exit_code;
};

#endif // MAINWINDOW_H
