#include "mythread.h"

Thread::Thread(QSize pos, std::size_t r, const QImage& img, QThread* ptr, bool net, QVector<QVector<int>> const *vec)
    : _pos(pos), _r(r), _img(img), img(img), net(net), matrix(vec)
{
    _img = img.copy(_r, _r, img.width()-2*_r, img.height()-2*_r);
    timer.moveToThread(ptr);
    timer.setInterval(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(work()));
    qDebug() << "IMAGE" << _pos << _img.size();
}

Thread::~Thread()
{}

void Thread::run()
{
    timer.start();
}


void Thread::work()
{
    std::size_t i = 0;
//    qDebug() << "aa" << _pos << (_x*img.height()+_y);
//    qDebug() << "img" << img.size();
    for (; x < _img.width(); x++) {
//        qDebug() << "s" << x << y;
        if (y >= _img.height())
            y = 0;
        for (; y < _img.height(); y++) {
            if (ended) {
                emit change(i);
                timer.stop();
                return ;
            }
            if (i > 200) {
                emit change(i);
                return ;
            }
            std::size_t r = 0;
            std::size_t g = 0;
            std::size_t b = 0;
            int k = 0;
            for (auto i = matrix->begin(); i != matrix->end(); i++)
                for (auto j = i->begin(); j != i->end(); j++)
                    k += *j;
            for (int ix = 0; ix < 2*_r+1; ix++) {
                for (int iy = 0; iy < 2*_r+1; iy++) {
                    r += matrix->at(iy).at(ix) * img.pixelColor(x+ix, y+iy).red() / k;
                    g += matrix->at(iy).at(ix) * img.pixelColor(x+ix, y+iy).green() / k;
                    b += matrix->at(iy).at(ix) * img.pixelColor(x+ix, y+iy).blue() / k;
                }
            }
            if (r > 255)
                r = 255;
            if (g > 255)
                g = 255;
            if (b > 255)
                b = 255;
            _img.setPixelColor(x, y, QColor(r, g, b));
//            _img.setPixelColor(x, y, color);
            i++;
        }
//        qDebug() << "f" << _x << _y;
    }
    emit change(i);
    i = 0;
    if (x == _img.width() && y == _img.height())
        to_end();
    else if (ended)
        timer.stop();
}


void Thread::to_end() {
    qDebug() << "net" << net;
    if (net) {
        QColor color(0, 0, 255);
        for (std::size_t x = 0; x < _img.width(); x++) {
            _img.setPixelColor(x, 0, color);
            _img.setPixelColor(x, _img.height()-1, color);
        }
        for (std::size_t y = 0; y < _img.height(); y++) {
            _img.setPixelColor(0, y, color);
            _img.setPixelColor(_img.width()-1, y, color);
        }
    }
    if (!ended)
        emit result(_pos, _img);
    ended = true;
    timer.stop();
//    this->~Thread();
}

