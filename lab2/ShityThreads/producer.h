#ifndef PRODUCER_H
#define PRODUCER_H


#include <QThread>

#include <QDebug>
#include <QPlainTextEdit>

#include "buffer.h"

class Producer : public QThread
{
public:
    Producer(QPlainTextEdit const *, Buffer*, uint, uint, QObject *parent = nullptr);
    void run() override;
protected:
    QPlainTextEdit const * text;
    Buffer* _buf;
    uint _id;
    uint _sleep;
};

#endif // PRODUCER_H
