#pragma once

// Qt
#include <QDialog>

// Project
#include "ui_AboutDialog.h"

class AboutDialog 
	: public QDialog
	, public Ui_AboutDialog
{
	Q_OBJECT

public:
			AboutDialog(QWidget *parent = 0);
	virtual ~AboutDialog();

};
