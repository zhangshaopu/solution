QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QT += printsupport
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MainWindowSlot.cpp \
    aboutdialog.cpp \
    appconfig.cpp \
    dlinklist.cpp \
    dlnode.cpp \
    finddialog.cpp \
    linklistform.cpp \
    lnode.cpp \
    main.cpp \
    mainwindow.cpp \
    myarrowitem.cpp \
    textedit.cpp

HEADERS += \
    MyArrowItem.h \
    aboutdialog.h \
    appconfig.h \
    dlinklist.h \
    dlnode.h \
    finddialog.h \
    linklistform.h \
    lnode.h \
    mainwindow.h \
    myarrowitem.h \
    textedit.h \
    uidefine.h

FORMS += \
    dlinklist.ui \
    linklistform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../qtworkspace/QScintilla_src-2.12.1/build-qscintilla-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/release/ -lqscintilla2_qt5d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../qtworkspace/QScintilla_src-2.12.1/build-qscintilla-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/ -lqscintilla2_qt5d

INCLUDEPATH += $$PWD/../../qtworkspace/QScintilla_src-2.12.1/build-qscintilla-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/../../qtworkspace/QScintilla_src-2.12.1/build-qscintilla-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
