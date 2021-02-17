
#include <QMouseEvent>
#include <QFile>
#include <QDebug>
#include <QMovie>
#include <QPainter>
#include <QIcon>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QBitmap>

#include "loginwindow.h"
#include "resources.h"
#include "tools/tools.h"
#include "./ui_loginwindow.h"

loginWindow::loginWindow(QWidget *p,accountDataGroup data)
        : QWidget(p)
        , parent(p)
        , ui(new Ui::loginWindow)
        , accountsData(std::move(data))
        , pAccountBox(new accountBox(this)) {
    ui->setupUi(this);
    pAccountBox->move(130, 190);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.92);
    initControls();          // init Controls
    initBack();

    loadStyleSheet(loginWindow_css);

    /* auto login */
    if (accountsData.isAutoLogin)
        autoLoginDelayTimer = startTimer(1000);
}

loginWindow::~loginWindow()
{
    delete ui;
    delete backMovie;
    delete stateMenu;
    delete acOnline;
    delete acActive;
    delete acAway;
    delete acBusy;
    delete acNotDisturb;
    delete acHide;
    delete pwdHide;
    delete enablePwdHide;
    delete sysTrayIcon;
}

void loginWindow::mousePressEvent(QMouseEvent *e) // 鼠标点击事件
{
    lastPos = e->globalPos();
    isPressedWidget = true;
}
void loginWindow::mouseMoveEvent(QMouseEvent *e) // 鼠标移动事件
{
    if(isPressedWidget){
        int xLen = e->globalX() - lastPos.x();
        int yLen = e->globalY() - lastPos.y();
        lastPos = e->globalPos();
        move(x()+xLen, y()+yLen);    // move widget
    }
}

void loginWindow::mouseReleaseEvent(QMouseEvent *e) {
    isPressedWidget = false;
}

