TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Og
QMAKE_CXXFLAGS += -Og

INCLUDEPATH += include/

SOURCES += main.cpp \
    convolution.cpp \
    png_utils.cpp

HEADERS += \
    convolution.h \
    png_utils.h

LIBS += -lpng
