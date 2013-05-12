
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
bool MathDisplay::klfDisabled = false;
KLFBackend::klfSettings MathDisplay::klfsetts;

MathDisplay::MathDisplay(giac::context* context, QWidget* parent) :
	QLabel(parent), context(context), renderer(NULL), unthemedRenderer(NULL) 
{
	initKLF();
	buildActions();
}
MathDisplay::MathDisplay(giac::context* context, const QString& text, QWidget* parent) :
	QLabel(parent), context(context), renderer(NULL), unthemedRenderer(NULL)
{
	initKLF();
	buildActions();
	setRawText(text);
}

void MathDisplay::setRawText(QString text)
{
	rawText = text;
	renderAvailable(false);
	setText(text);
	adjustSize();

	QString tex = toTex(text);
	updateTex(tex);

	if(needsUnthemedRender)
		updateUnthemedTex(tex);
}

void MathDisplay::copyText()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setText(rawText);
}

void MathDisplay::copyImage()
{
	if(pixmap() == 0)
		return;
	QClipboard *cb = QApplication::clipboard();

	cb->setImage(unthemedRender);
}

void MathDisplay::saveImage()
{
	QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."), QDir::homePath(),
			tr("Images (*.png *.jpg *.bmp)"));	// TODO improve default dir (go to last used one)
	if(filepath.isEmpty())
		return;

	int lastSlash = filepath.lastIndexOf('/');
	int lastDot = filepath.lastIndexOf('.');

	if(lastDot == -1 || (lastSlash >= lastDot))
		filepath.append(".png");

	if(!QPixmap::fromImage(unthemedRender).save(filepath))
		QMessageBox::warning(this, tr("Error"), tr("Cannot save this image as %1. Please check your file name.").arg(filepath));
}

void MathDisplay::buildActions()
{
	setContextMenuPolicy(Qt::ActionsContextMenu);

	act_copyText = new QAction(tr("Copy text"), this);
	connect(act_copyText, SIGNAL(triggered()), this, SLOT(copyText()));
	this->addAction(act_copyText);

	act_copyImage = new QAction(tr("Copy image"), this);
	connect(act_copyImage, SIGNAL(triggered()), this, SLOT(copyImage()));
	act_copyImage->setEnabled(false);
	this->addAction(act_copyImage);

	act_saveImage = new QAction(tr("Save image"), this);
	connect(act_saveImage, SIGNAL(triggered()), this, SLOT(saveImage()));
	act_saveImage->setEnabled(false);
	this->addAction(act_saveImage);
}

void MathDisplay::initKLF()
{
	if(!MathDisplay::initDone)
	{
		if(!KLFBackend::detectSettings(&MathDisplay::klfsetts))
		{
			QMessageBox::warning(this, tr("TeX error"), tr("Unable to find math formula rendering dependancies (latex and ghostscript executables). The formulas will be displayed in text mode."));
			klfDisabled = true;
		}

		MathDisplay::initDone=true;
	}

	// If theme colors are not black on white, we have to gen another image
	needsUnthemedRender = ! TexRenderThread::defaultMatchUsed();	
}

QString MathDisplay::toTex(const QString& toConvert)
{
	giac::gen inputGen(toConvert.toStdString(), context);

	return QString(gen2tex(inputGen, context).c_str());
}

void MathDisplay::updateTex(const QString& texStr)
{
	updateTexOf(texStr, renderer, SLOT(texRendered(const QImage&, const QString&)), false);
}
void MathDisplay::updateUnthemedTex(const QString& texStr)
{
	updateTexOf(texStr, unthemedRenderer, SLOT(unthemedTexRendered(const QImage&, const QString&)), true);
}

void MathDisplay::updateTexOf(const QString& texStr, TexRenderThread* renThread, const char* renderedSlot, const bool isUnthemed)
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

void MathDisplay::texRendered(const QImage& image, const QString& errstr)
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
}

void MathDisplay::unthemedTexRendered(const QImage& image, const QString& errstr)
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

void MathDisplay::renderAvailable(const bool isAvailable)
{
	act_copyImage->setEnabled(isAvailable);
	act_saveImage->setEnabled(isAvailable);
}

