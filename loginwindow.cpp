#include "loginwindow.h"
#include "database.h"
#include "mainwindow.h"
#include "registerdialog.h"
#include <QMessageBox>
#include <QApplication>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("聊天室系统 - 登录");
    resize(400, 300);
}

void LoginWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // 标题
    QLabel *titleLabel = new QLabel("聊天室管理系统", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
    m_mainLayout->addWidget(titleLabel);
    
    // 登录组
    m_loginGroup = new QGroupBox("用户登录", this);
    QVBoxLayout *loginLayout = new QVBoxLayout(m_loginGroup);
    
    // 用户名
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QLabel *usernameLabel = new QLabel("用户名:", this);
    usernameLabel->setFixedWidth(80);
    m_usernameEdit = new QLineEdit(this);
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit);
    loginLayout->addLayout(usernameLayout);
    
    // 密码
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("密码:", this);
    passwordLabel->setFixedWidth(80);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    loginLayout->addLayout(passwordLayout);
    
    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_loginButton = new QPushButton("登录", this);
    m_registerButton = new QPushButton("注册", this);
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_registerButton);
    loginLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_loginGroup);
    
    // 状态标签
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red;");
    m_mainLayout->addWidget(m_statusLabel);
    
    m_mainLayout->addStretch();
    
    // 连接信号
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    
    // 回车登录
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("请输入用户名和密码");
        return;
    }
    
    if (Database::instance().validateUser(username, password)) {
        // 更新用户在线状态
        Database::instance().updateUserStatus(username, true);
        
        // 打开主窗口
        MainWindow *mainWindow = new MainWindow(username);
        mainWindow->show();
        
        // 关闭登录窗口
        close();
    } else {
        m_statusLabel->setText("用户名或密码错误");
    }
}

void LoginWindow::onRegisterClicked()
{
    RegisterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_statusLabel->setText("注册成功，请登录");
        m_statusLabel->setStyleSheet("color: green;");
    }
}
