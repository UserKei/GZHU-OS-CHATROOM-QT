#include "mainwindow.h"
#include "database.h"
#include "usermanagerdialog.h"
#include "settingsdialog.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QApplication>
#include <QTimer>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , m_username(username)
    , m_client(nullptr)
    , m_server(nullptr)
    , m_isServerMode(false)
{
    setupUI();
    setupMenuBar();
    
    setWindowTitle(QString("聊天室 - %1").arg(m_username));
    resize(800, 600);
    
    // 连接到服务器
    connectToServer();
    
    // 加载历史消息
    loadHistoryMessages();
}

MainWindow::~MainWindow()
{
    if (m_client) {
        m_client->disconnectFromServer();
    }
    
    if (m_server) {
        m_server->stopServer();
    }
    
    // 更新用户离线状态
    Database::instance().updateUserStatus(m_username, false);
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 左侧聊天区域
    QWidget *chatWidget = new QWidget();
    QVBoxLayout *chatLayout = new QVBoxLayout(chatWidget);
    
    // 接收者选择
    QHBoxLayout *recipientLayout = new QHBoxLayout();
    QLabel *recipientLabel = new QLabel("发送给:", this);
    m_recipientCombo = new QComboBox(this);
    m_recipientCombo->addItem("所有人", "");
    recipientLayout->addWidget(recipientLabel);
    recipientLayout->addWidget(m_recipientCombo);
    recipientLayout->addStretch();
    chatLayout->addLayout(recipientLayout);
    
    // 聊天显示区域
    m_chatDisplay = new QTextEdit(this);
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setFont(QFont("Arial", 10));
    chatLayout->addWidget(m_chatDisplay);
    
    // 消息输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout();
    m_messageInput = new QLineEdit(this);
    m_messageInput->setPlaceholderText("输入消息...");
    m_sendButton = new QPushButton("发送", this);
    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_sendButton);
    chatLayout->addLayout(inputLayout);
    
    m_mainSplitter->addWidget(chatWidget);
    
    // 右侧用户列表
    QWidget *userWidget = new QWidget();
    QVBoxLayout *userLayout = new QVBoxLayout(userWidget);
    
    QLabel *userLabel = new QLabel("在线用户:", this);
    userLabel->setFont(QFont("Arial", 10, QFont::Bold));
    userLayout->addWidget(userLabel);
    
    m_userList = new QListWidget(this);
    userLayout->addWidget(m_userList);
    
    m_mainSplitter->addWidget(userWidget);
    m_mainSplitter->setSizes({600, 200});
    
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_mainSplitter);
    
    // 状态栏
    m_statusLabel = new QLabel("正在连接服务器...", this);
    statusBar()->addWidget(m_statusLabel);
    
    // 连接信号
    connect(m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendMessage);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &MainWindow::onSendMessage);
    connect(m_recipientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onRecipientChanged);
}

void MainWindow::setupMenuBar()
{
    // 服务器菜单
    m_serverMenu = menuBar()->addMenu("服务器");
    
    m_startServerAction = m_serverMenu->addAction("启动服务器");
    m_stopServerAction = m_serverMenu->addAction("停止服务器");
    m_stopServerAction->setEnabled(false);
    
    connect(m_startServerAction, &QAction::triggered, this, &MainWindow::onStartServer);
    connect(m_stopServerAction, &QAction::triggered, this, &MainWindow::onStopServer);
    
    // 用户菜单
    m_userMenu = menuBar()->addMenu("用户");
    m_userManagerAction = m_userMenu->addAction("用户管理");
    connect(m_userManagerAction, &QAction::triggered, this, &MainWindow::onShowUserManager);
    
    // 设置菜单
    m_settingsMenu = menuBar()->addMenu("设置");
    m_settingsAction = m_settingsMenu->addAction("个人设置");
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::onShowSettings);
}

void MainWindow::connectToServer()
{
    m_client = new ChatClient(this);
    
    connect(m_client, &ChatClient::connected, this, &MainWindow::onClientConnected);
    connect(m_client, &ChatClient::disconnected, this, &MainWindow::onClientDisconnected);
    connect(m_client, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_client, &ChatClient::onlineUsersReceived, this, &MainWindow::onOnlineUsersReceived);
    connect(m_client, &ChatClient::userOnline, this, &MainWindow::onUserOnline);
    connect(m_client, &ChatClient::userOffline, this, &MainWindow::onUserOffline);
    connect(m_client, &ChatClient::errorOccurred, this, &MainWindow::onClientError);
    
    if (m_client->connectToServer()) {
        m_client->sendLogin(m_username);
    } else {
        m_statusLabel->setText("连接服务器失败，可能需要启动服务器");
    }
}

void MainWindow::onSendMessage()
{
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    QString receiver = m_recipientCombo->currentData().toString();
    
    if (m_client) {
        // 先发送消息到服务器
        m_client->sendMessage(message, receiver);
        
        // 立即在本地显示消息（不等服务器回显）
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        addMessageToChat(m_username, message, receiver, timestamp);
        
        m_messageInput->clear();
    } else {
        // 如果没有连接到服务器，仍然显示消息但标记为离线
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        addMessageToChat(m_username + " (离线)", message, receiver, timestamp);
        m_messageInput->clear();
    }
}

