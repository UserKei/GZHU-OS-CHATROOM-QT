#include "chatserver.h"
#include "core/database.h"
#include "core/messagefilter.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
    // 设置定时器清理旧消息（每天清理一次）
    m_cleanupTimer = new QTimer(this);
    connect(m_cleanupTimer, &QTimer::timeout, this, &ChatServer::cleanupOldMessages);
    m_cleanupTimer->start(24 * 60 * 60 * 1000); // 24小时
}

bool ChatServer::startServer(quint16 port)
{
    if (listen(QHostAddress::Any, port)) {
        qDebug() << "Chat server started on port" << port;
        return true;
    }
    
    qDebug() << "Failed to start server:" << errorString();
    return false;
}

void ChatServer::stopServer()
{
    // 断开所有客户端
    for (QTcpSocket *client : m_clients) {
        client->disconnectFromHost();
    }
    
    close();
    qDebug() << "Chat server stopped";
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);
    
    connect(client, &QTcpSocket::connected, this, &ChatServer::onClientConnected);
    connect(client, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    connect(client, &QTcpSocket::readyRead, this, &ChatServer::onMessageReceived);
    
    m_clients.append(client);
    
    qDebug() << "New client connected:" << client->peerAddress().toString();
}

void ChatServer::onClientConnected()
{
    qDebug() << "Client connected";
}

void ChatServer::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QString username = m_clientUsers.value(client);
        if (!username.isEmpty()) {
            Database::instance().updateUserStatus(username, false);
            
            // 通知其他客户端用户下线
            QJsonObject message;
            message["type"] = "user_offline";
            message["username"] = username;
            
            for (QTcpSocket *otherClient : m_clients) {
                if (otherClient != client) {
                    sendToClient(otherClient, message);
                }
            }
        }
        
        m_clients.removeOne(client);
        m_clientUsers.remove(client);
        client->deleteLater();
        
        qDebug() << "Client disconnected:" << username;
    }
}

void ChatServer::onMessageReceived()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;
    
    QByteArray data = client->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isObject()) {
        handleClientMessage(client, doc.object());
    }
}

void ChatServer::handleClientMessage(QTcpSocket *client, const QJsonObject &message)
{
    QString type = message["type"].toString();
    
    if (type == "login") {
        QString username = message["username"].toString();
        m_clientUsers[client] = username;
        
        // 发送在线用户列表
        QJsonObject response;
        response["type"] = "online_users";
        QJsonArray users;
        QStringList onlineUsers = Database::instance().getOnlineUsers();
        for (const QString &user : onlineUsers) {
            users.append(user);
        }
        response["users"] = users;
        sendToClient(client, response);
        
        // 通知其他客户端用户上线
        QJsonObject notification;
        notification["type"] = "user_online";
        notification["username"] = username;
        
        for (QTcpSocket *otherClient : m_clients) {
            if (otherClient != client) {
                sendToClient(otherClient, notification);
            }
        }
        
        qDebug() << "User logged in:" << username;
    }
    else if (type == "message") {
        QString sender = m_clientUsers.value(client);
        QString content = message["content"].toString();
        QString receiver = message["receiver"].toString();
        
        if (sender.isEmpty()) return;
        
        // 检查发送权限
        if (!Database::instance().getUserPermission(sender, "send_messages")) {
            QJsonObject error;
            error["type"] = "error";
            error["message"] = "您没有发送消息的权限";
            sendToClient(client, error);
            return;
        }
        
        // 过滤敏感词
        QString filteredContent = MessageFilter::instance().filterMessage(content);
        
        // 保存消息到数据库
        Database::instance().saveMessage(sender, receiver, filteredContent, QDateTime::currentDateTime());
        
        // 广播消息
        broadcastMessage(sender, filteredContent, receiver);
        
        qDebug() << "Message from" << sender << "to" << (receiver.isEmpty() ? "all" : receiver) << ":" << filteredContent;
    }
}

void ChatServer::broadcastMessage(const QString &sender, const QString &message, const QString &receiver)
{
    QJsonObject messageObj;
    messageObj["type"] = "message";
    messageObj["sender"] = sender;
    messageObj["content"] = message;
    messageObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    if (receiver.isEmpty()) {
        // 广播给所有用户
        messageObj["receiver"] = "";
        for (QTcpSocket *client : m_clients) {
            QString username = m_clientUsers.value(client);
            if (!username.isEmpty() && Database::instance().getUserPermission(username, "receive_messages")) {
                sendToClient(client, messageObj);
            }
        }
    } else {
        // 私聊消息
        messageObj["receiver"] = receiver;
        
        // 发送给接收者
        for (QTcpSocket *client : m_clients) {
            QString username = m_clientUsers.value(client);
            if (username == receiver && Database::instance().getUserPermission(username, "receive_messages")) {
                sendToClient(client, messageObj);
                break;
            }
        }
        
        // 发送给发送者（确认）
        for (QTcpSocket *client : m_clients) {
            QString username = m_clientUsers.value(client);
            if (username == sender) {
                sendToClient(client, messageObj);
                break;
            }
        }
    }
}

void ChatServer::sendToClient(QTcpSocket *client, const QJsonObject &message)
{
    QJsonDocument doc(message);
    client->write(doc.toJson());
}

void ChatServer::cleanupOldMessages()
{
    // 这里可以实现清理3天前的消息
    qDebug() << "Cleaning up old messages";
    // 可以添加SQL查询来删除旧消息
}
