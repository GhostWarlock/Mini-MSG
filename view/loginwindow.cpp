
#include <QMouseEvent>
#include <QFile>
#include <QDebug>
#include <QMovie>
#include <QPainter>
#include <QIcon>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QListView>
#include <QBitmap>

#include "loginwindow.h"
#include "resources.h"
#include "tools.h"
#include "./ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *p,loginDataGroup data)
        : QWidget(p)
        , parent(p)
        , ui(new Ui::LoginWindow)
        , accountsData(std::move(data))
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.92);
    initControls();          // init Controls
    initBack();

    loadStyleSheet(loginWindow_css);
}

LoginWindow::~LoginWindow()
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

void LoginWindow::mousePressEvent(QMouseEvent *e) // 鼠标点击事件
{
    lastPos = e->globalPos();
    isPressedWidget = true;
}
void LoginWindow::mouseMoveEvent(QMouseEvent *e) // 鼠标移动事件
{
    if(isPressedWidget){
        int xLen = e->globalX() - lastPos.x();
        int yLen = e->globalY() - lastPos.y();
        lastPos = e->globalPos();
        move(x()+xLen, y()+yLen);    // move widget
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *e) {
    isPressedWidget = false;
}

void LoginWindow::initControls() {
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
    ui->userName->setEditable(true);
    ui->userName->addItems(accountsData.usrNameList);
    ui->userName->setView(new QListView);
    int i = 0;
    for(auto &img : accountsData.headVector)
        ui->userName->setItemIcon(i++,QIcon(img));
    if(!accountsData.headVector.empty()){
        curHead = tool::pixmapToRound(QPixmap(accountsData.headVector[usrIndex]),
                                      ui->usrHead->width());
    }

    /* user head */
    if(curHead.isNull()){
        curHead = tool::pixmapToRound(QPixmap(avatar_png),ui->usrHead->width());
    }
    ui->usrHead->setPixmap(curHead);
    ui->usrHead->installEventFilter(this);

    /* login status Icon */
    ui->loginState->setToolTip(tips[accountsData.state % 7]);
    switch (accountsData.state){
        case ACTIVE:{
            ui->loginState->setIcon(QIcon(active_png));
            curState = ACTIVE;
        }break;
        case AWAY:{
            ui->loginState->setIcon(QIcon(away_png));
            curState = AWAY;
        }break;
        case BUSY:{
            ui->loginState->setIcon(QIcon(busy_png));
            curState = BUSY;
        }break;
        case NOT_DISTURB:{
            ui->loginState->setIcon(QIcon(notDisturb_png));
            curState = NOT_DISTURB;
        }break;
        case HIDE:{
            ui->loginState->setIcon(QIcon(hide_png));
            curState = HIDE;
        }break;
        default:{
            ui->loginState->setIcon(QIcon(online_png));
            curState = ONLINE;
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
    if((!accountsData.rememberVector.empty())
        && accountsData.rememberVector[usrIndex]){
        ui->remember->setCheckState(Qt::CheckState::Checked);
        ui->pwd->setText("rememberType");
    }
    else{
        ui->remember->setCheckState(Qt::CheckState::Unchecked);
        ui->pwd->clear();
    }

    /* login button */
    ui->login->setText(tLogin);

    /* connect SIGNAL and SLOT */
    connect(ui->min,&QToolButton::clicked,[&](){
#ifdef Q_OS_WIN
        if (sysTrayIcon == nullptr) {
            sysTrayIcon = new QSystemTrayIcon(QIcon(appIcon_png), this);
            sysTrayIcon->setToolTip("Mini-MSG:Login");
        }
        this->hide();
        sysTrayIcon->show();
#endif
#ifdef Q_OS_LINUX
        QMessageBox::information(this, "Mini-MSG", "系统不支持最小化\t");
#endif
    });
    connect(ui->close,&QToolButton::clicked,[=](){emit exitLoginWindow();});
    connect(ui->loginState,SIGNAL(clicked()), this,SLOT(pressStateButton()));
    connect(ui->addUsr,&QPushButton::clicked,[=](){
        ui->userName->clearEditText();
        ui->pwd->clear();
        ui->usrHead->setPixmap(curHead);
    });
    connect(ui->userName,&QComboBox::editTextChanged,this, &LoginWindow::onUserNameChanged);
    connect(ui->pwd,&QLineEdit::textChanged,this,[&](){
        qDebug() << ui->pwd->text();
    });
    connect(ui->login,&QPushButton::clicked, this,&LoginWindow::pressLoginButton);
    connect(pwdHide,&QToolButton::clicked,[&](bool checked){
        if(checked){
            pwdHide->setToolTip("隐藏密码");
            ui->pwd->setEchoMode(QLineEdit::Normal);
            pwdHide->setIcon(QIcon(hidePwd_png));
        }
        else{
            pwdHide->setToolTip("显示密码");
            ui->pwd->setEchoMode(QLineEdit::Password);
            pwdHide->setIcon(QIcon(showPwd_png));
        }

    });
}
void LoginWindow::onUserNameChanged(const QString &text)
{
    /* check user Name */
}
void LoginWindow::pressLoginButton() {
    if(ui->login->text() == tLogin){
        if((ui->userName->currentText().length() >= 6)
           && (ui->pwd->text().length() >= 6)){
            loginData.pwd = ui->pwd->text();
            loginData.usrNameList.clear();
            loginData.usrNameList.append(ui->userName->currentText());
            loginData.rememberVector.clear();
            loginData.rememberVector.push_back(ui->remember->isChecked());
            loginData.isAutoLogin = ui->autoLogin->isChecked();
            loginData.state = curState;

            ui->userName->setDisabled(true);
            ui->pwd->setDisabled(true);
            ui->remember->setDisabled(true);
            ui->autoLogin->setDisabled(true);
            ui->registerAccount->setDisabled(true);
            ui->findPwd->setDisabled(true);
            ui->addUsr->setDisabled(true);
            ui->login->setText(tCancelLogin);
            qDebug() << "emit Login";
            // emit
        }
        else QMessageBox::critical(this,"Mini-MSG",tUsrInfoCheck);
    }
    else{
        ui->login->setText(tLogin);
        ui->userName->setDisabled(false);
        ui->pwd->setDisabled(false);
        ui->remember->setDisabled(false);
        ui->autoLogin->setDisabled(false);
        ui->registerAccount->setDisabled(false);
        ui->findPwd->setDisabled(false);
        ui->addUsr->setDisabled(false);
        qDebug() << "emit cancelLogin";
        // emit
    }


}


void LoginWindow::pressStateButton() {
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
            qDebug() << "in";
            ui->loginState->setIcon(QIcon(notDisturb_png));
            curState = NOT_DISTURB;
        });
        connect(acHide,&QAction::triggered,[&](){
            qDebug() << "in";
            ui->loginState->setIcon(QIcon(hide_png));
            curState = HIDE;
        });
    }
    stateMenu->exec(QCursor::pos());
}


void LoginWindow::loadStyleSheet(const QString &sheetName) {
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void LoginWindow::initBack() {
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
    backMovie->setFileName(":resources/loginWindow/back.gif");
    ui->back->setMovie(backMovie);
    backMovie->start();
}

void LoginWindow::paintEvent(QPaintEvent *event)
{
    /* set login window radius */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);   // Antialiasing
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(this->rect(), radiusSize, radiusSize);
    QWidget::paintEvent(event);
}

bool LoginWindow::eventFilter(QObject *target, QEvent *event) {
    if(target == ui->usrHead){
        if(event->type() == QEvent::Enter){
            timerID = startTimer(50);
        }
        else if(event->type() == QEvent::Leave){
            killTimer(timerID);
            ui->usrHead->setPixmap(curHead);
        }
    }
    return QObject::eventFilter(target, event);
}

void LoginWindow::timerEvent(QTimerEvent *event){
    if(event->timerId() == timerID){
        ui->usrHead->setPixmap(tool::setPixMapRotate(curHead,angle));
        angle = (angle>=357) ? (angle+4)%360 : (angle+4);
    }
}

