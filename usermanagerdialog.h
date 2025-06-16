#ifndef USERMANAGERDIALOG_H
#define USERMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class UserManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagerDialog(const QString &username, QWidget *parent = nullptr);

private slots:
    void onPermissionChanged();
    void onAddSensitiveWord();

private:
    void setupUI();
    void loadPermissions();
    void loadSensitiveWords();
    
    QString m_username;
    
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_permissionGroup;
    QCheckBox *m_receiveMessagesCheck;
    QCheckBox *m_sendMessagesCheck;
    QCheckBox *m_deleteMessagesCheck;
    
    QGroupBox *m_sensitiveWordsGroup;
    QListWidget *m_sensitiveWordsList;
    QPushButton *m_addWordButton;
};

#endif // USERMANAGERDIALOG_H
