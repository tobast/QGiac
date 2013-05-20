DEFINES += USE_MATHML
QT += gui xml
TEMPLATE = app
TARGET = 
DEPENDPATH += .
QMAKE_CXXFLAGS += -std=c++11

LIBS += -lgiac -lgmp

# Input
HEADERS += 	ContextTab.h \
			GraphicalCore.h \
			CalculationWidget.h \
			MathDisplay.h \
			MathMmlDisplay.h \
			"qtmml/qtmmlwidget.h" \
			WizardMatrix.h

SOURCES += 	ContextTab.cpp \
			GraphicalCore.cpp \
			CalculationWidget.cpp \
			MathDisplay.cpp \
			MathMmlDisplay.cpp \
			WizardMatrix.cpp \
			"qtmml/qtmmlwidget.cpp" \
			main.cpp
