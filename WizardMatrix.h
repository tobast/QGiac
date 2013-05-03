
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

#ifndef DEF_WIZARDMATRIX
#define DEF_WIZARDMATRIX

#include <QMainWindow> // Shortcuts, menus, ...
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>

class WizardMatrix : public QMainWindow
{
	Q_OBJECT
	
	public:
		WizardMatrix(QWidget* parent=0);
		WizardMatrix(const int& nbRows, const int& nbCols, QWidget* parent=0);

		QString toText();

		static void open();

	signals:
		void resultAvailable(QString);
		void aboutToClose();

	private slots:
		void addRow();
		void addCol();
		void copyMatrix();

	private: //meth
		void buildWidget(const int& nbRows=1, const int& nbCols=1);
		QLineEdit* lineEditAllocator();
		QLineEdit* getLineEdit(const int& row, const int& col);

	private:
		QWidget* centralWidget;
		QVBoxLayout* l_main;

		QGridLayout* l_grid;
		QPushButton* btn_addRow;
		QPushButton* btn_addCol;

		QPushButton* btn_copy;
};

#endif//DEF_WIZARDMATRIX

