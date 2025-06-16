#include "registerdialog.h"
#include "database.h"
#include <QMessageBox>
#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("用户注册");
    setModal(true);
    resize(350, 250);
}

void RegisterDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 用户名
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QLabel *usernameLabel = new QLabel("用户名:", this);
    usernameLabel->setFixedWidth(80);
    m_usernameEdit = new QLineEdit(this);
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit);
    m_mainLayout->addLayout(usernameLayout);
    
    // 密码
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("密码:", this);
    passwordLabel->setFixedWidth(80);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    m_mainLayout->addLayout(passwordLayout);
    
    // 确认密码
    QHBoxLayout *confirmLayout = new QHBoxLayout();
    QLabel *confirmLabel = new QLabel("确认密码:", this);
    confirmLabel->setFixedWidth(80);
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmLabel);
    confirmLayout->addWidget(m_confirmPasswordEdit);
    m_mainLayout->addLayout(confirmLayout);
    
    // 邮箱
    QHBoxLayout *emailLayout = new QHBoxLayout();
    QLabel *emailLabel = new QLabel("邮箱:", this);
    emailLabel->setFixedWidth(80);
    m_emailEdit = new QLineEdit(this);
    emailLayout->addWidget(emailLabel);
    emailLayout->addWidget(m_emailEdit);
    m_mainLayout->addLayout(emailLayout);
    
    // 状态标签
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: red;");
    m_mainLayout->addWidget(m_statusLabel);
    
    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_registerButton = new QPushButton("注册", this);
    m_cancelButton = new QPushButton("取消", this);
    buttonLayout->addWidget(m_registerButton);
    buttonLayout->addWidget(m_cancelButton);
    m_mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(m_registerButton, &QPushButton::clicked, this, &RegisterDialog::onRegisterClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void RegisterDialog::onRegisterClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();
    QString email = m_emailEdit->text().trimmed();
    
    // 验证输入
    if (username.isEmpty()) {
        m_statusLabel->setText("请输入用户名");
        return;
    }
    
    if (username.length() < 3 || username.length() > 20) {
        m_statusLabel->setText("用户名长度应在3-20字符之间");
        return;
    }
    
    if (password.isEmpty()) {
        m_statusLabel->setText("请输入密码");
        return;
    }
    
    if (password.length() < 6) {
        m_statusLabel->setText("密码长度至少6位");
        return;
    }
    
    if (password != confirmPassword) {
        m_statusLabel->setText("两次输入的密码不一致");
        return;
    }
    
    if (!email.isEmpty()) {
        QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.\\w+$");
        if (!emailRegex.match(email).hasMatch()) {
            m_statusLabel->setText("邮箱格式不正确");
            return;
        }
    }
    
    // 创建用户
    if (Database::instance().createUser(username, password, email)) {
        QMessageBox::information(this, "成功", "注册成功！");
        accept();
    } else {
        m_statusLabel->setText("注册失败，用户名可能已存在");
    }
}