void MainWindow::onMessageReceived(const QString &sender, const QString &content, const QString &receiver, const QString &timestamp)
{
    // 如果是自己发送的消息，不重复显示（因为发送时已经显示了）
    if (sender != m_username) {
        addMessageToChat(sender, content, receiver, timestamp);
    }
}

void MainWindow::addMessageToChat(const QString &sender, const QString &content, const QString &receiver, const QString &timestamp)
{
    QString timeStr = QDateTime::fromString(timestamp, Qt::ISODate).toString("hh:mm:ss");
    QString displayText;
    
    if (receiver.isEmpty()) {
        // 公共消息
        displayText = QString("<b>[%1] %2:</b> %3").arg(timeStr, sender, content);
    } else {
        // 私聊消息
        if (sender == m_username) {
            displayText = QString("<b>[%1] 私聊给 %2:</b> %3").arg(timeStr, receiver, content);
        } else {
            displayText = QString("<b>[%1] %2 私聊:</b> %3").arg(timeStr, sender, content);
        }
    }
    
    m_chatDisplay->append(displayText);
    
    // 自动滚动到底部
    QTextCursor cursor = m_chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_chatDisplay->setTextCursor(cursor);
}

void MainWindow::onOnlineUsersReceived(const QStringList &users)
{
    m_userList->clear();
    m_recipientCombo->clear();
    m_recipientCombo->addItem("所有人", "");
    
    for (const QString &user : users) {
        m_userList->addItem(user);
        if (user != m_username) {
            m_recipientCombo->addItem(user, user);
        }
    }
    
    updateStatus();
}

void MainWindow::onUserOnline(const QString &username)
{
    if (username != m_username) {
        m_userList->addItem(username);
        m_recipientCombo->addItem(username, username);
        
        QString message = QString("%1 上线了").arg(username);
        addMessageToChat("系统", message, "", QDateTime::currentDateTime().toString(Qt::ISODate));
    }
    
    updateStatus();
}

void MainWindow::onUserOffline(const QString &username)
{
    for (int i = 0; i < m_userList->count(); ++i) {
        if (m_userList->item(i)->text() == username) {
            delete m_userList->takeItem(i);
            break;
        }
    }
    
    for (int i = 1; i < m_recipientCombo->count(); ++i) {
        if (m_recipientCombo->itemData(i).toString() == username) {
            m_recipientCombo->removeItem(i);
            break;
        }
    }
    
    QString message = QString("%1 下线了").arg(username);
    addMessageToChat("系统", message, "", QDateTime::currentDateTime().toString(Qt::ISODate));
    
    updateStatus();
}

void MainWindow::onClientConnected()
{
    updateStatus();
}

void MainWindow::onClientDisconnected()
{
    m_statusLabel->setText("与服务器断开连接");
}

void MainWindow::onClientError(const QString &error)
{
    m_statusLabel->setText(QString("连接错误: %1").arg(error));
}

void MainWindow::onStartServer()
{
    if (!m_server) {
        m_server = new ChatServer(this);
    }
    
    if (m_server->startServer()) {
        m_isServerMode = true;
        m_startServerAction->setEnabled(false);
        m_stopServerAction->setEnabled(true);
        m_statusLabel->setText("服务器已启动");
        
        // 如果客户端未连接，重新连接
        if (m_client && !m_client->connectToServer()) {
            QTimer::singleShot(1000, [this]() {
                if (m_client->connectToServer()) {
                    m_client->sendLogin(m_username);
                }
            });
        }
    } else {
        QMessageBox::warning(this, "错误", "启动服务器失败");
    }
}

void MainWindow::onStopServer()
{
    if (m_server) {
        m_server->stopServer();
        m_isServerMode = false;
        m_startServerAction->setEnabled(true);
        m_stopServerAction->setEnabled(false);
        m_statusLabel->setText("服务器已停止");
    }
}

void MainWindow::onShowUserManager()
{
    UserManagerDialog dialog(m_username, this);
    dialog.exec();
}

void MainWindow::onShowSettings()
{
    SettingsDialog dialog(m_username, this);
    dialog.exec();
}

void MainWindow::onRecipientChanged()
{
    // 可以在这里添加私聊相关的逻辑
}

void MainWindow::loadHistoryMessages()
{
    auto messages = Database::instance().getRecentMessages(m_username, 3);
    
    for (const auto &msg : messages) {
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        addMessageToChat(msg.first, msg.second, "", timestamp);
    }
    
    if (!messages.isEmpty()) {
        m_chatDisplay->append("<hr><i>以上是历史消息</i><hr>");
    }
}

void MainWindow::updateStatus()
{
    int userCount = m_userList->count();
    QString status = QString("在线用户: %1 人").arg(userCount);
    
    if (m_isServerMode) {
        status += " (服务器模式)";
    }
    
    m_statusLabel->setText(status);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Database::instance().updateUserStatus(m_username, false);
    event->accept();
}