void loginWindow::initControls() {
    /* min button */
    ui->min->setToolTip(tr("最小化"));
    ui->min->setIcon(QIcon(min_png));
    ui->min->setIconSize(QSize(25,25));

    /* close button */
    ui->close->setToolTip(tr("关闭"));
    ui->close->setIcon(QIcon(close_png));
    ui->close->setIconSize(QSize(25,25));

    /* help button */
    ui->help->setToolTip("帮助");
    ui->help->setIcon(QIcon(help_png));
    ui->help->setIconSize(QSize(15,15));

    /* addUsr button */
    ui->addUsr->setToolTip("添加新账户");
    ui->addUsr->setIcon(QIcon(addUsr_png));
    ui->addUsr->setIconSize(QSize(20,20));

    // init usrName pwd checkbox usrHead loginState
    /* user name */
    for(int i=0; i<accountsData.usrNameList.size(); ++i){
        pAccountBox->addAccount(new accountItem(accountsData.ids[i],accountsData.headVector[i],
                                                accountsData.usrNameList[i],
                                                accountsData.accountList[i],pAccountBox));
    }
    if(!accountsData.accountList.empty()){
        pAccountBox->setEditText(accountsData.accountList[0]);
    }
    if(!accountsData.usrNameList.empty()){
        curUserName = accountsData.usrNameList[0];
    }


    /* user head */
    if(!accountsData.headVector.empty()){
        curHead = tool::pixmapToRound(QPixmap(accountsData.headVector[0]),
                                      ui->usrHead->width());
    }
    else{
        curHead = tool::pixmapToRound(QPixmap(avatar_png),ui->usrHead->width());
    }
    ui->usrHead->setPixmap(curHead);
    ui->usrHead->installEventFilter(this);

    /* login status Icon */
    if(!accountsData.states.empty()) {
        curState = accountsData.states[0];
    }
    ui->loginState->setToolTip(tips[curState]);
    switch (curState){
        case ACTIVE:{
            ui->loginState->setIcon(QIcon(active_png));
        }break;
        case AWAY:{
            ui->loginState->setIcon(QIcon(away_png));
        }break;
        case BUSY:{
            ui->loginState->setIcon(QIcon(busy_png));
        }break;
        case NOT_DISTURB:{
            ui->loginState->setIcon(QIcon(notDisturb_png));
        }break;
        case HIDE:{
            ui->loginState->setIcon(QIcon(hide_png));
        }break;
        default:{
            ui->loginState->setIcon(QIcon(online_png));
        }
    }

    /* password  checkbox */
    pwdHide = new QToolButton(this);       // add right button
    pwdHide->setText("");
    pwdHide->setIcon(QIcon(showPwd_png));
    pwdHide->setCursor(Qt::PointingHandCursor);
    pwdHide->setObjectName("pwdHide");
    pwdHide->setCheckable(true);
    pwdHide->setToolTip("显示密码");
    enablePwdHide = new QWidgetAction(ui->pwd);
    enablePwdHide->setDefaultWidget(pwdHide);
    ui->pwd->addAction(enablePwdHide, QLineEdit::TrailingPosition);
    ui->pwd->setValidator(new QRegExpValidator(QRegExp("^[0-9a-zA-Z]{6,16}$"), this));
    ui->pwd->setEchoMode(QLineEdit::Password);
    ui->pwd->setMaxLength(16);
    ui->pwd->setFont(QFont("Timers",7));
    if((!accountsData.rememberVector.empty())
        && accountsData.rememberVector[0]){
        ui->remember->setCheckState(Qt::Checked);
        ui->pwd->setText(normalPwd);
    }
    else{
        ui->remember->setCheckState(Qt::Unchecked);
        ui->pwd->clear();
    }
    ui->autoLogin->setCheckState(accountsData.isAutoLogin ? Qt::Checked : Qt::Unchecked);
    /* login button */
    ui->login->setText(tLogin);
    /* connect SIGNAL and SLOT */
    connect(ui->min,&QToolButton::clicked,this,[&](){
#ifdef Q_OS_WIN
        if (sysTrayIcon == nullptr) {
            sysTrayIcon = new QSystemTrayIcon(QIcon(appIcon_png), this);
            sysTrayIcon->setToolTip("Mini-MSG:Login");
        }
        this->hide();
        sysTrayIcon->show();
#endif
#ifdef Q_OS_LINUX
//        QMessageBox::information(this, "Mini-MSG", "系统不支持最小化\t");
        this->showMinimized();
#endif
    });
    connect(ui->close,&QToolButton::clicked,[=](){
        disconnectSigSlots();
        if(ui->login->text() == tLogin) emit sigCancelLogin();
        emit sigExitLoginWindow();});
    connect(ui->loginState,SIGNAL(clicked()), this,SLOT(onPressStateButton()));
    connect(ui->addUsr,&QPushButton::clicked,[=](){
        ui->pwd->clear();
        ui->usrHead->setPixmap(QPixmap(avatar_png));
    });
    connect(pAccountBox,&QComboBox::editTextChanged,this, &loginWindow::onUserNameChanged);
    connect(pAccountBox,&accountBox::sigRemoveAccount,this,[&](const int &id){
        emit sigDeleteAccount(id);
    });
    connect(ui->login,&QPushButton::clicked, this,&loginWindow::onPressLoginButton);
    connect(pwdHide,&QToolButton::clicked,[&](bool checked){
        if(checked){
            pwdHide->setToolTip("隐藏密码");
            ui->pwd->setEchoMode(QLineEdit::Normal);
            ui->pwd->setFont(QFont("Timers",12));
            pwdHide->setIcon(QIcon(hidePwd_png));
        }
        else{
            pwdHide->setToolTip("显示密码");
            ui->pwd->setEchoMode(QLineEdit::Password);
            ui->pwd->setFont(QFont("Timers",7));
            pwdHide->setIcon(QIcon(showPwd_png));
        }

    });
}
void loginWindow::onUserNameChanged(const QString &text)
{
    QString curAccountNum = pAccountBox->currentText();
    int i;
    /* check user Name */
    for(i=0; i<accountsData.accountList.size(); ++i){
        if(curAccountNum == accountsData.accountList[i]){
            if(accountsData.rememberVector[i % accountsData.rememberVector.size()]){
                ui->pwd->setText(normalPwd);
                ui->remember->setCheckState(Qt::Checked);
            }
            else{
                ui->pwd->clear();
                ui->remember->setCheckState(Qt::Unchecked);
            }
            curUserName = accountsData.usrNameList[i % accountsData.usrNameList.size()];
            curHead = tool::pixmapToRound(QPixmap(accountsData.headVector[i % accountsData.headVector.size()]),
                                          ui->usrHead->width());
            ui->usrHead->setPixmap(curHead);

            curState = accountsData.states[i % accountsData.states.size()];
            ui->loginState->setToolTip(tips[curState % 7]);
            switch (curState){
                case ACTIVE:{
                    ui->loginState->setIcon(QIcon(active_png));
                }break;
                case AWAY:{
                    ui->loginState->setIcon(QIcon(away_png));
                }break;
                case BUSY:{
                    ui->loginState->setIcon(QIcon(busy_png));
                }break;
                case NOT_DISTURB:{
                    ui->loginState->setIcon(QIcon(notDisturb_png));
                }break;
                case HIDE:{
                    ui->loginState->setIcon(QIcon(hide_png));
                }break;
                default:{
                    ui->loginState->setIcon(QIcon(online_png));
                }
            }

            break;
        }
    }
    if(i==accountsData.accountList.size()){
        curHead = tool::pixmapToRound(QPixmap(avatar_png),
                                      ui->usrHead->width());
        ui->usrHead->setPixmap(curHead);
        curUserName.clear();
        ui->pwd->clear();
    }

}
void loginWindow::onPressLoginButton() {
    if(ui->login->text() == tLogin){
        if((pAccountBox->currentText().length() >= 6)
           && (ui->pwd->text().length() >= 6)){
            loginData.passWord = ui->pwd->text();
            loginData.account = pAccountBox->currentText();
            loginData.isRemember = ui->remember->isChecked();
            loginData.isAutoLogin = ui->autoLogin->isChecked();
            loginData.state = curState;
            loginData.head = curHead;
            loginData.usrName = curUserName;

            pAccountBox->setDisabled(true);
            ui->pwd->setDisabled(true);
            ui->remember->setDisabled(true);
            ui->autoLogin->setDisabled(true);
            ui->registerAccount->setDisabled(true);
            ui->findPwd->setDisabled(true);
            ui->addUsr->setDisabled(true);
            ui->login->setText(tCancelLogin);
            // emit
            emit sigLoginRequest(loginData);

        }
        else QMessageBox::critical(this,"Mini-MSG",tUsrInfoCheck);
    }
    else{
        if(autoLoginDelayTimer != -1){
            killTimer(autoLoginDelayTimer);
            autoLoginDelayTimer = -1;
        }
        ui->login->setText(tLogin);
        pAccountBox->setDisabled(false);
        ui->pwd->setDisabled(false);
        ui->remember->setDisabled(false);
        ui->autoLogin->setDisabled(false);
        ui->registerAccount->setDisabled(false);
        ui->findPwd->setDisabled(false);
        ui->addUsr->setDisabled(false);
        // emit
        emit sigCancelLogin();
    }


}


