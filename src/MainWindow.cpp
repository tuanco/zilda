/*
 *  MainWindow.cpp
 *  Zool
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 imp GmbH. All rights reserved.
 *
 */

// Qt 
#include <QtGui/QtGui>

// Project
#include "MainWindow.h"

//=======================================================================================

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	setupUi(this);
	
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	
	QGraphicsScene *scene = new QGraphicsScene();
	graphicsView->setScene(scene);
	scene->addText(tr("No ILDA sequence loaded"));
}

//=======================================================================================

MainWindow::~MainWindow()
{
}

//=======================================================================================

void MainWindow::fileOpen()
{
	QFileDialog ofd(this, "Open", "");
	ofd.setFileMode(QFileDialog::AnyFile);
	ofd.setFilter(tr("ILDA files (*.ild);;All files (*.*)"));
	ofd.setViewMode(QFileDialog::Detail);
	
	if (ofd.exec())
	{
		QString fileName = ofd.selectedFiles().at(0);
		QFileInfo fileInfo(fileName);
		
		if (fileInfo.exists())
		{
			
		}
	}
}
