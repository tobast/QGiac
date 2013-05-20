
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

#include "MathMmlDisplay.h"

// === static const init ========
const char MathMmlDisplay::MATHML_PREFIX[] =
	"<!DOCTYPE math PUBLIC \"-//W3C//DTD MathML 2.0//EN\" "
	"\"http://www.w3.org/Math/DTD/mathml2/mathml2.dtd\">\n"
	"<math mode=\"display\" xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";
const char MathMmlDisplay::MATHML_SUFFIX[] = "\n</math>";
// === END static const init ====

MathMmlDisplay::MathMmlDisplay(giac::context* context, QWidget* parent) :
	QtMmlWidget(parent), context(context), displayedText()
{
	init();
}
MathMmlDisplay::MathMmlDisplay(giac::context* context, const QString& text, QWidget* parent) :
	QtMmlWidget(parent), context(context), displayedText(text)
{
	init();
	setRawText(text);
}

QImage MathMmlDisplay::getUnthemedRender()
{
	if(needsUnthemedRender)
	{
		QtMmlWidget renderWid;

		QPalette palette = QApplication::palette();
		palette.setColor(QPalette::WindowText, QColor(RENDER_COLOR_FG));
		palette.setColor(QPalette::Window, QColor(RENDER_COLOR_BG));
		renderWid.setPalette(palette);

		setMmlContent(displayedMml, &renderWid);
		renderWid.setFixedSize(renderWid.sizeHint());

		return QPixmap::grabWidget(&renderWid).toImage();
	}

	return QPixmap::grabWidget(this).toImage();
}

void MathMmlDisplay::setRawText(QString text, const bool processMml)
{
	displayedText = text;

	if(!processMml)
		text = QString("<mtext>%1</mtext>").arg(text);
	else
		text = toMml(text);

	displayedMml = text;
	setMmlContent(text);
	
	setFixedSize(sizeHint());
	emit(resized());
	emit(s_renderAvailable(true));
}

void MathMmlDisplay::init()
{
	// needsUnthemedRender
	needsUnthemedRender = ( (MathMmlDisplay::RENDER_COLOR_FG != QApplication::palette().text().color().rgb()) ||
			(MathMmlDisplay::RENDER_COLOR_BG == QApplication::palette().window().color().rgb()) );

	setFixedSize(0,0);
	emit(resized());
}

QString MathMmlDisplay::toMml(const QString& str)
{
	giac::gen inputGen(str.toStdString(), context);
	return QString(gen2mathml(inputGen, context).c_str());
}

void MathMmlDisplay::setMmlContent(const QString& text, QtMmlWidget* wid)
{
	if(wid == NULL)
		wid = this;

	QString mathml = MATHML_PREFIX + text + MATHML_SUFFIX;
	QString errstr;
	int row,col;
	if(!wid->setContent(mathml, &errstr, &row, &col))
	{
		QString errline = mathml.section('\n', row-1, row-1);
		QMessageBox::warning(this, tr("Error"), tr("Could not render math expression %1. MathMl renderer returned:\n"
					"%2\n"
					"at pos %3 in line:\n"
					"%4.").arg(text).arg(errstr).arg(col).arg(errline));
	}
}

