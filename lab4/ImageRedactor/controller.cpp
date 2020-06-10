#include "controller.h"

Controller::Controller()
{
    qRegisterMetaType<QImage>("QImage");
    qRegisterMetaType<std::size_t>("std::size_t");
}


void Controller::set_image(QString url)
{
    qDebug() << url;
//    QString name = url.mid(7);
    QString name = url;
    qDebug() << name;
//    _image = QPixmap(name);
    _image = QImage(name);
    qDebug() << _image.isNull();
    _pix = 0;
    emit progress();
    emit act();
}

const QImage& Controller::get_result()
{
    qDebug() << "result";
//    return _result;
    return _res;
}

void Controller::save_result(QString name)
{
    if (!name.contains(".jpg")&&!name.contains(".png")) {
        name += ".jpg";
    }
    qDebug() << "save" << name << "'" << name << "'" << _res.save(name);
}


void Controller::start(int threads, int radius, bool net, QVector<QVector<int>> mat)
{
    matrix = mat;
    _threads = static_cast<std::size_t>(threads);
    _radius = static_cast<std::size_t>(radius);
    net = net;
    qDebug() << "start" << _threads << _radius;
    _timer.start();
    _pix = 0;
//    _radius = 100;
    qDebug() << _radius << _image.width() << _image.height();
    QImage tmp(_image.width()+2*_radius, _image.height()+2*_radius, QImage::Format_RGB666);

    _res = QImage(_image.width(), _image.height(), QImage::Format_RGB666);
    _res.fill(Qt::blue);
    std::size_t tmp_x = 0, tmp_y = 0;
    for (std::size_t x = 0; x < tmp.width(); x++) {
        for (std::size_t y = 0; y < tmp.height(); y++) {
            if (x < _radius)
                tmp_x = 0;
            else if (x >= _radius + _image.width())
                tmp_x = _image.width()-1;
            else
                tmp_x = x - _radius;
            if (y < _radius)
                tmp_x = 0;
            else if (y >= _radius + _image.height())
                tmp_y = _image.height()-1;
            else
                tmp_y = y - _radius;
            tmp.setPixelColor(x, y, _image.pixelColor(tmp_x, tmp_y));
        }
    }
//    _res = tmp;
//    emit end();
    Thread* thread;

    std::size_t split = 2;

    std::size_t _w = _image.width() / split;
    double del = 1;
    if (_threads / split > 0)
        del = _threads / split;
    if (_threads & 1 && _threads != 1)
        del += 0.5;
    std::size_t _h = _image.height() / del;
    _flag = _threads;
    qDebug() << "flag" << _flag << _w << _h;
    for (std::size_t id = 0; id < _threads / split; id++) {
        for (std::size_t num = 0; num < split; num++) {
            workers.append(new QThread());
            std::size_t w = _w * ((num * _w) % _image.width() / _w) + _radius;
            std::size_t h = _h * ((id * _h) % _image.height() / _h) + _radius;
            thread = new Thread(QSize(w-_radius, h-_radius), _radius,
                                tmp.copy(w-_radius, h-_radius, _w%(tmp.width()-w)+2*_radius, _h%(tmp.height()-h)+2*_radius), workers.back(), net, &matrix);
            thread->moveToThread(workers.back());
            connect(thread, SIGNAL(result(QSize,QImage)), this, SLOT(add_res(QSize, QImage)));
            connect(thread, SIGNAL(change(std::size_t)), this, SLOT(update(std::size_t)));
            connect(this, SIGNAL(end_proc()), thread, SLOT(to_end()));
            connect(this, SIGNAL(run()), thread, SLOT(run()));
            workers.back()->start();
        }
    }
    if (_threads & 1) {
//        std::size_t w = _radius;
        workers.append(new QThread());
        std::size_t w = 0;
        std::size_t h = _h * ((_threads - 1) / 2) + _radius;
        thread = new Thread(QSize(w, h-_radius), _radius,
                            tmp.copy(0, h-_radius, tmp.width()-1+2*_radius, (tmp.height()-h)+2*_radius-1), workers.back(), net, &matrix);
        thread->moveToThread(workers.back());
        connect(thread, SIGNAL(result(QSize,QImage)), this, SLOT(add_res(QSize, QImage)));
        connect(thread, SIGNAL(change(std::size_t)), this, SLOT(update(std::size_t)));
        connect(this, SIGNAL(end_proc()), thread, SLOT(to_end()));
        connect(this, SIGNAL(run()), thread, SLOT(run()));
        workers.back()->start();
    }
    emit run();
}

void Controller::add_res(QSize pos, QImage in)
{
    qDebug() << "add res" << pos << in.size() << _flag;
    for (std::size_t x = 0; x < in.width(); x++) {
        for (std::size_t y = 0; y < in.height(); y++) {
            _res.setPixelColor(pos.width() + x, pos.height() + y, in.pixelColor(x, y));
        }
    }
    _flag--;
    if (!_flag) {
//        for (auto i = workers.begin(); i != workers.end(); i++) {
//            qDebug() << "quit" << (*i);
//            (*i)->quit();
//            (*i)->wait();
//            delete *i;
//        }
        stop();
    }

}

void Controller::stop()
{
    emit progress();
    emit end();
}



void Controller::bre()
{
    emit end_proc();
//    workers.clear();
//    stop();
}

QString Controller::get_pixels()
{
//    qDebug() << "get pixels";
    if (_image.isNull()) {
        return "0";
    } else {
        return QString::number(_pix);
    }
}

//void Controller::settings(int threads, int radius)
//{
//    _threads = static_cast<std::size_t>(threads);
//    _radius = static_cast<std::size_t>(radius);
//}



QString Controller::get_time()
{
//    qDebug() << "get time";
    int m_sec = _timer.elapsed();
    int sec = m_sec / 1000;
    int minute = sec / 60;
    int hour = minute / 60;

    m_sec = m_sec % 1000;
    sec = sec % 60;
    minute = minute % 60;
    hour = hour % 24;
    QTime& time = *new QTime(hour, minute, sec, m_sec);
    return time.toString("mm:ss:zzz");
}


int Controller::get_max()
{
    if (_image.isNull()) {
        return 0;
    } else {
        return _image.size().height()*_image.size().width();
    }
}

void Controller::update(std::size_t pixels)
{
//    std::this_thread::sleep_for(std::chrono::milliseconds(200));
//    qDebug() << pixels;
    _pix+=pixels;
    emit progress();
//    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    if (get_max() == 1) {
//        emit end();
    }
}




Controller::~Controller()
{
    for (auto i = workers.begin(); i != workers.end(); i++) {
        (*i)->quit();
        (*i)->wait();
        delete *i;
    }
}

