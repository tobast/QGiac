
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

#include "MathLatexDisplay.h"

// STATIC INIT
bool MathLatexDisplay::initDone = false;
bool MathLatexDisplay::klfDisabled = false;
KLFBackend::klfSettings MathLatexDisplay::klfsetts;

MathLatexDisplay::MathLatexDisplay(giac::context* context, QWidget* parent) :
	QLabel(parent), context(context), renderer(NULL), unthemedRenderer(NULL) 
{
	initKLF();
}
MathLatexDisplay::MathLatexDisplay(giac::context* context, const QString& text, QWidget* parent) :
	QLabel(parent), context(context), renderer(NULL), unthemedRenderer(NULL)
{
	initKLF();
	setRawText(text);
}

const QImage MathLatexDisplay::getUnthemedRender()
{
	if(needsUnthemedRender)
		return unthemedRender;
	return pixmap()->toImage();
}

void MathLatexDisplay::setRawText(QString text, const bool processLatex)
{
	rawText = text;
	renderAvailable(false);
	setText(text);
	adjustSize();

	if(processLatex)
	{
		QString tex = toTex(text);
		updateTex(tex);

		if(needsUnthemedRender)
			updateUnthemedTex(tex);
	}

	setAlignment(Qt::AlignCenter);
}

void MathLatexDisplay::initKLF()
{
	if(!MathLatexDisplay::initDone)
	{
		if(!KLFBackend::detectSettings(&MathLatexDisplay::klfsetts))
		{
			QMessageBox::warning(this, tr("TeX error"), tr("Unable to find math formula rendering dependancies (latex and ghostscript executables). The formulas will be displayed in text mode."));
			klfDisabled = true;
		}

		MathLatexDisplay::initDone=true;
	}

	// If theme colors are not black on white, we have to gen another image
	needsUnthemedRender = ! TexRenderThread::defaultMatchUsed();	
}

QString MathLatexDisplay::toTex(const QString& toConvert)
{
	giac::gen inputGen(toConvert.toStdString(), context);

	return QString(gen2tex(inputGen, context).c_str());
}

void MathLatexDisplay::updateTex(const QString& texStr)
{
	updateTexOf(texStr, renderer, SLOT(texRendered(const QImage&, const QString&)), false);
}
void MathLatexDisplay::updateUnthemedTex(const QString& texStr)
{
	updateTexOf(texStr, unthemedRenderer, SLOT(unthemedTexRendered(const QImage&, const QString&)), true);
}

void MathLatexDisplay::updateTexOf(const QString& texStr, TexRenderThread* renThread, const char* renderedSlot, const bool isUnthemed)
{
	if(klfDisabled)
		return;

	if(renThread != NULL)
	{
		renThread->terminate();
		renThread->wait();
	}

	renThread = new TexRenderThread(texStr, klfsetts, isUnthemed);
	connect(renThread, SIGNAL(resultAvailable(const QImage&, const QString&)), this, renderedSlot);
	renThread->start(QThread::LowPriority);
}

void MathLatexDisplay::texRendered(const QImage& image, const QString& errstr)
{
	if(!errstr.isEmpty())
		QMessageBox::warning(this, tr("Rendering error"), tr("The application failed to render a formula. The renderer returned:\n")+errstr);

	unthemedRender = image;
	setPixmap(QPixmap::fromImage(image));
	adjustSize();
	emit(resized());

	if(!needsUnthemedRender)
		renderAvailable(true);

	renderer=NULL;

	setAlignment(Qt::AlignCenter);
}

void MathLatexDisplay::unthemedTexRendered(const QImage& image, const QString& errstr)
{
	if(!errstr.isEmpty())
	{
		QMessageBox::warning(this, tr("Rendering error for unthemed result"),
				tr("The application failed to render a formula in its unthemed form"
					"(ie., with default backgroud/foreground colors). The renderer returned: \n")+errstr);
	}

	unthemedRender = image;
	renderAvailable(true);

	unthemedRenderer=NULL;
}

void MathLatexDisplay::renderAvailable(const bool isAvailable)
{
	emit(s_renderAvailable(isAvailable));
}

