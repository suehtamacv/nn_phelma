TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O0 -D__SIMULATION__ -D__STAT__ -D__FLOATVERSION__
QMAKE_CXXFLAGS += -O0 -D__SIMULATION__ -D__STAT__ -D__FLOATVERSION__

INCLUDEPATH += include/

SOURCES += main.cpp

HEADERS += \
    maxpooling.h \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    perceptron.h \
    biases.h \
    kernels.h

LIBS += -lpng
