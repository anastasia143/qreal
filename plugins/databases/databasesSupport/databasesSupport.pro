QT += xml

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

HEADERS += \
    databasesSupportPlugin.h

SOURCES += \
	databasesSupportPlugin.cpp

OTHER_FILES += \
    databasesSupport.pri

include(databasesSupport.pri)
