TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Og -D__HWC__
QMAKE_CXXFLAGS += -Og -D__HWC__

INCLUDEPATH += include/

SOURCES += main.cpp \
    png_utils.cpp

HEADERS += \
    png_utils.h \
    maxpooling.h \
    softmax.h \
    fixedpointvariables.h \
    nnarrays.h \
    image1.h \
    convolutionrelu.h \
    perceptron.h

LIBS += -lpng
