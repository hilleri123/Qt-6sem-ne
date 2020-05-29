#include "fileplugin.h"


//FilePlugin::FilePlugin(QString f, int s, int a)
//    : filter(f), size(s), age(a)
//{}
QFileInfoList FilePlugin::findAndFilter(const QDir& dir, const QStringList& filter, int const* age, int const* size) {
//    QDir dir = in_dir;
//    QFileInfoList res;
    if (age != nullptr) {
        _age = *age;
        age = &_age;
    }
    if (size != nullptr) {
        _size = *size;
        size = &_size;
    }
    find(dir, filter, age, size);
    QFileInfoList res;
//    mutex.lock();
//    for (auto i = main_list.begin(); i != main_list.end(); i++) {
//        res += (*i)->future().results();
//    }
//    mutex.unlock();
    return res;
}


void FilePlugin::find(const QDir& dir, const QStringList& filter, int const* age, int const* size) {
    QFileInfoList files = dir.entryInfoList(filter, QDir::Files);

//    qDebug() << "find";
    auto func = [age, size] (const QFileInfo& f) {
        if (age != nullptr) {
            QDateTime time = f.birthTime();
            int ageOfFile = static_cast<int>(time.daysTo(QDateTime::currentDateTime()));
//            qDebug() << "age" << ageOfFile << (*age);
            if (*age > 0) {
                if (*age >= ageOfFile)
                    return false;
            } else if (*age < 0) {
                if (*age >= -ageOfFile)
                    return false;
            }
        }
        if (size != nullptr) {
            int sizeOfFile = static_cast<int>(f.size()/1024);
            if (*size > 0) {
                if (*size >= sizeOfFile)
                    return false;
            } else if (*size < 0) {
                if (*size >= -sizeOfFile)
                    return false;
            }
        }
        return true;
    };

//    QFuture<QFileInfo> future = QtConcurrent::filtered(files, func);
//    QFutureWatcher<QFileInfo> watcher;
//    watcher.setFuture(future);

    mutex.lock();
//    main_list.append(QFutureWatcher<QFileInfo>());
//    main_list.emplace_back();
    QFuture<QFileInfo> future = QtConcurrent::filtered(files, func);
//    main_list.append(QtConcurrent::filtered(files, func));
    QFutureWatcher<QFileInfo>* watcher = new QFutureWatcher<QFileInfo>;
    watcher->setFuture(future);
    main_list.append(watcher);
    connect(watcher, SIGNAL(finished()), this, SLOT(finished()));
//    main_list.back().setFuture(future);
    mutex.unlock();


    QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
//    qDebug() << subdirs;
    for(auto &subdir : subdirs) {
//        qDebug() << subdir.absolutePath();
        QDir tmp_dir(subdir.absolutePath()+"/"+subdir.fileName());
        files += findAndFilter(tmp_dir, filter, age, size);
    }
//    return files;
}


void FilePlugin::finished() {
//    qDebug() << "finish";
    QFutureWatcher<QFileInfo>* watcher = reinterpret_cast<QFutureWatcher<QFileInfo>*>(QObject::sender());
//    QFutureWatcher<QFileInfo>* watcher = qobject_cast<QFutureWatcher<QFileInfo>*>(QObject::sender());
    emit foundFiles(watcher->future().results());
}


void FilePlugin::cancelFuture() {
    mutex.lock();
    for (auto i = main_list.begin(); i != main_list.end(); i++) {
        (*i)->future().cancel();
        (*i)->cancel();
    }
    mutex.unlock();
}

//void FilePlugin::cancelFutureWatcher() {
//    mutex.lock();
//    for (auto i = main_list.begin(); i != main_list.end(); i++) {
//        i->cancel();
//    }
//    mutex.unlock();
//}


FilePlugin::~FilePlugin() {
    mutex.lock();
    for (auto i = main_list.begin(); i != main_list.end(); i++) {
        delete *i;
    }
    mutex.unlock();
}