void loginWindow::onPressStateButton() {
    if(stateMenu == nullptr){
        stateMenu = new QMenu(this);
        acOnline = new QAction(QIcon(":resources/loginWindow/loginState/online.png"),
                               "我在线上",
                               this);
        acActive = new QAction(QIcon(":resources/loginWindow/loginState/active.png"),
                               "想聊天",
                               this);
        acAway = new QAction(QIcon(":resources/loginWindow/loginState/away.png"),
                               "离开",
                               this);
        acBusy = new QAction(QIcon(":resources/loginWindow/loginState/busy.png"),
                               "忙碌",
                               this);
        acNotDisturb = new QAction(QIcon(":resources/loginWindow/loginState/notDisturb.png"),
                               "请勿打扰",
                               this);
        acHide = new QAction(QIcon(":resources/loginWindow/loginState/hide.png"),
                               "隐身",
                               this);
        stateMenu->addAction(acOnline);
        stateMenu->addAction(acActive);
        stateMenu->addAction(acAway);
        stateMenu->addAction(acBusy);
        stateMenu->addAction(acNotDisturb);
        stateMenu->addAction(acHide);
        stateMenu->setMaximumSize(100,180);
       /*  action  -  slot */
        connect(acOnline,&QAction::triggered,[&](){
                ui->loginState->setIcon(QIcon(online_png));
                curState = ONLINE;
        });
        connect(acActive,&QAction::triggered,[&](){
            ui->loginState->setIcon(QIcon(active_png));
            curState = ACTIVE;
        });
        connect(acAway,&QAction::triggered,[&](){
            ui->loginState->setIcon(QIcon(away_png));
            curState = AWAY;
        });
        connect(acBusy,&QAction::triggered,[&](){
            ui->loginState->setIcon(QIcon(busy_png));
            curState = BUSY;
        });
        connect(acNotDisturb,&QAction::triggered,[&](){
            ui->loginState->setIcon(QIcon(notDisturb_png));
            curState = NOT_DISTURB;
        });
        connect(acHide,&QAction::triggered,[&](){
            ui->loginState->setIcon(QIcon(hide_png));
            curState = HIDE;
        });
    }
    stateMenu->exec(QCursor::pos());
}


