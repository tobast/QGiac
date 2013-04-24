
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

MathDisplay::MathDisplay(giac::context* context, QWidget* parent) : QSvgWidget(parent), context(context) 
{
}
MathDisplay::MathDisplay(giac::context* context, const QString& text, QWidget* parent) : QSvgWidget(parent), context(context)
{
	setRawText(text);
}

void MathDisplay::setRawText(QString text)
{
/*
	QwtText mathtext(toMML(text), QwtText::MathMLText);
	setText(mathtext);
	resize(sizeHint());
*/
	load(QByteArray::fromRawData(toSvg(text).toStdString().c_str(), -1));
//	qDebug() << toSvg(text);
}

/*
QString MathDisplay::toMML(const QString& toConvert)
{
	giac::gen inputGen(toConvert.toStdString(), context);
	return QString(_mathml(inputGen, context).print().c_str());
}
*/

QString MathDisplay::toTex(const QString& toConvert)
{
	qDebug() << toConvert;
	giac::gen inputGen(toConvert.toStdString(), context);

//	return QString(gen2tex(inputGen, context).c_str()); // RELEASE VERSION

	/* FIXME DEBUG VERSION */
	QString out = gen2tex(inputGen, context).c_str();
	qDebug() << out;
	return out;
}

