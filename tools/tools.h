//
// Created by wusong on 2021/2/6.
//

#ifndef MINI_MSG_TOOLS_H
#define MINI_MSG_TOOLS_H

#include <QPixmap>
#include <string>
#ifdef Q_OS_UNIX
#include <sys/stat.h>
#include <cerrno>
#endif

class tool {
public:
    static QPixmap pixmapToRound(const QPixmap &src, int diameter = 0);
    static QPixmap setPixMapRotate(QPixmap &pix, int rotateNumber=0);
    static int makePath(std::string path, mode_t mode);
};


#endif //MINI_MSG_TOOLS_H
