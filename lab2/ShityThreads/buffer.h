#ifndef BUFFER_H
#define BUFFER_H

#include <QWidget>
#include <QList>
#include <QSemaphore>
#include <QDebug>
#include <QMutex>

#include "box.h"

namespace Ui {
class buffer;
}

class Buffer : public QWidget
{
    Q_OBJECT

public:
    explicit Buffer(QWidget *parent = nullptr);
    ~Buffer();

    void setNumber(int);

    void setChar(char, uint);
    bool getChar(char&, uint);

    QSemaphore& getFree() { return _free; }
    QSemaphore& getUsed() { return _used; }

private:
    Ui::buffer *ui;

    QSemaphore _free;
    QSemaphore _used;

    QGridLayout layout;

    QMutex mutex_prod, mutex_cons;
    int prod_pos = 0;
    int cons_pos = 0;

    QList<Box*> boxes;
};

#endif // BUFFER_H
