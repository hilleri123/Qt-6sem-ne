#include "box.h"

Box::Box(QWidget *parent) : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);
    layout->addWidget(&label);
//    setChar('_', 0);
}


void Box::setChar(char c, uint id) {
    _c = c;
    _id = id;
    label.setText(QString(_c));
    QPalette pal = palette();
//    qDebug() << "color" << _id << color().value(_id);
    QColor tmp_color;
    if (color().contains(_id))
        tmp_color = color().value(_id);
    pal.setColor(QPalette::Background, tmp_color);
    setAutoFillBackground(true);
    setPalette(pal);
    update();
    show();
}

void Box::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.drawRect(0,0,width()-1, height()-1);

    QWidget::paintEvent(e);
}


QMap<uint, QColor> Box::color() {
    QMap<uint, QColor> res;
//    res.insert(0, QColor(1,0,0));
//    res.insert(1, QColor(0,1,0));
//    res.insert(2, QColor(0,0,1));
    for (uint i = 1; i <= 0x8; i++) {
//        qDebug() << "color" << (i-1) << QColor((i & 0x4) >> 2, (i & 0x2) >> 1, i & 0x1);
        res.insert(i-1, QColor(255*((i & 0x4) >> 2), 255*((i & 0x2) >> 1), 255*(i & 0x1)));
    }
    qDebug() << res;
    return res;
}
