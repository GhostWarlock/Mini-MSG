
#ifndef MINI_MSG_LOGINWINDOW_H
#define MINI_MSG_LOGINWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QWidgetAction>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

#define radiusSize 10

/* login status */
typedef enum
{
    ONLINE = 0,
    ACTIVE,
    AWAY,
    BUSY,
    NOT_DISTURB,
    HIDE,
    OFFLINE
}loginState;
typedef std::vector<QPixmap> ImageVector;
typedef std::vector<bool>    BoolVector;

typedef struct {
    QStringList usrNameList;
    QString pwd;
    ImageVector headVector;
    BoolVector  rememberVector;
    bool isAutoLogin;
    loginState state;
}loginDataGroup;

class LoginWindow : public QWidget {
Q_OBJECT

public:
    explicit LoginWindow(QWidget *p = nullptr, loginDataGroup data = {
            QStringList(""),"",
            ImageVector(),
            BoolVector(), false,ONLINE});

    ~LoginWindow() override;

signals:

    void loginRequest();                 // request login
    void usrInfoQuery(QString usrName);
    void exitLoginWindow();

public slots:
    void onUserNameChanged(const QString &);
    void pressLoginButton();
    void pressStateButton();


private:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;
    void timerEvent(QTimerEvent *) override;

    void initControls();
    void initBack();
    void loadStyleSheet(const QString &sheetName);

private:
    QWidget *parent = nullptr;
    Ui::LoginWindow *ui = nullptr;
    QMovie* backMovie = nullptr;
    QPoint lastPos;
    bool isPressedWidget = false;
    loginDataGroup loginData;
    loginDataGroup accountsData;

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

    int usrIndex = 0;
    QPixmap curHead;
    int timerID = 0;
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
