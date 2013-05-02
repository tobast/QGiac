
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

#ifndef DEF_GRAPHICALCORE
#define DEF_GRAPHICALCORE

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QMenu>

#include "ContextTab.h"

class GraphicalCore : public QMainWindow
{
	Q_OBJECT
	public:
		GraphicalCore(QWidget* parent=0);

	private: //meth
		void buildWidget();
		void buildMenuBar();
		void buildCentralWidget();

	private slots:
		void addContext();
		void delCurContext();
		void delContext(const int& tabId);
		void quitOnAction();

	private:
		// Actions
		QMenuBar* menubar;
		
		QMenu* menu_file;
		QAction* act_addContext;
		QAction* act_delContext;
		QAction* act_quit;
	
		// Central widget
		QWidget* centralWidget;
		QVBoxLayout* l_main;
		QTabWidget* sessions;
		QPushButton* btnAddTab;

		unsigned int nextSessionId; // If, by chance, it reaches its limit, the unsigned type will make it cycle back to 0.
};

#endif//DEF_GRAPHICALCORE

