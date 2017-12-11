TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O1
QMAKE_CXXFLAGS += -O1

INCLUDEPATH += include/

SOURCES += main.cpp

HEADERS += \
    maxpooling.h \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    perceptron.h \
    biases.h \
    kernels.h \
    meminterface.h

LIBS += -lpng
