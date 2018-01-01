TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Og -D__SIMULATION__
QMAKE_CXXFLAGS += -Og -D__SIMULATION__

INCLUDEPATH += include/

SOURCES += main.cpp \
    top.cpp \
    perceptron.cpp \
    convolutionrelu.cpp \
    maxpooling.cpp

HEADERS += \
    maxpooling.h \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    perceptron.h \
    meminterface.h \
    kernels.h \
    biases.h


LIBS += -lpng
