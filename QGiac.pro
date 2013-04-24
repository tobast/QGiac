######################################################################
# Automatically generated by qmake (2.01a) ven. avr. 12 19:17:49 2013
######################################################################

DEFINES += KLF_SRC_BUILD
QT += xml
# QMAKEFEATURES += "/usr/share/qt4/mkspecs/features"
# CONFIG += qwt qwtmathml
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += $$PWD/klfbackend/include

win32: LIBS += -L$$PWD/klfbackend/win32
else:unix: LIBS += -L$$PWD/klfbackend/linux

LIBS += -lgiac -lgmp -lklfbackend -lklftools

# Input
HEADERS += 	ContextTab.h \
			GraphicalCore.h \
			CalculationWidget.h \
			MathDisplay.h

SOURCES += 	ContextTab.cpp \
			GraphicalCore.cpp \
			CalculationWidget.cpp \
			MathDisplay.cpp \
			main.cpp
