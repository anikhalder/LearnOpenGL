TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

SOURCES += \
        main.cpp \
    src/glad.c

HEADERS += \
    glad/glad.h \
    KHR/khrplatform.h
