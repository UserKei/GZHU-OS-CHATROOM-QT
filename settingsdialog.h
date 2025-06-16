#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QString &username, QWidget *parent = nullptr);

private slots:
    void onChangePassword();
    void onSaveSettings();

private:
    void setupUI();
    void loadSettings();
    
    QString m_username;
    
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_accountGroup;
    QLineEdit *m_currentPasswordEdit;
    QLineEdit *m_newPasswordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QPushButton *m_changePasswordButton;
    
    QGroupBox *m_chatGroup;
    QSpinBox *m_messageHistoryDays;
    
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
};

#endif // SETTINGSDIALOG_H
