
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

#ifndef DEF_MATHDISPLAY
#define DEF_MATHDISPLAY

#include <QString>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QAction>
#include <QClipboard>
#include <QFileDialog>

#include <klfbackend.h> // Display TeX

#include <string>

#include <giac/giac.h>

#include "TexRenderThread.h"

class MathDisplay : public QLabel
{
	Q_OBJECT
	public:
		MathDisplay(giac::context* context, QWidget* parent=0);
		MathDisplay(giac::context* context, const QString& text, QWidget* parent=0);

	public slots:
		void setRawText(QString text);
		void copyText();
		void copyImage();
		void saveImage();

	signals:
		void resized();

	private: //meth
		void buildActions();
		void initKLF();
//		QString toMML(const QString& toConvert);
		QString toTex(const QString& toConvert);
		void updateTex(const QString& texStr);
		void updateUnthemedTex(const QString& texStr);
		void updateTexOf(const QString& texStr, TexRenderThread* renThread, const char* renderedSlot, const bool isUnthemed);
	
	private slots:
		void texRendered(const QImage& image, const QString& errstr);
		void unthemedTexRendered(const QImage& image, const QString& errstr);

	private:
		static bool initDone; // = false
		static bool klfDisabled; // =false
		static KLFBackend::klfSettings klfsetts;

		QString rawText;
		QAction* act_copyText;
		QAction* act_copyImage;
		giac::context* context;
		TexRenderThread* renderer;

		bool needsUnthemedRender;
		TexRenderThread* unthemedRenderer;
		QImage unthemedRender;
};

#endif//DEF_MATHDISPLAY

