#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

Database::Database()
{
}

Database::~Database()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool Database::initDatabase()
{
    // 获取应用数据目录
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dataPath + "/chatroom.db");
    
    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    
    // 创建用户表
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE NOT NULL,"
               "password TEXT NOT NULL,"
               "email TEXT,"
               "online BOOLEAN DEFAULT 0,"
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
    
    // 创建消息表
    query.exec("CREATE TABLE IF NOT EXISTS messages ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "sender TEXT NOT NULL,"
               "receiver TEXT,"
               "message TEXT NOT NULL,"
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "deleted BOOLEAN DEFAULT 0)");
    
    // 创建用户权限表
    query.exec("CREATE TABLE IF NOT EXISTS user_permissions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT NOT NULL,"
               "permission TEXT NOT NULL,"
               "enabled BOOLEAN DEFAULT 1,"
               "UNIQUE(username, permission))");
    
    // 创建敏感词表
    query.exec("CREATE TABLE IF NOT EXISTS sensitive_words ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "word TEXT UNIQUE NOT NULL)");
    
    // 插入默认敏感词
    QStringList defaultWords = {"fuck", "shit", "damn", "hell", "傻逼", "操", "草", "卧槽"};
    for (const QString &word : defaultWords) {
        query.prepare("INSERT OR IGNORE INTO sensitive_words (word) VALUES (?)");
        query.addBindValue(word);
        query.exec();
    }
    
    return true;
}

bool Database::createUser(const QString &username, const QString &password, const QString &email)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email) VALUES (?, ?, ?)");
    query.addBindValue(username);
    
    // 密码加密
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    query.addBindValue(hash.toHex());
    query.addBindValue(email);
    
    if (!query.exec()) {
        qDebug() << "Failed to create user:" << query.lastError().text();
        return false;
    }
    
    // 为新用户设置默认权限
    setUserPermission(username, "receive_messages", true);
    setUserPermission(username, "send_messages", true);
    setUserPermission(username, "delete_own_messages", true);
    
    return true;
}

bool Database::validateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return query.value(0).toString() == hash.toHex();
}

bool Database::updateUserStatus(const QString &username, bool online)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET online = ? WHERE username = ?");
    query.addBindValue(online);
    query.addBindValue(username);
    
    return query.exec();
}

bool Database::saveMessage(const QString &sender, const QString &receiver, const QString &message, const QDateTime &timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO messages (sender, receiver, message, timestamp) VALUES (?, ?, ?, ?)");
    query.addBindValue(sender);
    query.addBindValue(receiver.isEmpty() ? QVariant() : receiver); // NULL for broadcast messages
    query.addBindValue(message);
    query.addBindValue(timestamp);
    
    return query.exec();
}

QList<ChatMessage> Database::getRecentMessages(const QString &username, int days)
{
    QList<ChatMessage> messages;
    
    QSqlQuery query;
    query.prepare("SELECT sender, receiver, message, timestamp FROM messages "
                  "WHERE (receiver IS NULL OR receiver = ? OR sender = ?) "
                  "AND deleted = 0 "
                  "AND timestamp >= datetime('now', '-' || ? || ' days') "
                  "ORDER BY timestamp ASC");
    query.addBindValue(username);
    query.addBindValue(username);
    query.addBindValue(days);
    
    if (query.exec()) {
        while (query.next()) {
            ChatMessage msg;
            msg.sender = query.value(0).toString();
            msg.receiver = query.value(1).toString();
            msg.content = query.value(2).toString();
            msg.timestamp = query.value(3).toDateTime();
            messages.append(msg);
        }
    }
    
    return messages;
}

bool Database::deleteMessage(int messageId, const QString &username)
{
    QSqlQuery query;
    query.prepare("UPDATE messages SET deleted = 1 WHERE id = ? AND sender = ?");
    query.addBindValue(messageId);
    query.addBindValue(username);
    
    return query.exec() && query.numRowsAffected() > 0;
}

bool Database::setUserPermission(const QString &username, const QString &permission, bool enabled)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO user_permissions (username, permission, enabled) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(permission);
    query.addBindValue(enabled);
    
    return query.exec();
}

bool Database::getUserPermission(const QString &username, const QString &permission)
{
    QSqlQuery query;
    query.prepare("SELECT enabled FROM user_permissions WHERE username = ? AND permission = ?");
    query.addBindValue(username);
    query.addBindValue(permission);
    
    if (query.exec() && query.next()) {
        return query.value(0).toBool();
    }
    
    return false; // 默认为false
}

QStringList Database::getOnlineUsers()
{
    QStringList users;
    
    QSqlQuery query("SELECT username FROM users WHERE online = 1");
    while (query.next()) {
        users.append(query.value(0).toString());
    }
    
    return users;
}

bool Database::addSensitiveWord(const QString &word)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO sensitive_words (word) VALUES (?)");
    query.addBindValue(word);
    
    return query.exec();
}

QStringList Database::getSensitiveWords()
{
    QStringList words;
    
    QSqlQuery query("SELECT word FROM sensitive_words");
    while (query.next()) {
        words.append(query.value(0).toString());
    }
    
    return words;
}
