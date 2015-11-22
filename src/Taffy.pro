QT += core
QT -= gui

TARGET = taffy
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS_DEBUG += -std=c++11 -O0
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -std=c++11

SOURCES += \
    main.cpp \
    taffy.cpp

HEADERS += \
    taffy.h

include(model/model.pri)
include(model.query/model.query.pri)
