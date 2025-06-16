#include "usermanagerdialog.h"
#include "core/database.h"
#include "core/messagefilter.h"
#include <QInputDialog>
#include <QMessageBox>

UserManagerDialog::UserManagerDialog(const QString &username, QWidget *parent)
    : QDialog(parent)
    , m_username(username)
{
    setupUI();
    loadPermissions();
    loadSensitiveWords();
    
    setWindowTitle("用户管理");
    setModal(true);
    resize(400, 500);
}

void UserManagerDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 权限管理组
    m_permissionGroup = new QGroupBox("个人权限设置", this);
    QVBoxLayout *permissionLayout = new QVBoxLayout(m_permissionGroup);
    
    m_receiveMessagesCheck = new QCheckBox("接收消息", this);
    m_sendMessagesCheck = new QCheckBox("发送消息", this);
    m_deleteMessagesCheck = new QCheckBox("删除自己的消息", this);
    
    permissionLayout->addWidget(m_receiveMessagesCheck);
    permissionLayout->addWidget(m_sendMessagesCheck);
    permissionLayout->addWidget(m_deleteMessagesCheck);
    
    m_mainLayout->addWidget(m_permissionGroup);
    
    // 敏感词管理组
    m_sensitiveWordsGroup = new QGroupBox("敏感词管理", this);
    QVBoxLayout *sensitiveLayout = new QVBoxLayout(m_sensitiveWordsGroup);
    
    m_sensitiveWordsList = new QListWidget(this);
    sensitiveLayout->addWidget(m_sensitiveWordsList);
    
    m_addWordButton = new QPushButton("添加敏感词", this);
    sensitiveLayout->addWidget(m_addWordButton);
    
    m_mainLayout->addWidget(m_sensitiveWordsGroup);
    
    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *closeButton = new QPushButton("关闭", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    m_mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(m_receiveMessagesCheck, &QCheckBox::toggled, this, &UserManagerDialog::onPermissionChanged);
    connect(m_sendMessagesCheck, &QCheckBox::toggled, this, &UserManagerDialog::onPermissionChanged);
    connect(m_deleteMessagesCheck, &QCheckBox::toggled, this, &UserManagerDialog::onPermissionChanged);
    connect(m_addWordButton, &QPushButton::clicked, this, &UserManagerDialog::onAddSensitiveWord);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void UserManagerDialog::loadPermissions()
{
    m_receiveMessagesCheck->setChecked(Database::instance().getUserPermission(m_username, "receive_messages"));
    m_sendMessagesCheck->setChecked(Database::instance().getUserPermission(m_username, "send_messages"));
    m_deleteMessagesCheck->setChecked(Database::instance().getUserPermission(m_username, "delete_own_messages"));
}

void UserManagerDialog::loadSensitiveWords()
{
    m_sensitiveWordsList->clear();
    QStringList words = Database::instance().getSensitiveWords();
    m_sensitiveWordsList->addItems(words);
}

void UserManagerDialog::onPermissionChanged()
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;
    
    QString permission;
    if (checkBox == m_receiveMessagesCheck) {
        permission = "receive_messages";
    } else if (checkBox == m_sendMessagesCheck) {
        permission = "send_messages";
    } else if (checkBox == m_deleteMessagesCheck) {
        permission = "delete_own_messages";
    }
    
    if (!permission.isEmpty()) {
        Database::instance().setUserPermission(m_username, permission, checkBox->isChecked());
    }
}

void UserManagerDialog::onAddSensitiveWord()
{
    bool ok;
    QString word = QInputDialog::getText(this, "添加敏感词", "请输入要添加的敏感词:", QLineEdit::Normal, "", &ok);
    
    if (ok && !word.isEmpty()) {
        if (Database::instance().addSensitiveWord(word)) {
            loadSensitiveWords();
            MessageFilter::instance().updateSensitiveWords();
            QMessageBox::information(this, "成功", "敏感词添加成功");
        } else {
            QMessageBox::warning(this, "错误", "敏感词添加失败，可能已存在");
        }
    }
}
