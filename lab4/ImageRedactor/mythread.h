#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>

class Thread : public QObject
{
    Q_OBJECT

public:
    explicit Thread(QSize, std::size_t, const QImage&, QThread*, bool net = false, QVector<QVector<int>> const* ptr = nullptr);
    ~Thread();
signals:
    void change(std::size_t);
    void result(QSize, QImage);


public slots:
    void run();
    void work();
    void to_end();
protected:
    std::size_t x = 0;
    std::size_t y = 0;
    bool ended = false;
    QSize _pos;
    std::size_t _r;
    QImage _img;
    QImage img;

    bool net = false;
    QVector<QVector<int>> const * matrix;

    QTimer timer;
};

#endif // THREAD_H
