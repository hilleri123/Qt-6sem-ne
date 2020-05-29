#ifndef PLUGIN_H
#define PLUGIN_H

#pragma once

#include <QDebug>
#include <QString>
#include <QFileInfoList>
#include <QFuture>
#include <QFutureWatcher>
#include <QStringList>


#include <QObject>
#include <QtPlugin>



class Plugin : public QObject
{

    Q_OBJECT

public:
    Plugin() {}
    virtual ~Plugin(){}
//    virtual void findFilteredFiles(const QString &path,QStringList &formatFilter, bool &isAgeChecked, bool &isSizeChecked,int &ageRequired, bool &moreLess,int &sizeRequired) = 0;
    virtual QFileInfoList findAndFilter(const QDir& dir, const QStringList& filter, int const* age = nullptr, int const* size=nullptr) = 0;

    virtual QFileInfoList getFilteredFiles() = 0;
public slots:
    virtual void cancelFuture() = 0;
//    virtual void cancelFutureWatcher() = 0;

// ===== SIGNALS ===== //
signals:
    void foundFiles(QFileInfoList);
//    void emitCancel();
};

Q_DECLARE_INTERFACE(Plugin, "ru.mephi")

#endif // PLUGIN_H
