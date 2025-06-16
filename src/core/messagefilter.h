#ifndef MESSAGEFILTER_H
#define MESSAGEFILTER_H

#include <QString>
#include <QStringList>

class MessageFilter
{
public:
    static MessageFilter& instance();
    
    QString filterMessage(const QString &message);
    void updateSensitiveWords();
    
private:
    MessageFilter();
    
    QStringList m_sensitiveWords;
    void loadSensitiveWords();
};

#endif // MESSAGEFILTER_H
