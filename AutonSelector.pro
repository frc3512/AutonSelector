QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = AutonSelector
TEMPLATE = app
CONFIG += c++1z

win32:LIBS += -lws2_32

CONFIG += debug_and_release

SOURCES += \
    src/Main.cpp \
    src/MainWindow.cpp \
    src/Settings.cpp

HEADERS  += \
    src/MainWindow.hpp \
    src/Settings.hpp

RESOURCES += \
    AutonSelector.qrc

DISTFILES += \
    Resources.rc

LIBS += -ljpeg
