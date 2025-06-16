#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    void setupUI();
    
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_loginGroup;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;
    QLabel *m_statusLabel;
};

#endif // LOGINWINDOW_H
