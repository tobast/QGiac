QT += gui
TEMPLATE = app
TARGET = 
DEPENDPATH += .
QMAKE_CXXFLAGS += -std=c++11

LIBS += -lgiac -lgmp

## Select the display mode : 
include(disp_mathml.pro)
# include(disp_latex.pro)

# Input
HEADERS += 	ContextTab.h \
			GraphicalCore.h \
			CalculationWidget.h \
			MathDisplay.h \
			WizardMatrix.h

SOURCES += 	ContextTab.cpp \
			GraphicalCore.cpp \
			CalculationWidget.cpp \
			MathDisplay.cpp \
			WizardMatrix.cpp \
			main.cpp
