QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/Users/toupist/Downloads/Qt-Advanced-Stylesheets-main/src

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circle.cpp \
    ellipse.cpp \
    main.cpp \
    mainwindow.cpp \
    pixel.cpp \
    stask.cpp \
    staskpool.cpp

HEADERS += \
    algoTypes.h \
    circle.h \
    ellipse.h \
    mainwindow.h \
    pixel.h \
    stask.h \
    staskpool.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    lab_03_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
