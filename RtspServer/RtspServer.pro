TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    _src/network.cpp
HEADERS += ./_inc/*.h \
    _inc/network.h
SOURCES += ./_src/MediaBuffer.cpp ./_src/MediaSession.cpp ./_src/RtpTrans.cpp
SOURCES += ./_src/H264Data.cpp ./_src/HardDiskRead.cpp

include(deployment.pri)
qtcAddDeployment()

