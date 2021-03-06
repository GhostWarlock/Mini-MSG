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

#ifdef Q_OS_UNIX

int tool::makePath(std::string path, unsigned int mode) {
    int res;
    size_t pre = 0,pos = 0;
    std::string dir;
    if(path[path.size()-1]!='/'){
        path+='/';
    }

    while((pos=path.find_first_of('/',pre))!=std::string::npos){
        dir=path.substr(0,pos++);
        pre=pos;
        if(dir.empty()) continue;
        if((res=::mkdir(dir.c_str(),mode)) && errno!=EEXIST){
            return res;
        }
    }
    return (errno == EEXIST) ? 0 : res;
}
#endif

#ifdef Q_OS_WIN
int tool::makePath(std::string path, unsigned int mode) {
    ;
}
#endif