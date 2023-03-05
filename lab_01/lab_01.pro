QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/backend/inc
INCLUDEPATH += $$PWD/frontend/inc

SOURCES += \
    backend/src/circle.cpp \
    backend/src/point.cpp \
    backend/src/vector.cpp \
    frontend/src/drawer.cpp \
    frontend/src/graphicsview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    backend/inc/circle.h \
    backend/inc/point.h \
    backend/inc/vector.h \
    frontend/inc/drawer.h \
    frontend/inc/graphicsview.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
