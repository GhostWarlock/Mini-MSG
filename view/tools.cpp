//
// Created by wusong on 2021/2/6.
//

#include "tools.h"

#include <QBitmap>
#include <QPainter>

QPixmap tool::pixmapToRound(const QPixmap &pix, int diameter)
{
    if (pix.isNull()) {
        return QPixmap();
    }
    if(diameter < 1)
        diameter = pix.width() > pix.height() ? pix.height() : pix.width();
    QBitmap mask(diameter,diameter);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, diameter, diameter, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, diameter, diameter, 100, 100);
    QPixmap image = pix.scaled(diameter,diameter,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    image.setMask(mask);
    return image;
}

QPixmap tool::setPixMapRotate(QPixmap &pix, int rotateNumber ){
    int imageWidth = pix.width();
    int imageHeight = pix .height();
    QPixmap temp(pix.size());
    temp.fill(Qt::transparent);
    QPainter  painter(&temp);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(imageWidth / 2, imageHeight / 2);       // set center
    painter.rotate(rotateNumber); //顺时针旋转90度
    painter.translate(-(imageWidth / 2), -(imageHeight / 2)); //使原点复原
    painter.drawPixmap(0, 0,   pix);
    painter.end();
    return temp;
}