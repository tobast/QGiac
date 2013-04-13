
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

#include "ContextTab.h"

ContextTab::ContextTab(QString name) : contextName(name), context(name.toStdString())
{
	buildWidget();
}

void ContextTab::endSession() // Called on session close
{
	// Maybe some day we'll need this.
}

void ContextTab::computedExpr(const int& id)
{
	if(id == calcWidgets.size())
		addCalcWidget();
}

void ContextTab::buildWidget()
{
	containerWidget=new QWidget;
	l_main = new QVBoxLayout;
	containerWidget->setLayout(l_main);
	l_main->setAlignment(Qt::AlignTop);

	setWidget(containerWidget);
	setWidgetResizable(true);

	addCalcWidget();
}

void ContextTab::addCalcWidget(bool setFocus)
{
	CalculationWidget* calcWid = new CalculationWidget(&context, calcWidgets.size()+1);
	l_main->addWidget(calcWid);
	calcWidgets.append(calcWid);

	connect(calcWid, SIGNAL(computedExpr(const int&)), this, SLOT(computedExpr(const int&)));

	widget()->resize(widget()->sizeHint());

	if(setFocus)
	{
		qApp->processEvents();
		calcWid->setFocus(Qt::OtherFocusReason);
		ensureVisible(0,containerWidget->height(),0,0);
	}
}

