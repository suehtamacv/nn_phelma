TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O3 -D__SIMULATION__
QMAKE_CXXFLAGS += -O3 -D__SIMULATION__

INCLUDEPATH += include/

SOURCES += main.cpp \
    png_utils.cpp \
    relu.tpp \
    maxpooling.tpp \
    softmax.tpp \
    convolution.tpp \
    matrixmultiply.tpp

HEADERS += \
    convolution.h \
    png_utils.h \
    maxpooling.h \
    softmax.h \
    matrixmultiply.h \
    fixedpointvariables.h \
    relu.h

LIBS += -lpng
