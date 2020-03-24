#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    selector.setWindowTitle("Select executable");
    selector.setFilter(QDir::Executable | QDir::Files);
    selector.setNameFilter("*.exe");

    QVBoxLayout* v_tmp_layout = new QVBoxLayout(ui->centralWidget);
    ui->centralWidget->setLayout(v_tmp_layout);

    QHBoxLayout* h_tmp_layout = new QHBoxLayout(ui->centralWidget);
    v_tmp_layout->addLayout(h_tmp_layout);

    h_tmp_layout->addWidget(&file);
    h_tmp_layout->addWidget(&select);
    select.setText("Select");
    file.setReadOnly(true);
    connect(&select, SIGNAL(clicked(bool)), &selector, SLOT(exec()));
    connect(&selector, SIGNAL(fileSelected(QString)), &file, SLOT(setText(QString)));

    h_tmp_layout = new QHBoxLayout(ui->centralWidget);
    v_tmp_layout->addLayout(h_tmp_layout);

    h_tmp_layout->addWidget(&args_text);
    h_tmp_layout->addWidget(&args);
    args_text.setText("args :");

    h_tmp_layout = new QHBoxLayout(ui->centralWidget);
    v_tmp_layout->addLayout(h_tmp_layout);

    h_tmp_layout->addWidget(&out);
    h_tmp_layout->addWidget(&err);

    h_tmp_layout = new QHBoxLayout(ui->centralWidget);
    v_tmp_layout->addLayout(h_tmp_layout);

    h_tmp_layout->addWidget(&start);
    h_tmp_layout->addWidget(&stop);
    h_tmp_layout->addWidget(&text);
    h_tmp_layout->addWidget(&exit_code);
    text.setText("Exit code:");
    stop.setText("Stop");
    start.setText("Start");
    out.setReadOnly(true);
    connect(&stop, SIGNAL(clicked(bool)), this, SLOT(on_stop()));
    connect(&start, SIGNAL(clicked(bool)), this, SLOT(on_start()));

//    stop.setEnabled(false);
    staying();
}


void MainWindow::on_start()
{
    err.clear();
    out.clear();
    prog.setProgram(file.text());
    prog.setArguments(args.text().split(" "));
    connect(&prog, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_exit()));
    connect(&prog, SIGNAL(readyReadStandardOutput()), this, SLOT(read_out()));
    connect(&prog, SIGNAL(readyReadStandardError()), this, SLOT(read_err()));
//    prog.waitForStarted(1000);
//    if (prog.waitForStarted(1000)) {
//        start.setEnabled(false);
//        stop.setEnabled(true);
//    }
//    connect(&prog, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_exit()));
    connect(&prog, SIGNAL(started()), this, SLOT(running()));
    connect(&prog, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(staying()));

    prog.start();
}


void MainWindow::running()
{
    start.setEnabled(false);
    stop.setEnabled(true);
}

void MainWindow::staying()
{
    start.setEnabled(true);
    stop.setEnabled(false);
}

void MainWindow::on_stop()
{
    prog.kill();
}

void MainWindow::read_err()
{
//    err.appendPlainText(prog.readAllStandardError());
    err.moveCursor (QTextCursor::End);
    err.insertPlainText(prog.readAllStandardError());
    err.moveCursor (QTextCursor::End);

}

void MainWindow::read_out()
{
//    out.appendPlainText(prog.readAllStandardOutput());
    out.moveCursor (QTextCursor::End);
    out.insertPlainText(prog.readAllStandardOutput());
    out.moveCursor (QTextCursor::End);
}

void MainWindow::on_exit()
{
    exit_code.setText(QString::number(prog.exitCode()));
//    staying();
}


MainWindow::~MainWindow()
{
    delete ui;
}
