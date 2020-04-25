#ifndef CONSUMER_H
#define CONSUMER_H

#include <array>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <utility>

#include <QDebug>
#include <QPlainTextEdit>
#include <QThread>

#include "buffer.h"


class Consumer : public QThread
{
    Q_OBJECT
public:
    Consumer(QPlainTextEdit *, Buffer*, uint, uint, uint, QObject *parent = nullptr);
    void run() override;

signals:
    void addText(QString);

protected:
    QPlainTextEdit * text;
    Buffer* _buf;
    uint _id;
    uint _size;
    uint _sleep;
};


#endif // CONSUMER_H
