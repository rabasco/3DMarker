#-------------------------------------------------
#
# Project created by QtCreator 2012-12-02T20:40:08
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DMarker
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    vertex.cpp \
    poly.cpp \
    glwidget.cpp \
    model.cpp \
    bookmark.cpp \
    bookmarklist.cpp \
    plyimporter.cpp

HEADERS  += mainwindow.h \
    vertex.h \
    poly.h \
    glwidget.h \
    model.h \
    bookmark.h \
    bookmarklist.h \
    plyimporter.h \
    modelimporter.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
