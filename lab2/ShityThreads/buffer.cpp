#include "buffer.h"
#include "ui_buffer.h"

Buffer::Buffer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::buffer)
{
    ui->setupUi(this);
//    QGridLayout* layout = new QGridLayout(this);
    setLayout(&layout);
//    setNumber(5);
}


void Buffer::setNumber(int num) {
    if (num <= 0)
        return ;
    qDebug() << "setNumber" << num;
    while (boxes.size() != num) {
//        qDebug() << "size" << boxes.size();
        if (boxes.size() > num) {
            Box* box = boxes.takeLast();
//            qDebug() << "remove";
            layout.removeWidget(static_cast<QWidget*>(box));
//            layout()->removeWidget(static_cast<QWidget*>(box));
            delete box;
        } else {
//            qDebug() << "new";
            Box* box = new Box();
//            qDebug() << "append";
            boxes.append(box);
//            qDebug() << "add";
//            layout()->addWidget(box);
            int clmn = (boxes.size()-1) % 5;
            int row = (boxes.size()-1) / 5;
//            qDebug() << "add" << row << clmn;
            layout.addWidget(box, row, clmn);
        }
    }
    _free.acquire(_free.available());
    _free.release(num);
    _used.acquire(_used.available());
    qDebug() << QString("free %1 used %2").arg(_free.available()).arg(_used.available());
}


void Buffer::setChar(char c, uint id) {
    mutex_prod.lock();
//    qDebug() << "setChar" << c;
    _free.acquire();
    QChar tmp_c(c);
    if (tmp_c.isLower())
        c = tmp_c.toUpper().toLatin1();
    else
        c = tmp_c.toLower().toLatin1();
    boxes.at(prod_pos % boxes.size())->setChar(c, id);
    _used.release();
    prod_pos++;
    mutex_prod.unlock();
}

bool Buffer::getChar(char& c, uint id) {
    if (boxes.at(cons_pos % boxes.size())->getId() != id)
        return false;
    mutex_cons.lock();
    _used.acquire();
    c = boxes.at(cons_pos % boxes.size())->getChar();
    _free.release();
    qDebug() << "getChar" << c;
    cons_pos++;
    mutex_cons.unlock();
    return true;
}


Buffer::~Buffer()
{
    for (auto i = boxes.begin(); i != boxes.end(); i++)
        delete *i;
    delete ui;
}
