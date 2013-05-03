
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

#include "WizardMatrix.h"

WizardMatrix::WizardMatrix(QWidget* parent) : QMainWindow(parent)
{
	buildWidget();
}
WizardMatrix::WizardMatrix(const int& nbRows, const int& nbCols, QWidget* parent) : QMainWindow(parent)
{
	buildWidget((nbRows < 1) ? 1 : nbRows, (nbCols < 1) ? 1 : nbCols);
}

QString WizardMatrix::toText()
{
	QString out="[";

	for(int row=1; row < l_grid->rowCount()-1; row++)
	{
		out+="[";
		for(int col=1; col < l_grid->columnCount()-1; col++)
		{
			QString cell = getLineEdit(row,col)->text();
			if(cell.contains("[") || cell.contains("]"))
			{
				QMessageBox::warning(this, tr("Invalid value"), QString(tr("Your cell %i;%i contains a '[' or ']' forbidden character.")).arg(row).arg(col));
				return "";
			}
			out+=cell;

			if(col+1 < l_grid->columnCount()-1)
				out+=",";
		}
		if(row+1 < l_grid->rowCount()-1)
			out+="],";
		else
			out+="]";
	}
	out+="]";
	return out;
}

void WizardMatrix::open()
{
	WizardMatrix* mw = new WizardMatrix;
	mw->show();
}

void WizardMatrix::addRow()
{
	int curRow = l_grid->rowCount()-1;

	l_grid->addWidget(new QLabel(QString::number(curRow)), curRow, 0);
	l_grid->addWidget(btn_addRow, curRow+1,0);
	for(int col=1; col < l_grid->columnCount()-1; col++)
		l_grid->addWidget(lineEditAllocator(), curRow, col);
}

void WizardMatrix::addCol()
{
	int curCol = l_grid->columnCount()-1;

	l_grid->addWidget(new QLabel(QString::number(curCol)), 0,curCol);
	l_grid->addWidget(btn_addCol, 0,curCol+1);
	for(int row=1; row < l_grid->rowCount()-1; row++)
		l_grid->addWidget(lineEditAllocator(), row, curCol);
}

void WizardMatrix::copyMatrix()
{
	QClipboard* cb = QApplication::clipboard();
	QString text=toText();
	if(text.isEmpty())
		return;
	cb->setText(text);
}

void WizardMatrix::buildWidget(const int& nbRows, const int& nbCols)
{
	centralWidget = new QWidget;
	l_main = new QVBoxLayout;

	l_grid = new QGridLayout;

	// Legends
	for(int col=1; col <= nbCols; col++)
		l_grid->addWidget(new QLabel(QString::number(col)), 0, col);
	for(int row=1; row <= nbCols; row++)
		l_grid->addWidget(new QLabel(QString::number(row)), row, 0, 1,1, Qt::AlignRight);

	// text fields 
	for(int row=1; row <= nbRows; row++)
		for(int col=1; col <= nbCols; col++)
			l_grid->addWidget(lineEditAllocator(), row,col);

	btn_addRow = new QPushButton("+");
	btn_addRow->setMaximumWidth(30);
	connect(btn_addRow, SIGNAL(clicked()), this, SLOT(addRow()));
	btn_addCol = new QPushButton("+");
	btn_addCol->setMaximumWidth(30);
	connect(btn_addCol, SIGNAL(clicked()), this, SLOT(addCol()));

	l_grid->addWidget(btn_addRow, nbRows+1, 0);
	l_grid->addWidget(btn_addCol, 0, nbCols+1);

	btn_copy = new QPushButton(tr("Copy matrix to clipboard"));
	connect(btn_copy, SIGNAL(clicked()), this, SLOT(copyMatrix()));

	l_main->addLayout(l_grid);

	btn_copy = new QPushButton(tr("Copy matrix to clipboard"));
	connect(btn_copy, SIGNAL(clicked()), this, SLOT(copyMatrix()));
	l_main->addWidget(btn_copy);

	centralWidget->setLayout(l_main);
	setCentralWidget(centralWidget);
}

QLineEdit* WizardMatrix::lineEditAllocator()
{
	QLineEdit* le = new QLineEdit;
	le->setMinimumWidth(20);
	le->setMaximumWidth(30);
	return le;
}

QLineEdit* WizardMatrix::getLineEdit(const int& row, const int& col)
{
	QWidget* wid = l_grid->itemAtPosition(row,col)->widget();
	QLineEdit* le = static_cast<QLineEdit*>(wid);
	if(le == 0)
		return NULL;
	return le;
}

