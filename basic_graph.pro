QMAKE_CXXFLAGS += -Werror
CONFIG += degub warn_off
HEADERS       = window.h \
    functions.h
SOURCES       = main.cpp \
                window.cpp \
    functions.cpp
QT += widgets
