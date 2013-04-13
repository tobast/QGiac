
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

#include "GraphicalCore.h"

GraphicalCore::GraphicalCore(QWidget* parent) : QMainWindow(parent)
{
	buildWidget();
	addContext();
}

void GraphicalCore::buildWidget()
{
	setWindowTitle("QGiac");
	resize(800,600);
	buildCentralWidget();
}

void GraphicalCore::buildCentralWidget()
{
	centralWidget = new QWidget;
	l_main = new QVBoxLayout;
	
	sessions = new QTabWidget;
	l_main->addWidget(sessions);

	btnAddTab = new QPushButton("+");
	sessions->setCornerWidget(btnAddTab);

	btnDelTab = new QPushButton("-");
	sessions->setCornerWidget(btnDelTab, Qt::TopLeftCorner);

	centralWidget->setLayout(l_main);
	setCentralWidget(centralWidget);
	
	connect(btnAddTab, SIGNAL(clicked()), this, SLOT(addContext()));
	connect(btnDelTab, SIGNAL(clicked()), this, SLOT(delContext()));
}

void GraphicalCore::addContext()
{
	QString name = QString::number(sessions->count()+1);
	ContextTab* tab = new ContextTab(name);
	sessions->addTab(tab, name);
	tab->getCalcWidgets().back()->setFocus(Qt::OtherFocusReason);
}

void GraphicalCore::delContext()
{
	//TODO
}

