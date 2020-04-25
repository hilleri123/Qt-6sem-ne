#include "producer.h"


Producer::Producer(QPlainTextEdit const * t,  Buffer* buf, uint id, uint sleep, QObject *parent) : QThread(parent), text(t), _buf(buf), _id(id), _sleep(sleep)
{}

void Producer::run() {
    QString cur_text = text->toPlainText();
    for (auto i = cur_text.begin(); i != cur_text.end(); i++) {
        QSemaphore& free = _buf->getFree();
        QSemaphore& used = _buf->getUsed();
//        if ((*i).toLatin1() == 0)
//            break;
//        free.acquire();
        _buf->setChar(i->toLatin1(), _id);
//        used.release();
        qDebug() << QString("prod %1(%2) free %3 used %4 of %5").arg(QString(*i)).arg(_id).arg(free.available()).arg(used.available()).arg(i-cur_text.begin());
        QThread::msleep(_sleep);
    }
}
