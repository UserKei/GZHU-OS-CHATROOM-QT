#include "messagefilter.h"
#include "database.h"

MessageFilter& MessageFilter::instance()
{
    static MessageFilter instance;
    return instance;
}

MessageFilter::MessageFilter()
{
    loadSensitiveWords();
}

void MessageFilter::loadSensitiveWords()
{
    m_sensitiveWords = Database::instance().getSensitiveWords();
}

void MessageFilter::updateSensitiveWords()
{
    loadSensitiveWords();
}

QString MessageFilter::filterMessage(const QString &message)
{
    QString filteredMessage = message;
    
    for (const QString &word : m_sensitiveWords) {
        if (filteredMessage.contains(word, Qt::CaseInsensitive)) {
            QString replacement = QString("*").repeated(word.length());
            filteredMessage.replace(word, replacement, Qt::CaseInsensitive);
        }
    }
    
    return filteredMessage;
}