void loginWindow::loadStyleSheet(const QString &sheetName) {
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void loginWindow::initBack() {
    QRect rectA(0,0,ui->back->width(),ui->back->height());
    QRect rectB(0,ui->back->height()-radiusSize,ui->back->width(),ui->back->height());
    QBitmap mask(ui->back->width(),ui->back->height());
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(rectA, Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(rectA, radiusSize, radiusSize);
    painter.fillRect(rectB, Qt::black);
    ui->back->setMask(mask);

    backMovie = new QMovie();
    backMovie->setFileName(back_gif);
    ui->back->setMovie(backMovie);
    backMovie->start();
}

void loginWindow::paintEvent(QPaintEvent *event)
{
    /* set login window radius */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);   // Antialiasing
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(this->rect(), radiusSize, radiusSize);
    QWidget::paintEvent(event);
}

bool loginWindow::eventFilter(QObject *target, QEvent *event) {
    if(target == ui->usrHead){
        if(event->type() == QEvent::Enter){
            angle = 0;
            timerID = startTimer(50);
        }
        else if(event->type() == QEvent::Leave){
            killTimer(timerID);
            timerID = -1;
            ui->usrHead->setPixmap(curHead);
        }
    }
    return QObject::eventFilter(target, event);
}

void loginWindow::timerEvent(QTimerEvent *event){
    if(event->timerId() == timerID){
        ui->usrHead->setPixmap(tool::setPixMapRotate(curHead,angle));
        angle = (angle>=357) ? (angle+5)%360 : (angle+5);
    }
    else if(event->timerId() == autoLoginDelayTimer){
        emit ui->login->clicked(true);
        killTimer(autoLoginDelayTimer);
        autoLoginDelayTimer = -1;
    }
}

void loginWindow::onLoginState(const QString& loginResult) {
    if(loginResult == "OK"){
        // login success
        disconnectSigSlots();
        emit sigExitLoginWindow();
    }
    else {
        QMessageBox::information(this,"登录",loginResult + "\t");
        ui->login->setText(tLogin);
        pAccountBox->setDisabled(false);
        ui->pwd->setDisabled(false);
        ui->remember->setDisabled(false);
        ui->autoLogin->setDisabled(false);
        ui->registerAccount->setDisabled(false);
        ui->findPwd->setDisabled(false);
        ui->addUsr->setDisabled(false);
    }
}

void loginWindow::disconnectSigSlots() {
    disconnect(ui->login, nullptr, nullptr, nullptr);
    disconnect(pAccountBox, nullptr, nullptr, nullptr);
    disconnect(ui->loginState , nullptr, nullptr, nullptr);
    disconnect(ui->addUsr, nullptr, nullptr, nullptr);
    disconnect(ui->usrHead, nullptr, nullptr, nullptr);
}

