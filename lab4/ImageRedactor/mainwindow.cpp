#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _start.setText("start");
    _chose.setText("choose");
    _stop.setText("stop");
    _save.setText("save");
    _matrix.setText("modify matrix");


    QWidget* w = new QWidget;
    setCentralWidget(w);
    QVBoxLayout* vl = new QVBoxLayout;
    w->setLayout(vl);
    QHBoxLayout* hl = new QHBoxLayout;
    vl->addLayout(hl);
    hl->addWidget(&_img1);
    hl->addWidget(&_img2);
    hl = new QHBoxLayout;
    vl->addLayout(hl);
    hl->addWidget(&_progress);
    hl->addWidget(&_file);

    hl = new QHBoxLayout;
    vl->addLayout(hl);
    QLabel* label = new QLabel("pixels");
    hl->addWidget(label);
    hl->addWidget(&_bytes);
    label = new QLabel("time");
    hl->addWidget(label);
    hl->addWidget(&_time);


    hl = new QHBoxLayout;
    vl->addLayout(hl);
    label = new QLabel("threads");
    hl->addWidget(label);
    hl->addWidget(&_threads);
    label = new QLabel("radius");
    hl->addWidget(label);
    hl->addWidget(&_radius);
    _net.setText("net");
    hl->addWidget(&_net);


    hl = new QHBoxLayout;
    vl->addLayout(hl);
    hl->addWidget(&_chose);
    hl->addWidget(&_start);
    hl->addWidget(&_stop);
    hl->addWidget(&_matrix);
    hl->addWidget(&_save);


    connect(&_chose, SIGNAL(clicked()), _dialog1, SLOT(exec()));
    connect(&_save, SIGNAL(clicked()), _dialog2, SLOT(exec()));
    connect(&_start, SIGNAL(clicked()), this, SLOT(start()));
    connect(&_matrix, SIGNAL(clicked()), this, SLOT(matrix()));

    _c = new Controller();

    connect(&_stop, SIGNAL(clicked()), _c, SLOT(bre()));

    connect(_c, SIGNAL(progress()), this, SLOT(update()));
    _dialog1->setAcceptMode(QFileDialog::AcceptOpen);
    _dialog2->setAcceptMode(QFileDialog::AcceptSave);
    connect(_dialog1, SIGNAL(fileSelected(QString)), &_file, SLOT(setText(QString)));
    connect(_dialog1, SIGNAL(fileSelected(QString)), _c, SLOT(set_image(QString)));
    connect(_dialog2, SIGNAL(fileSelected(QString)), _c, SLOT(save_result(QString)));
    connect(_dialog1, SIGNAL(fileSelected(QString)), this, SLOT(set(QString)));
    connect(_c, SIGNAL(end()), this, SLOT(end()));
    _file.setReadOnly(true);
    _start.setEnabled(false);
    _stop.setEnabled(false);
    _save.setEnabled(false);
    _dialog1->setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    _dialog2->setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

    _threads.setMinimum(1);
    _radius.setMinimum(1);

}



MainWindow::~MainWindow()
{
    delete ui;
    delete _c;
}

void MainWindow::start()
{
//    _c->settings(_threads.value(), _radius.value());
    _stop.setEnabled(true);
    _start.setEnabled(false);
    if (mat_rad != _radius.value()) {
        mat.clear();
        for (std::size_t y = 0; y < 2*_radius.value()+1; y++) {
            mat.append(QVector<int>());
            for (std::size_t x = 0; x < 2*_radius.value()+1; x++) {
                if ((x+y) & 1)
                    mat.back().append(1);
                else
                    mat.back().append(-1);
            }
        }
    }

    _c->start(_threads.value(), _radius.value(), _net.isChecked(), mat);
}

void MainWindow::update()
{
//    qDebug() << "update";
    _bytes.setText(_c->get_pixels());
    _time.setText(_c->get_time());
    _progress.setMaximum(_c->get_max());
    _progress.setValue(_c->get_pixels().toInt());
}


void MainWindow::end()
{
    _stop.setEnabled(false);
    _start.setEnabled(true);
//    qDebug() << "second";
    _img2.setPixmap(QPixmap::fromImage(_c->get_result()));
    _save.setEnabled(true);
}


void MainWindow::set(QString a)
{
    _img1.setPixmap(QPixmap::fromImage(QImage(a)));
    _start.setEnabled(true);
    _save.setEnabled(false);
}



void MainWindow::matrix()
{
    int r = _radius.value();
    QDialog d;

    QVBoxLayout* vl = new QVBoxLayout;
    d.setLayout(vl);
    QHBoxLayout* hl;
    QVector<QVector<QSpinBox*>> spins;
    for (std::size_t y = 0; y < 2*r+1; y++) {
        spins.append(QVector<QSpinBox*>());
        hl = new QHBoxLayout;
        vl->addLayout(hl);
        for (std::size_t x = 0; x < 2*r+1; x++) {
            spins.back().append(new QSpinBox());
            spins.back().back()->setMinimum(-100);
            if (mat_rad == r) {
                spins.back().back()->setValue(mat.at(y).at(x));
            } else {
                if ((x+y) & 1)
                    spins.back().back()->setValue(1);
                else
                    spins.back().back()->setValue(-1);
            }
            hl->addWidget(spins.back().back());
        }
    }
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

    hl = new QHBoxLayout;
    vl->addLayout(hl);
    hl->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &d, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &d, &QDialog::reject);


    int res = d.exec();
    if (res == QDialog::Accepted) {
        mat.clear();
        for (std::size_t y = 0; y < 2*r+1; y++) {
            mat.append(QVector<int>());
            for (std::size_t x = 0; x < 2*r+1; x++) {
                mat.back().append(spins.at(y).at(x)->value());
            }
        }
        mat_rad = r;
        for (std::size_t y = 0; y < 2*r+1; y++) {
            qDebug() << "|" << mat.at(y) << "|";
        }
    }
    for (auto i = spins.begin(); i != spins.end(); i++)
        for (auto j = i->begin(); j != i->end(); j++)
            delete *j;
}


