TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        algorithms.cpp \
        functions.cpp \
        main.cpp \
        maze.cpp \
        state.cpp \
        summary.cpp

HEADERS += \
    algorithms.h \
    functions.h \
    maze.h \
    state.h \
    summary.h
