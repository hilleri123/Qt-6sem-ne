#pragma once

#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QtConcurrent>
#include <QDateTime>
#include <QList>
#include <QMutex>
#include <QPair>

#include <vector>

#include "myplugin_global.h"
#include <plugin.h>

class FilePlugin : public Plugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "ru.mephi")
    Q_INTERFACES(Plugin)

public:
    FilePlugin() {}
//    explicit FilePlugin(QString, int, int);

    ~FilePlugin() override;
//    void findFilteredFiles(const QString &path, QStringList &formatFilter, bool &isAgeChecked, bool &isSizeChecked, int &ageRequired, bool &moreLess,int &sizeRequired) override {}
    virtual QFileInfoList findAndFilter(const QDir& dir, const QStringList& filter, int const* age = nullptr, int const* size=nullptr) override;
    virtual void find(const QDir& dir, const QStringList& filter, int const* age = nullptr, int const* size=nullptr);

    QFileInfoList getFilteredFiles() override {}

public slots:
    void cancelFuture() override;
//    void cancelFutureWatcher() override;

//signals:
//    virtual void emitFile();
//    virtual void emitCancel();
private slots:
    void finished();



protected:
//    QString filter;
    int _size;
    int _age;

    QMutex mutex;
    QList<QFutureWatcher<QFileInfo>*> main_list;
//    QList<QFutureWatcher<QFileInfo>> main_list;
//    std::vector<QFutureWatcher<QFileInfo>> main_list;

//    QFuture<QFileInfoList> futureInfoList;
//    QFutureWatcher<QFileInfoList> watcherInfoList;
};
