
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

MathDisplay::MathDisplay(giac::context* context, QWidget* parent) :
	QWidget(parent), context(context), dispText()
{
	buildWidget();
}

MathDisplay::MathDisplay(giac::context* context, const QString& text, QWidget* parent) :
	QWidget(parent), context(context), dispText()
{
	buildWidget();
	setText(text);
}

void MathDisplay::setText(const QString& text, const bool processLanguage)
{
	dispText = text;
	mathwidget->setRawText(text, processLanguage);
}


// ==== COPY ====
void MathDisplay::copyText()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setText(dispText);
}

void MathDisplay::copyLatex()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setText(toTex(dispText));
}

void MathDisplay::copyMml()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setText(toMml(dispText));
}

void MathDisplay::copyImage()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setImage(mathwidget->getUnthemedRender());
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

	if(!QPixmap::fromImage(mathwidget->getUnthemedRender()).save(filepath))
		QMessageBox::warning(this, tr("Error"), tr("Cannot save this image as %1. "
					"Please check your file name.").arg(filepath));
}
// ==== END COPY ====

void MathDisplay::buildWidget()
{
	l_main = new QVBoxLayout;
	mathwidget = new MATHWIDGET(context);
	connect(mathwidget, SIGNAL(resized()), this, SLOT(childResized()));
	connect(mathwidget, SIGNAL(s_renderAvailable(const bool&)), this, SLOT(renderAvailable(const bool&)));

	l_main->addWidget(mathwidget);
	l_main->setAlignment(Qt::AlignCenter);
	setLayout(l_main);

	buildActions();
}

void MathDisplay::buildActions()
{
	setContextMenuPolicy(Qt::ActionsContextMenu);

	act_copyText = new QAction(tr("Copy text"), this);
	connect(act_copyText, SIGNAL(triggered()), this, SLOT(copyText()));
	this->addAction(act_copyText);

	act_copyLatex = new QAction(tr("Copy LaTeX code"), this);
	connect(act_copyLatex, SIGNAL(triggered()), this, SLOT(copyLatex()));
	this->addAction(act_copyLatex);

	act_copyMml = new QAction(tr("Copy MathML code"), this);
	connect(act_copyMml, SIGNAL(triggered()), this, SLOT(copyMml()));
	this->addAction(act_copyMml);

	act_copyImage = new QAction(tr("Copy image"), this);
	connect(act_copyImage, SIGNAL(triggered()), this, SLOT(copyImage()));
	act_copyImage->setEnabled(false);
	this->addAction(act_copyImage);

	act_saveImage = new QAction(tr("Save image"), this);
	connect(act_saveImage, SIGNAL(triggered()), this, SLOT(saveImage()));
	act_saveImage->setEnabled(false);
	this->addAction(act_saveImage);
}

QString MathDisplay::toTex(const QString& str)
{
	giac::gen inputGen(str.toStdString(), context);
	return QString(gen2tex(inputGen, context).c_str());
}

QString MathDisplay::toMml(const QString& str)
{
	giac::gen inputGen(str.toStdString(), context);
	return QString(gen2mathml(inputGen, context).c_str());
}

void MathDisplay::childResized()
{
	emit(resized());
}

void MathDisplay::renderAvailable(const bool& avail)
{
	act_copyImage->setEnabled(avail);
	act_saveImage->setEnabled(avail);
}

