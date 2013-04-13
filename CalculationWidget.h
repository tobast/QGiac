
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

#ifndef DEF_CALCULATIONWIDGET
#define DEF_CALCULATIONWIDGET

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFocusEvent>

#include <giac/giac.h>

#include <stdexcept>

class CalculationWidget : public QFrame
{
	Q_OBJECT
	public:
		CalculationWidget(giac::context* context, const int& id=-1);

	protected: //meth
		void focusInEvent(QFocusEvent* event);
	
	private: //meth
		void buildWidget();

	private slots:
		void compute();

	signals:
		void computedExpr(const int& id);

	private:
		QVBoxLayout* l_main;
		QHBoxLayout* l_upperLine;

		QLabel* idLabel;
		QLineEdit* inputLine;
		QLineEdit* outputLine; // TODO replace with LaTeX output.

		giac::context* context;
		int id;
};

#endif//DEF_CALCULATIONWIDGET

