//
// Created by wusong on 2021/2/6.
//

#ifndef MINI_MSG_TOOLS_H
#define MINI_MSG_TOOLS_H

#include <QPixmap>



class tool {
public:
    static QPixmap pixmapToRound(const QPixmap &src, int diameter = 0);
    static QPixmap setPixMapRotate(QPixmap &pix, int rotateNumber=0);
};


#endif //MINI_MSG_TOOLS_H
