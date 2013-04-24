
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

#include "MathDisplay.h"

// STATIC INIT
bool MathDisplay::initDone = false;
KLFBackend::klfSettings MathDisplay::klfsetts;

MathDisplay::MathDisplay(giac::context* context, QWidget* parent) : QLabel(parent), context(context) 
{
	initKLF();
}
MathDisplay::MathDisplay(giac::context* context, const QString& text, QWidget* parent) : QLabel(parent), context(context)
{
	initKLF();
	setRawText(text);
}

void MathDisplay::setRawText(QString text)
{
	updateTex(toTex(text));
}

void MathDisplay::initKLF()
{
	if(!MathDisplay::initDone)
	{
		if(!KLFBackend::detectSettings(&MathDisplay::klfsetts))
		{
			QMessageBox::warning(this, tr("TeX error"), tr("Unable to find math formula rendering dependancies (LaTeX, Ghostscript, …). The formulas will be displayed in text mode."));
			// TODO implement fallback text mode
		}

		MathDisplay::initDone=true;
	}

	klfIn.dpi = 150;
	klfIn.mathmode = "\\[ ... \\]";
	klfIn.preamble = QString("\\usepackage{amssymb,amsmath,mathrsfs}");
	klfIn.fg_color = QApplication::palette().text().color().rgb();
	klfIn.bg_color = QApplication::palette().window().color().rgb();
}

QString MathDisplay::toTex(const QString& toConvert)
{
	giac::gen inputGen(toConvert.toStdString(), context);

	return QString(gen2tex(inputGen, context).c_str());
}

void MathDisplay::updateTex(const QString& texStr)
{
	klfIn.latex=texStr;
	
	KLFBackend::klfOutput output = KLFBackend::getLatexFormula(klfIn, MathDisplay::klfsetts);

	QPixmap outPixmap = QPixmap::fromImage(output.result);

	setPixmap(outPixmap);
	adjustSize();
}

