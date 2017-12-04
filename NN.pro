TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Og -D__SIMULATION__
QMAKE_CXXFLAGS += -Og -D__SIMULATION__

INCLUDEPATH += include/

SOURCES += main.cpp \
    png_utils.cpp

HEADERS += \
    convolution.h \
    png_utils.h \
    maxpooling.h \
    softmax.h \
    matrixmultiply.h \
    fixedpointvariables.h \
    relu.h \
    nnarrays.h \
    image1.h

LIBS += -lpng
