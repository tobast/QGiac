# Subproject file to compile using LaTeX display mode

DEFINES += KLF_SRC_BUILD USE_LATEX
QT += xml
unix:INCLUDEPATH += /usr/include/klfbackend

LIBS += -lklfbackend -lklftools
HEADERS += \
	MathLatexDisplay.h \
	TexRenderThread.h
SOURCES += \
	MathLatexDisplay.cpp \
	TexRenderThread.cpp
