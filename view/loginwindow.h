
#ifndef MINI_MSG_LOGINWINDOW_H
#define MINI_MSG_LOGINWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QWidgetAction>
#include <QSystemTrayIcon>

#include "model/dataType.h"
#include "subWidget/accountBox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; }
QT_END_NAMESPACE

#define radiusSize 10

class loginWindow : public QWidget {
Q_OBJECT

public:
    explicit loginWindow(QWidget *p = nullptr, accountDataGroup data = {IntVector(),
            StringVector(),
            StringVector(),"",
            ImageVector(),
            BoolVector(), false,
            EnumVector(ONLINE)});

    ~loginWindow() override;

signals:
    void sigLoginRequest(const accountInfo &);    // request login
    void sigCancelLogin();
    void sigExitLoginWindow();
    void sigDeleteAccount(const int &);

public slots:
    void onUserNameChanged(const QString &);
    void onPressLoginButton();
    void onPressStateButton();

    void onLoginState(const QString&);


private:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;
    void timerEvent(QTimerEvent *) override;

    void initControls();
    void disconnectSigSlots();
    void initBack();
    void loadStyleSheet(const QString &sheetName);

private:
    QWidget *parent = nullptr;
    Ui::loginWindow *ui = nullptr;
    accountBox* pAccountBox = nullptr;
    QMovie* backMovie = nullptr;
    QPoint lastPos;
    bool isPressedWidget = false;
    accountInfo loginData;
    accountDataGroup accountsData;

    QMenu * stateMenu = nullptr;
    QAction * acOnline = nullptr;
    QAction * acActive = nullptr;
    QAction * acAway = nullptr;
    QAction * acBusy = nullptr;
    QAction * acNotDisturb = nullptr;
    QAction * acHide = nullptr;

    QToolButton* pwdHide = nullptr;
    QWidgetAction* enablePwdHide = nullptr;

    QSystemTrayIcon* sysTrayIcon = nullptr;

    QPixmap curHead;
//    loginState curState;
    QString curUserName;
    int curId = -1;
    int autoLoginDelayTimer = -1;
    int timerID = -1;
    int angle = 0;
    loginState curState = ONLINE;
    std::vector<QString> tips = {"在线",
                                 "想聊天",
                                 "离开",
                                 "忙碌",
                                 "请勿打扰",
                                 "隐身",
                                 "离线"};
};

#endif //MINI_MSG_LOGINWINDOW_H
