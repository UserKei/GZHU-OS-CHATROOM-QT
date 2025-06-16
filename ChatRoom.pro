QT += core widgets network sql

CONFIG += c++17

TARGET = ChatRoom
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    chatclient.cpp \
    chatserver.cpp \
    userinfo.cpp \
    messagefilter.cpp \
    database.cpp \
    loginwindow.cpp \
    registerdialog.cpp \
    usermanagerdialog.cpp \
    settingsdialog.cpp

HEADERS += \
    mainwindow.h \
    chatclient.h \
    chatserver.h \
    userinfo.h \
    messagefilter.h \
    database.h \
    loginwindow.h \
    registerdialog.h \
    usermanagerdialog.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    loginwindow.ui \
    registerdialog.ui \
    usermanagerdialog.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
