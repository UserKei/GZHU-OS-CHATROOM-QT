#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

private slots:
    void onRegisterClicked();

private:
    void setupUI();
    
    QVBoxLayout *m_mainLayout;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QLineEdit *m_emailEdit;
    QPushButton *m_registerButton;
    QPushButton *m_cancelButton;
    QLabel *m_statusLabel;
};

#endif // REGISTERDIALOG_H
