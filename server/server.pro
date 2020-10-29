TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    calculator.cpp \
    machine.cpp

HEADERS += \
    calculator.h \
    threadpool.h \
    machine.h

QMAKE_CXXFLAGS += -pthread
LIBS += -pthread
