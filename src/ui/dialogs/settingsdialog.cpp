#include "settingsdialog.h"
#include "core/database.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>

SettingsDialog::SettingsDialog(const QString &username, QWidget *parent)
    : QDialog(parent)
    , m_username(username)
{
    setupUI();
    loadSettings();
    
    setWindowTitle("个人设置");
    setModal(true);
    resize(400, 350);
}

void SettingsDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 账户设置组
    m_accountGroup = new QGroupBox("账户设置", this);
    QVBoxLayout *accountLayout = new QVBoxLayout(m_accountGroup);
    
    // 当前密码
    QHBoxLayout *currentPwdLayout = new QHBoxLayout();
    QLabel *currentPwdLabel = new QLabel("当前密码:", this);
    currentPwdLabel->setFixedWidth(80);
    m_currentPasswordEdit = new QLineEdit(this);
    m_currentPasswordEdit->setEchoMode(QLineEdit::Password);
    currentPwdLayout->addWidget(currentPwdLabel);
    currentPwdLayout->addWidget(m_currentPasswordEdit);
    accountLayout->addLayout(currentPwdLayout);
    
    // 新密码
    QHBoxLayout *newPwdLayout = new QHBoxLayout();
    QLabel *newPwdLabel = new QLabel("新密码:", this);
    newPwdLabel->setFixedWidth(80);
    m_newPasswordEdit = new QLineEdit(this);
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    newPwdLayout->addWidget(newPwdLabel);
    newPwdLayout->addWidget(m_newPasswordEdit);
    accountLayout->addLayout(newPwdLayout);
    
    // 确认密码
    QHBoxLayout *confirmPwdLayout = new QHBoxLayout();
    QLabel *confirmPwdLabel = new QLabel("确认密码:", this);
    confirmPwdLabel->setFixedWidth(80);
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPwdLayout->addWidget(confirmPwdLabel);
    confirmPwdLayout->addWidget(m_confirmPasswordEdit);
    accountLayout->addLayout(confirmPwdLayout);
    
    // 修改密码按钮
    m_changePasswordButton = new QPushButton("修改密码", this);
    accountLayout->addWidget(m_changePasswordButton);
    
    m_mainLayout->addWidget(m_accountGroup);
    
    // 聊天设置组
    m_chatGroup = new QGroupBox("聊天设置", this);
    QVBoxLayout *chatLayout = new QVBoxLayout(m_chatGroup);
    
    QHBoxLayout *historyLayout = new QHBoxLayout();
    QLabel *historyLabel = new QLabel("消息保存天数:", this);
    m_messageHistoryDays = new QSpinBox(this);
    m_messageHistoryDays->setRange(1, 30);
    m_messageHistoryDays->setValue(3);
    m_messageHistoryDays->setSuffix("天");
    historyLayout->addWidget(historyLabel);
    historyLayout->addWidget(m_messageHistoryDays);
    historyLayout->addStretch();
    chatLayout->addLayout(historyLayout);
    
    m_mainLayout->addWidget(m_chatGroup);
    
    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("保存", this);
    m_cancelButton = new QPushButton("取消", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    m_mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(m_changePasswordButton, &QPushButton::clicked, this, &SettingsDialog::onChangePassword);
    connect(m_saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveSettings);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void SettingsDialog::loadSettings()
{
    // 这里可以加载用户的个人设置
    // 目前使用默认值
}

void SettingsDialog::onChangePassword()
{
    QString currentPassword = m_currentPasswordEdit->text();
    QString newPassword = m_newPasswordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();
    
    if (currentPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写所有密码字段");
        return;
    }
    
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "错误", "新密码和确认密码不一致");
        return;
    }
    
    if (newPassword.length() < 6) {
        QMessageBox::warning(this, "错误", "新密码长度至少6位");
        return;
    }
    
    // 验证当前密码
    if (!Database::instance().validateUser(m_username, currentPassword)) {
        QMessageBox::warning(this, "错误", "当前密码不正确");
        return;
    }
    
    // 更新密码
    QSqlQuery query;
    query.prepare("UPDATE users SET password = ? WHERE username = ?");
    QByteArray hash = QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256);
    query.addBindValue(hash.toHex());
    query.addBindValue(m_username);
    
    if (query.exec()) {
        QMessageBox::information(this, "成功", "密码修改成功");
        m_currentPasswordEdit->clear();
        m_newPasswordEdit->clear();
        m_confirmPasswordEdit->clear();
    } else {
        QMessageBox::warning(this, "错误", "密码修改失败");
    }
}

void SettingsDialog::onSaveSettings()
{
    // 这里可以保存其他设置
    QMessageBox::information(this, "成功", "设置已保存");
    accept();
}
