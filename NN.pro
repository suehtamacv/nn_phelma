TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O0 -D__SIMULATION__
QMAKE_CXXFLAGS += -O0 -D__SIMULATION__

INCLUDEPATH += include/

SOURCES += main.cpp \
    png_utils.cpp

HEADERS += \
    png_utils.h \
    maxpooling.h \
    softmax.h \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    perceptron.h \
    biases.h \
    kernels.h

LIBS += -lpng
