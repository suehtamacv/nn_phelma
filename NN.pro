TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O3 -D__SIMULATION__
QMAKE_CXXFLAGS += -O3 -D__SIMULATION__

INCLUDEPATH += fixedpoint/ layers/ arrays/ png/

SOURCES += main.cpp \
    png/png_utils.cpp

HEADERS += \
    layers/convolution.h \
    pn/gpng_utils.h \
    layers/maxpooling.h \
    layers/softmax.h \
    layers/matrixmultiply.h \
    fixedpoint/fixedpointvariables.h \
    layers/relu.h \
    arrays/nnarrays.h

LIBS += -lpng
