QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/undoredo

SOURCES += \
    figure.cpp \
    figure_item.cpp \
    main.cpp \
    mainwindow.cpp \
    point.cpp \
    undoredo/stask.cpp \
    undoredo/staskpool.cpp

HEADERS += \
    figure.h \
    figure_item.h \
    mainwindow.h \
    point.h \
    undoredo/stask.h \
    undoredo/staskpool.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    stylesheet.qss

RESOURCES +=
