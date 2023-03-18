QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    core.h \
    mainwindow.h \
    util.h



FORMS += \
    mainwindow.ui



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lWordList
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lWordList
else:unix: LIBS += -L$$PWD/./ -lWordList

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
