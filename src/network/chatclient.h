#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);
    
    bool connectToServer(const QString &host = "127.0.0.1", quint16 port = 12345);
    void disconnectFromServer();
    void sendLogin(const QString &username);
    void sendMessage(const QString &content, const QString &receiver = QString());

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &sender, const QString &content, const QString &receiver, const QString &timestamp);
    void onlineUsersReceived(const QStringList &users);
    void userOnline(const QString &username);
    void userOffline(const QString &username);
    void errorOccurred(const QString &error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    void handleServerMessage(const QJsonObject &message);
    
    QTcpSocket *m_socket;
};

#endif // CHATCLIENT_H
