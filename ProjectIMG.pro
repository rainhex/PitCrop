#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T14:17:13
#
#-------------------------------------------------

QT	   += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectIMG
TEMPLATE = app

RC_ICONS += icon.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000	# disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /home/zuriel/Qt/5.10.1/gcc_64/include/QtWidgets \
    /home/zuriel/Qt/5.10.1/gcc_64/include/QtGui \
    /home/zuriel/Qt/5.10.1/gcc_64/include/QtCore

SOURCES += \
		main.cpp \
		mainwindow.cpp \
    imagelist.cpp \
    cropbox.cpp \
    globals.cpp \
    util.cpp \
    cfgdialog.cpp \
    zgraphicsview.cpp

HEADERS += \
		mainwindow.h \
    imagelist.h \
    cropbox.h \
    globals.h \
    util.h \
    cfgdialog.h \
    zgraphicsview.h

FORMS += \
		mainwindow.ui \
    cfgdialog.ui
