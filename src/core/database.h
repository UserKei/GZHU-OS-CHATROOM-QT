#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDateTime>

struct ChatMessage {
    QString sender;
    QString receiver;
    QString content;
    QDateTime timestamp;
    
    ChatMessage() = default;
    ChatMessage(const QString &s, const QString &r, const QString &c, const QDateTime &t)
        : sender(s), receiver(r), content(c), timestamp(t) {}
};

class Database
{
public:
    static Database& instance();
    
    bool initDatabase();
    bool createUser(const QString &username, const QString &password, const QString &email);
    bool validateUser(const QString &username, const QString &password);
    bool updateUserStatus(const QString &username, bool online);
    bool saveMessage(const QString &sender, const QString &receiver, const QString &message, const QDateTime &timestamp);
    QList<ChatMessage> getRecentMessages(const QString &username, int days = 3);
    bool deleteMessage(int messageId, const QString &username);
    bool setUserPermission(const QString &username, const QString &permission, bool enabled);
    bool getUserPermission(const QString &username, const QString &permission);
    QStringList getOnlineUsers();
    bool addSensitiveWord(const QString &word);
    QStringList getSensitiveWords();
    
private:
    Database();
    ~Database();
    
    QSqlDatabase m_database;
};

#endif // DATABASE_H
