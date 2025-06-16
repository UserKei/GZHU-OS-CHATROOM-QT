#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

class ChatServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit ChatServer(QObject *parent = nullptr);
    
    bool startServer(quint16 port = 12345);
    void stopServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onMessageReceived();
    void cleanupOldMessages();

private:
    void handleClientMessage(QTcpSocket *client, const QJsonObject &message);
    void broadcastMessage(const QString &sender, const QString &message, const QString &receiver = QString());
    void sendToClient(QTcpSocket *client, const QJsonObject &message);
    
    QList<QTcpSocket*> m_clients;
    QMap<QTcpSocket*, QString> m_clientUsers;
    QTimer *m_cleanupTimer;
};

#endif // CHATSERVER_H
