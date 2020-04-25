#ifndef BOX_H
#define BOX_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QDebug>
#include <QPalette>
#include <QColor>
#include <QMap>
#include <QPainter>

class Box : public QWidget
{
    Q_OBJECT
public:
    explicit Box(QWidget *parent = nullptr);

    void setChar(char c, uint id);
    char getChar() const { return _c; }
    uint getId() const { return _id; }


    void paintEvent(QPaintEvent *e) override;

protected:
    QLabel label;
    char _c;
    uint _id = 255;

    static QMap<uint, QColor> color();
};

#endif // BOX_H
