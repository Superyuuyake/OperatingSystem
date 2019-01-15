#-------------------------------------------------
#
# Project created by QtCreator 2018-10-13T16:03:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSProject
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
    line.cpp \
    pcb.cpp \
    process.cpp \
    scheduler.cpp \
    cpu.cpp \
    shortestjobfirst.cpp \
    operatingsystemwindow.cpp \
    highestpriority.cpp \
    mailbox.cpp \
    mmu.cpp \
    multilevelfeedbackschedule.cpp \
    pagetable.cpp \
    pagetableentry.cpp \
    pipe.cpp \
    roundrobin.cpp \
    corethread.cpp \
    worker.cpp \
    underthehoodworker.cpp \
    mutexlock.cpp

HEADERS += \
        mainwindow.h \
    line.h \
    pcb.h \
    process.h \
    scheduler.h \
    cpu.h \
    shortestjobfirst.h \
    operatingsystemwindow.h \
    highestpriority.h \
    mailbox.h \
    mmu.h \
    multilevelfeedbackschedule.h \
    pagetable.h \
    pagetableentry.h \
    pipe.h \
    roundrobin.h \
    corethread.h \
    worker.h \
    underthehoodworker.h \
    mutexlock.h

FORMS += \
        mainwindow.ui \
    operatingsystemwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
