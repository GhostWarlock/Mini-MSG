//
// Created by wusong on 2021/2/9.
//

#include "accountBox.h"
#include <QMessageBox>

accountBox::accountBox(QWidget *parent)
            : QComboBox(parent){
    pListWidget = new QListWidget();
    setModel(pListWidget->model());
    setView(pListWidget);
    setEditable(true);
    setFixedSize(190,30);
//    setStyleSheet(thisStyle);
}

accountBox::~accountBox() {
    delete pListWidget;
}

void accountBox::addAccount(accountItem *pAccountItem) {
    connect(pAccountItem, &accountItem::sigShowItem, this, &accountBox::onShowAccount);
    connect(pAccountItem, &accountItem::sigRemoveItem,this, &accountBox::onRemoveAccount);
    auto* item = new QListWidgetItem(pListWidget);
    pListWidget->setItemWidget(item, pAccountItem);
}

void accountBox::onShowAccount(const QString &account) {
    setEditText(account);
    hidePopup();
}

void accountBox::onRemoveAccount(const QString &account) {
    hidePopup();
    auto dialog = QMessageBox::question(this, "删除账号", "确定移除该账号并清除所有记录吗？\t",
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
    if (dialog == QMessageBox::Yes) {
        auto listCount = pListWidget->count();
        for (int i = 0; i < listCount; ++i) {
            auto *item = reinterpret_cast<accountItem *>(pListWidget->itemWidget(pListWidget->item(i)));
            if (item->getAccountNum() == account) {
                disconnect(item,nullptr,nullptr,nullptr);
                pListWidget->takeItem(i);
                emit sigRemoveAccount(item->getAccountId());
                delete item;
                break;
            }
        }
    }
}
