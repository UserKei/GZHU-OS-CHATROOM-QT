#include "chatclient.h"
#include <QJsonArray>
#include <QDebug>

ChatClient::ChatClient(QObject *parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    
    connect(m_socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ChatClient::onError);
}

bool ChatClient::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
    return m_socket->waitForConnected(3000);
}

void ChatClient::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}

void ChatClient::sendLogin(const QString &username)
{
    QJsonObject message;
    message["type"] = "login";
    message["username"] = username;
    
    QJsonDocument doc(message);
    m_socket->write(doc.toJson());
}

void ChatClient::sendMessage(const QString &content, const QString &receiver)
{
    QJsonObject message;
    message["type"] = "message";
    message["content"] = content;
    message["receiver"] = receiver;
    
    QJsonDocument doc(message);
    m_socket->write(doc.toJson());
}

void ChatClient::onConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void ChatClient::onDisconnected()
{
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void ChatClient::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isObject()) {
        handleServerMessage(doc.object());
    }
}

void ChatClient::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit errorOccurred(m_socket->errorString());
}

void ChatClient::handleServerMessage(const QJsonObject &message)
{
    QString type = message["type"].toString();
    
    if (type == "message") {
        QString sender = message["sender"].toString();
        QString content = message["content"].toString();
        QString receiver = message["receiver"].toString();
        QString timestamp = message["timestamp"].toString();
        
        emit messageReceived(sender, content, receiver, timestamp);
    }
    else if (type == "online_users") {
        QJsonArray usersArray = message["users"].toArray();
        QStringList users;
        for (const QJsonValue &value : usersArray) {
            users.append(value.toString());
        }
        emit onlineUsersReceived(users);
    }
    else if (type == "user_online") {
        QString username = message["username"].toString();
        emit userOnline(username);
    }
    else if (type == "user_offline") {
        QString username = message["username"].toString();
        emit userOffline(username);
    }
    else if (type == "error") {
        QString errorMsg = message["message"].toString();
        emit errorOccurred(errorMsg);
    }
}
