
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

#include "TexRenderThread.h"

const QColor TexRenderThread::def_fgColor(0,0,0);
const QColor TexRenderThread::def_bgColor(0xff, 0xff, 0xff);

TexRenderThread::TexRenderThread(const QString& text, KLFBackend::klfSettings klfsetts, const bool defaultColors) : text(text), klfsetts(klfsetts)
{
	initKLF(defaultColors);
}

bool TexRenderThread::defaultMatchUsed()
{
	return (TexRenderThread::def_fgColor == QApplication::palette().text().color()) &&
		(TexRenderThread::def_bgColor == QApplication::palette().window().color());
}

void TexRenderThread::initKLF(const bool& defaultColors)
{
	klfIn.dpi = 150;
	klfIn.mathmode = "\\[ ... \\]";
	klfIn.preamble = QString("\\usepackage{amssymb,amsmath,mathrsfs}");

	if(defaultColors)
	{
		klfIn.fg_color = def_fgColor.rgb();	// Defaulting to white
		klfIn.bg_color = def_bgColor.rgb();	// Defaulting to black
	}
	else
	{
		klfIn.fg_color = QApplication::palette().text().color().rgb();
		klfIn.bg_color = QApplication::palette().window().color().rgb();
	}
}

void TexRenderThread::run()
{
	klfIn.latex=text;
	klfOut = KLFBackend::getLatexFormula(klfIn, klfsetts);
	
	emit resultAvailable(klfOut.result, (klfOut.status == 0) ? "" : klfOut.errorstr);
}

