#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* w = new QWidget();
    QVBoxLayout* main_layout = new QVBoxLayout(w);
    setCentralWidget(w);

    QHBoxLayout* tmp = new QHBoxLayout();
    QHBoxLayout* tmp1 = new QHBoxLayout();
    for (uint i = 0; i < textCount; i++) {
        textPrime[i].setReadOnly(true);
        QPushButton* tmp_button = new QPushButton("Open file");
        toEnable.append(tmp_button);
        tmp->addWidget(tmp_button);
        connect(tmp_button, SIGNAL(clicked()), &(fileDialog[i]), SLOT(exec()));
        connect(&(fileDialog[i]), SIGNAL(fileSelected(const QString&)), this, SLOT(loadFile(const QString&)));
        tmp1->addWidget(&(textPrime[i]));
    }
    main_layout->addLayout(tmp);
    main_layout->addLayout(tmp1);

    main_layout->addWidget(&boxes);

    tmp = new QHBoxLayout();
    for (uint i = 0; i < textCount; i++) {
        textChanged[i].setReadOnly(true);
        tmp->addWidget(&(textChanged[i]));
    }
    main_layout->addLayout(tmp);

    tmp = new QHBoxLayout();
    tmp->addWidget(new QLabel("Number of boxes"));
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(1);
    toEnable.append(spinBox);
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(boxesChanged(int)));
    spinBox->setValue(5);
    QPushButton* tmp_button = new QPushButton("start");
    toEnable.append(tmp_button);
    connect(tmp_button, SIGNAL(clicked()), this, SLOT(start()));
    tmp->addWidget(spinBox);
    tmp->addWidget(tmp_button);
    main_layout->addLayout(tmp);
}


void MainWindow::start() {
    qDebug() << "start";
    for (auto i = toEnable.begin(); i != toEnable.end(); i++) {
        qDebug() << "unEnable" << *i << (*i)->isEnabled();
        (*i)->setEnabled(false);
        qDebug() << (*i)->isEnabled();
    }

//    QList<Producer*> prod;
//    QList<Consumer*> cons;
    uint sleep = 1000;
    for (uint i = 0; i < textCount; i++) {
        Producer* prod_tmp = new Producer(&(textPrime[i]), &boxes, i, sleep);
        Consumer* cons_tmp = new Consumer(&(textChanged[i]), &boxes, i, textPrime[i].toPlainText().size(), sleep);
        prod.append(prod_tmp);
        prod_tmp->start();
        cons.append(cons_tmp);
        cons_tmp->start();
    }
    qDebug() << "started";

    for (auto i = prod.begin(); i != prod.end(); i++) {
        connect(*i, SIGNAL(finished()), this, SLOT(threadFinished()));
//        (*i)->wait();
//        delete *i;
    }

    for (auto i = cons.begin(); i != cons.end(); i++) {
        connect(*i, SIGNAL(finished()), this, SLOT(threadFinished()));
//        (*i)->wait();
//        delete *i;
    }

    if (prod.size()+cons.size() == 0)
        finish();
}


void MainWindow::threadFinished() {
    QObject* sender = QObject::sender();
    if (sender == nullptr)
        return ;
    if (prod.removeOne(static_cast<Producer*>(sender)) || cons.removeOne(static_cast<Consumer*>(sender))) {
        delete sender;
        if (prod.size()+cons.size() == 0)
            finish();
    }
}


void MainWindow::finish() {
    qDebug() << "finished";
    for (auto i = toEnable.begin(); i != toEnable.end(); i++)
        (*i)->setEnabled(true);
}

void MainWindow::boxesChanged(int count) {
    qDebug() << "nuber of boxes" << count;
    boxes.setNumber(count);
}


void MainWindow::loadFile(const QString& file) {
//    qDebug() << "selected" << file;
    QObject* sender = QObject::sender();
    if (sender == nullptr)
        return ;
    for (uint i = 0; i < textCount; i++)
        if (&(fileDialog[i]) == sender) {
            QFile f(file);
            if (!f.open(QFile::ReadOnly | QFile::Text))
                return ;
            QTextStream in(&f);
            textPrime[i].setPlainText(in.readAll());
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}
