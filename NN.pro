TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Og
QMAKE_CXXFLAGS += -Og

INCLUDEPATH += include/

SOURCES += main.cpp \
    top.cpp \
    convolutionrelu.cpp \
    png_utils.cpp

HEADERS += \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    meminterface.h \
    kernels.h \
    png_utils.h


LIBS += -lpng
