
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

#include "CalculationWidget.h"

CalculationWidget::CalculationWidget(giac::context* context, const int& id) : context(context), id(id)
{
	buildWidget();
	setFocusPolicy(Qt::StrongFocus);
}

void CalculationWidget::focusInEvent(QFocusEvent* event)
{
	QFrame::focusInEvent(event);
	inputLine->setFocus(event->reason());
}

void CalculationWidget::buildWidget()
{
	setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	setLineWidth(2);
	setMidLineWidth(3);

	l_main = new QVBoxLayout;
	l_upperLine = new QHBoxLayout;

	idLabel = new QLabel(QString::number(id));
	l_upperLine->addWidget(idLabel);
	inputLine = new QLineEdit;
	l_upperLine->addWidget(inputLine);
	l_main->addLayout(l_upperLine);

	outputDisp = new MathDisplay(context);
	outputDisp->setAlignment(Qt::AlignCenter);
	connect(outputDisp, SIGNAL(resized()), this, SLOT(subWidgetResized()));
	l_main->addWidget(outputDisp);

	setLayout(l_main);

	l_main->setAlignment(Qt::AlignTop);

	connect(inputLine, SIGNAL(returnPressed()), this, SLOT(compute()));
}

void CalculationWidget::compute() // slot
{
	if(inputLine->text().isEmpty())
		return;

	try {
		giac::gen input(inputLine->text().toStdString(), context);
		QString out(input.eval(1, context).print().c_str());
		outputDisp->setRawText(out);

	} catch(const std::runtime_error& e) {
		outputDisp->setRawText(e.what());
	}
	emit(computedExpr(id));
}

void CalculationWidget::subWidgetResized()
{
	emit(resized());
}

