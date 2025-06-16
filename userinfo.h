#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QDateTime>

class UserInfo
{
public:
    UserInfo();
    UserInfo(const QString &username, const QString &email, bool online = false);
    
    QString username() const { return m_username; }
    void setUsername(const QString &username) { m_username = username; }
    
    QString email() const { return m_email; }
    void setEmail(const QString &email) { m_email = email; }
    
    bool isOnline() const { return m_online; }
    void setOnline(bool online) { m_online = online; }
    
    QDateTime lastSeen() const { return m_lastSeen; }
    void setLastSeen(const QDateTime &lastSeen) { m_lastSeen = lastSeen; }

private:
    QString m_username;
    QString m_email;
    bool m_online;
    QDateTime m_lastSeen;
};

#endif // USERINFO_H
