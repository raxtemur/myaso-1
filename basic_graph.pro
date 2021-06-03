QMAKE_CXXFLAGS += -Werror
CONFIG += debug
HEADERS       = window.h \
    functions.h
SOURCES       = main.cpp \
                window.cpp \
    functions.cpp
QT += widgets
