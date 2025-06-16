#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include "chatclient.h"
#include "chatserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSendMessage();
    void onMessageReceived(const QString &sender, const QString &content, const QString &receiver, const QString &timestamp);
    void onOnlineUsersReceived(const QStringList &users);
    void onUserOnline(const QString &username);
    void onUserOffline(const QString &username);
    void onClientConnected();
    void onClientDisconnected();
    void onClientError(const QString &error);
    void onStartServer();
    void onStopServer();
    void onShowUserManager();
    void onShowSettings();
    void onRecipientChanged();
    void loadHistoryMessages();

private:
    void setupUI();
    void setupMenuBar();
    void connectToServer();
    void updateStatus();
    void addMessageToChat(const QString &sender, const QString &content, const QString &receiver, const QString &timestamp);
    
    QString m_username;
    ChatClient *m_client;
    ChatServer *m_server;
    bool m_isServerMode;
    
    // UI组件
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QTextEdit *m_chatDisplay;
    QListWidget *m_userList;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QComboBox *m_recipientCombo;
    QLabel *m_statusLabel;
    
    // 菜单
    QMenu *m_serverMenu;
    QMenu *m_userMenu;
    QMenu *m_settingsMenu;
    QAction *m_startServerAction;
    QAction *m_stopServerAction;
    QAction *m_userManagerAction;
    QAction *m_settingsAction;
};

#endif // MAINWINDOW_H
