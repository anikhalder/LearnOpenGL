TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

INCLUDEPATH += $$PWD/../common

SOURCES += \
    main.cpp \
    ../common/src/glad.c

HEADERS += \
    ../common/glad/glad.h \
    ../common/KHR/khrplatform.h \
    ../common/stb_image.h \
    ../common/shader.h

DISTFILES += \
    hello_transformation.vert \
    hello_transformation.frag \
