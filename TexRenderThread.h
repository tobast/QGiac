
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

#ifndef DEF_TEXRENDERTHREAD
#define DEF_TEXRENDERTHREAD

#include <klfbackend.h>
#include <QString>
#include <QImage>
#include <QThread>
#include <QApplication>

class TexRenderThread : public QThread
{
	Q_OBJECT

	public:
		TexRenderThread(const QString& text, KLFBackend::klfSettings klfsetts);

	signals:
		void resultAvailable(const QImage& img, const QString& errstr);

	private:// meth
		void initKLF();
		void run();

	private:
		QString text;
		KLFBackend::klfSettings klfsetts;
		KLFBackend::klfInput klfIn;
		KLFBackend::klfOutput klfOut; // As long as the class is not deleted, the result persists.
};

#endif//DEF_TEXRENDERTHREAD

