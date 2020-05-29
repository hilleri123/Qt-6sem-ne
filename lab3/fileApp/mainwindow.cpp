#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    select_dir.setFileMode(QFileDialog::Directory);
    run.setText("Run");
    run.setEnabled(false);
    select.setText("Select dir");
    plug.setText("Select plugin");
    select_plug.setNameFilter("*.so *.dll");
    cd(QDir::homePath());
    text.setReadOnly(true);
    use_s.setText("Size in Kb");
    use_a.setText("Days old");
    c_age.setMaximum(100000);
    c_size.setMaximum(1000000);
    QStringList opt = QStringList() << ">" << "<";
    more_s.addItems(opt);
    more_a.addItems(opt);
    filter_label.setText("filter");

    QWidget *w = new QWidget(this);
    setCentralWidget(w);
    QVBoxLayout *v_l = new QVBoxLayout(w);
    w->setLayout(v_l);
    QHBoxLayout *h_l = new QHBoxLayout();
    v_l->addLayout(h_l);
    h_l->addWidget(&dir);
    h_l->addWidget(&select);

    h_l = new QHBoxLayout();
    v_l->addLayout(h_l);
    h_l->addWidget(&run);
    h_l->addWidget(&plug);
    h_l->addWidget(&count);

    h_l = new QHBoxLayout();
    v_l->addLayout(h_l);
    h_l->addWidget(&filter_label);
    h_l->addWidget(&filter);

    h_l = new QHBoxLayout();
    v_l->addLayout(h_l);
    h_l->addWidget(&use_a);
    h_l->addWidget(&more_a);
    h_l->addWidget(&c_age);

    h_l = new QHBoxLayout();
    v_l->addLayout(h_l);
    h_l->addWidget(&use_s);
    h_l->addWidget(&more_s);
    h_l->addWidget(&c_size);

    v_l->addWidget(&text);

    connect(&run, SIGNAL(clicked()), this, SLOT(runFindAndFilter()));

    connect(&select, SIGNAL(clicked()), &select_dir, SLOT(exec()));
    connect(&select_dir, SIGNAL(fileSelected(QString)), this, SLOT(cd(QString)));
    connect(&dir, SIGNAL(currentTextChanged(QString)), this, SLOT(cd(QString)));

    connect(&plug, SIGNAL(clicked()), &select_plug, SLOT(exec()));
    connect(&select_plug, SIGNAL(fileSelected(QString)), this, SLOT(loadPlugin(QString)));

}

bool MainWindow::cd(QString path)
{
//    qDebug() << "cd" << path;
    bool res = curr.cd(path);
//    qDebug() << res << curr.absolutePath();
    disconnect(&dir, SIGNAL(currentTextChanged(QString)), this, SLOT(cd(QString)));
    dir.clear();
//    dir.addItem(curr.absolutePath());
    QFileInfoList list = curr.entryInfoList(QDir::Dirs);
//    qDebug() << list;
    for(auto i = list.begin(); i != list.end(); i++)
        dir.addItem(i->filePath());
    connect(&dir, SIGNAL(currentTextChanged(QString)), this, SLOT(cd(QString)));
    return res;
}



void MainWindow::runFindAndFilter()
{
    QStringList filters;
    if (filter.text().isEmpty())
        filters << "*";
    else
        filters << filter.text();
    int* p_age = nullptr;
    int* p_size = nullptr;
    int age = c_age.value();
    int size = c_size.value();
    if (plugin) {
        files = 0;
        text.clear();
        if (filters.length())
        if (use_a.checkState())
            p_age = &age;
        if (use_s.checkState())
            p_size = &size;
        if (more_a.currentText() == "<")
            age *= -1;
        if (more_s.currentText() == "<")
            size *= -1;
        qDebug() << "run" << curr << filters << age << size;
        plugin->findAndFilter(curr, filters, p_age, p_size);
    }
}


void MainWindow::appendFiles(QFileInfoList list)
{
    files += list.size();
    count.setText("count files "+QString::number(files));
    for (auto i = list.begin(); i != list.end(); i++) {
        text.append(i->absolutePath()+"/"+i->fileName()+"  "+i->birthTime().toString()+"  "+QString::number(i->size()/1024));
    }
}


bool MainWindow::loadPlugin(QString path)
{
    setWindowTitle(path);
    qDebug() << path;
    QPluginLoader loader(path);
    if(!loader.load())
        qDebug() << "Error:" << loader.fileName() << loader.errorString();
    plugin = qobject_cast<Plugin*>(loader.instance());
    if (plugin){
        qDebug() << "Loaded: " << loader.fileName();
        connect(plugin, SIGNAL(foundFiles(QFileInfoList)),this,SLOT(appendFiles(QFileInfoList)));
//        connect(plugin, SIGNAL(emitCancel()), this, SLOT(stopButtonActivate()));
//        ui->choosePathButton->setEnabled(true);
        qDebug() << "Done";
        run.setEnabled(true);
        return true;
    }
    return false;
}


MainWindow::~MainWindow()
{
    delete ui;
}
