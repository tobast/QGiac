
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

#ifndef MATHMMLDISPLAY
#define MATHMMLDISPLAY

#include <QApplication>
#include <QString>
#include <QImage>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QFileDialog>
#include "qtmml/qtmmlwidget.h"

#include <giac/giac.h>

class MathMmlDisplay : public QtMmlWidget
{
	Q_OBJECT

	public:
		MathMmlDisplay(giac::context* context, QWidget* parent=0);
		MathMmlDisplay(giac::context* context, const QString& text, QWidget* parent=0);

		QImage getUnthemedRender();

	public slots:
		void setRawText(QString text, const bool processMml = true);

	signals:
		void resized();
		void s_renderAvailable(const bool&);

	private: //meth
		void init();
		QString toMml(const QString& str);
	
	private:
		static const char MATHML_PREFIX[]; /* = "<!DOCTYPE math PUBLIC \"-//W3C//DTD MathML 2.0//EN\" "
			"\"http://www.w3.org/Math/DTD/mathml2/mathml2.dtd\">\n"
			"<math mode=\"display\" xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"; */
		static const char MATHML_SUFFIX[]; // = "\n</math>";
		static const QRgb RENDER_COLOR_BG = ~0;
		static const QRgb RENDER_COLOR_FG = 0xff<<24;

		giac::context* context;
		QString displayedText;

		bool needsUnthemedRender;
};

#endif//MATHMMLDISPLAY

