TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O1
QMAKE_CXXFLAGS += -O1

INCLUDEPATH += include/

SOURCES += main.cpp \
    top.cpp

HEADERS += \
    maxpooling.h \
    fixedpointvariables.h \
    nnarrays.h \
    convolutionrelu.h \
    perceptron.h \
    meminterface.h \
    kernel_conv1.h \
    kernel_conv2.h \
    kernel_conv3.h \
    kernel_percep4.h \
    bias_conv1.h \
    bias_conv2.h \
    bias_conv3.h \
    bias_percep4.h


LIBS += -lpng
