# -------------------------------------------------
# Project created by QtCreator 2013-02-10T12:52:06
# -------------------------------------------------
TARGET = Laser
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    tlasershm.cpp \
    tlasermsg.cpp
HEADERS += mainwindow.h \
    tlasershm.h \
    ../common/control.h \
    tlasermsg.h
FORMS += mainwindow.ui
TRANSLATIONS = Laser_pl.ts
CODECFORTR = UTF-8

LIBS += -lrt
