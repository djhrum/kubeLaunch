#-------------------------------------------------
#
# Project created by QtCreator 2019-03-21T23:46:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kubelaunch
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        logwindow.cpp

HEADERS += \
        mainwindow.h \
    logwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

#FORMS +=

RC_ICONS = logo.ico

#static { # everything below takes effect with CONFIG ''= static
# CONFIG+= static
# CONFIG += staticlib # this is needed if you create a static library, not a static executable
# DEFINES+= STATIC
# message("~~~ static build ~~~") # this is for information, that the static build is done
# mac: TARGET = $$join(TARGET,,,_static) #this adds an _static in the end, so you can seperate static build from non static build
# win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from non static build
#}
