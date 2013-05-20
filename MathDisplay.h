
/*
 * SOFTWARE:
 *   QGiac
 * 
 * AUTHOR:
 *   Theophile BASTIAN "Tobast" <contact@tobast.fr>
 *
 * WEBSITE:
 *   http://tobast.fr/
 *
 * LICENCE:
 *   GNU GPL v3
 *
 * DESCRIPTION:
 *   QGiac is an interface to the command-line CAS software Giac.
 *
 * CREDITS:
 *   All credits goes to Bernard PARISSE <bernard.parisse@ujf-grenoble.fr> for Giac, a library which does all the CAS stuff.
 *
 * LICENCE HEADER:
 *   Copyright (C) 2013 BASTIAN Theophile
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/gpl.txt>.
 */

/// Selects the math displayer, interfaces it, and implements right-click menu

#ifndef MATH_DISPLAY
#define MATH_DISPLAY

/// Choose your displayer here
// DO NOT SELECT MORE THAN ONE.
// Selected in .pro by default
//#define USE_MATHML
//#define USE_LATEX

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QAction>
#include <QClipboard>

#include <giac/gen.h>

#ifdef USE_MATHML
	#include "MathMmlDisplay.h"
	#define MATHWIDGET MathMmlDisplay
#elif defined USE_LATEX
	#include "MathLatexDisplay.h"
	#define MATHWIDGET MathLatexDisplay
#endif

class MathDisplay : public QWidget
{
	Q_OBJECT
	public:
		MathDisplay(giac::context* context, QWidget* parent=0);
		MathDisplay(giac::context* context, const QString& text, QWidget* parent=0);

	public slots:
		void setText(const QString& text, const bool processLanguage=true);
		void copyText();
		void copyLatex();
		void copyMml();
		void copyImage();
		void saveImage();

	signals:
		void resized();

	private: //meth
		void buildWidget();
		void buildActions();
		QString toTex(const QString& str);
		QString toMml(const QString& str);

	private slots:
		void childResized();
		void renderAvailable(const bool& avail);

	private:
		giac::context* context;

		QVBoxLayout* l_main;
		MATHWIDGET* mathwidget;

		QString dispText;

		QAction* act_copyText;
		QAction* act_copyLatex;
		QAction* act_copyMml;
		QAction* act_copyImage;
		QAction* act_saveImage;
};

#endif//MATH_DISPLAY

