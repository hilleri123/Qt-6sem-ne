#include "consumer.h"



Consumer::Consumer(QPlainTextEdit * t,  Buffer* buf, uint id, uint size, uint sleep, QObject *parent) : QThread(parent), text(t), _buf(buf), _id(id), _size(size), _sleep(sleep)
{
    connect(this, SIGNAL(addText(QString)), text, SLOT(insertPlainText(QString)));
}

void Consumer::run() {
    char c = ' ';
    QString tmp;
    uint size = _size;
    while (size > 0) {
//        qDebug() << "prod" << *i;
        QSemaphore& free = _buf->getFree();
        QSemaphore& used = _buf->getUsed();
        qDebug() << QString("pre cons free %1 used %2").arg(free.available()).arg(used.available());
//        used.acquire();
        if (_buf->getChar(c, _id)) {
//            if (c == '\n') {
//                emit addText(tmp);
//                tmp = "";
//            } else
//                tmp += c;
            emit addText(QString(c));
//            text->moveCursor (QTextCursor::End);
//            text->insertPlainText (QString(c));
//            text->moveCursor (QTextCursor::End);
            size--;
//            free.release();
        } else {
//            used.release();
        }
        qDebug() << QString("cons %1(%2) free %3 used %4 of %5").arg(QString(c)).arg(_id).arg(free.available()).arg(used.available()).arg(size);
        QThread::msleep(_sleep);
    }
    emit addText(tmp);
}
