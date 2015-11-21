QT += core
QT -= gui

TARGET = taffy
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    taffy.cpp

HEADERS += \
    taffy.h

include(model/model.pri)
