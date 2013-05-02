
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

GraphicalCore::GraphicalCore(QWidget* parent) : QMainWindow(parent), nextSessionId(1)
{
	buildWidget();
	addContext();
}

void GraphicalCore::buildWidget()
{
	setWindowTitle("QGiac");
	resize(800,600);

	buildCentralWidget();
	buildMenuBar();
}

void GraphicalCore::buildMenuBar()
{
	menubar = new QMenuBar;
	setMenuBar(menubar);

	menu_file = menubar->addMenu(tr("File"));

	act_addContext = menu_file->addAction(tr("New context"), this, SLOT(addContext()), QKeySequence(QKeySequence::AddTab));
	act_delContext = menu_file->addAction(tr("Close context"), this, SLOT(delCurContext()), QKeySequence(QKeySequence::Close));

	menu_file->addSeparator();

	QKeySequence seq(QKeySequence::Quit);
	if(seq.isEmpty())
		seq=QKeySequence("Ctrl+Q");
	act_quit = menu_file->addAction(tr("Quit"), this, SLOT(quitOnAction()), seq);
}

void GraphicalCore::buildCentralWidget()
{
	centralWidget = new QWidget;
	l_main = new QVBoxLayout;
	
	sessions = new QTabWidget;
	l_main->addWidget(sessions);
	sessions->setTabsClosable(true);

	btnAddTab = new QPushButton("+"); //FIXME add a real icon
	sessions->setCornerWidget(btnAddTab);

	centralWidget->setLayout(l_main);
	setCentralWidget(centralWidget);
	
	connect(btnAddTab, SIGNAL(clicked()), this, SLOT(addContext()));
	connect(sessions, SIGNAL(tabCloseRequested(int)), this, SLOT(delContext(const int&)));
}

void GraphicalCore::addContext()
{
	QString name = QString::number(nextSessionId);
	nextSessionId++;
	ContextTab* tab = new ContextTab(name);
	sessions->addTab(tab, name);
	sessions->setCurrentIndex(sessions->count()-1);
	tab->getCalcWidgets().back()->setFocus(Qt::OtherFocusReason);
}

void GraphicalCore::delCurContext()
{
	delContext(sessions->currentIndex());
}

void GraphicalCore::delContext(const int& tabId)
{
	QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Close context?"), tr("Are you sure you want to close this context? All the calculation done will be lost."), QMessageBox::Yes | QMessageBox::No);
	if(btn != QMessageBox::Yes)
		return;
	
	ContextTab* context = dynamic_cast<ContextTab*>(sessions->widget(tabId));
	if(context == 0)
		return; // cannot find the context

	context->endSession();
	sessions->removeTab(tabId);
	
	if(sessions->count() == 0)
		addContext();
}

void GraphicalCore::quitOnAction()
{
	if(QMessageBox::question(this, tr("Are you sure?"), tr("Do you really want to quit QGiac? Your session will not be saved."),
							QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes)
		qApp->quit();
}

