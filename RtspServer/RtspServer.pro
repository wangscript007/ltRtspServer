TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    _src/network.cpp
HEADERS += ./_inc/*.h \
    _inc/network.h
SOURCES += ./_src/h264MediaStream.cpp ./_src/MediaBuffer.cpp ./_src/MediaCreateSdp.cpp \
    ./_src/MediaSession.cpp ./_src/MediaStream.cpp ./_src/RtspString.cpp ./_src/StreamBuilder.cpp
INCLUDEPATH += /usr/include ./smart_ptr ./_inc
LIBS += -L//usr/lib/ -levent -lpthread

include(deployment.pri)
qtcAddDeployment()

