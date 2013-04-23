
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

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

#include <qwt/qwt.h>
#include <qwt/qwt_mathml_text_engine.h>

#include "GraphicalCore.h"

int main(int argc, char** argv)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8")); // Strings in files are UTF-8 encoded
	QwtText::setTextEngine(QwtText::MathMLText, new QwtMathMLTextEngine()); // Setting Qwt text engine to MathML

	QApplication a(argc, argv);

	QString locale = QLocale::system().name().section('_', 0, 0); // Installs a translator to the user's locale
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&translator);

	GraphicalCore c;
	c.show();

	return a.exec();
}

