#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QImage>
#include <QTime>
#include <QPixmap>
#include <utility>
#include <QDir>
#include <QThread>
#include <QTimer>

#include <thread>
#include <ios>
#include <iostream>
#include <cstdio>
#include <vector>
#include <math.h>

#include "mythread.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller();


    ~Controller();
signals:
    void progress();
    void act();
    void save();
    void end();
    void end_proc();
    void run();
public slots:
    void update(std::size_t);
    void start(int threads, int radius, bool net, QVector<QVector<int>>);
    void stop();
    void bre();

    void add_res(QSize, QImage);

    void save_result(QString);
    QString get_pixels();
    void set_image(QString);
    const QImage& get_result();
//    void settings(int threads, int radius);
    QString get_time();
    int get_max();
protected:

    QVector<QThread*> workers;

    QVector<QVector<int>> matrix;

    QImage _image;
    QImage _res;
    std::size_t _pix=0;
    std::size_t _threads=0;
    std::size_t _radius=0;
    QTime _timer;
    QString _result;
    bool net = false;
    std::size_t _flag = 0;
};

#endif // CONTROLLER_H
