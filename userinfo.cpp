#include "userinfo.h"

UserInfo::UserInfo()
    : m_online(false)
    , m_lastSeen(QDateTime::currentDateTime())
{
}

UserInfo::UserInfo(const QString &username, const QString &email, bool online)
    : m_username(username)
    , m_email(email)
    , m_online(online)
    , m_lastSeen(QDateTime::currentDateTime())
{
}
