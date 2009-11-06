/*
 *  MainWindow.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Qt 
#include <QtGui>

// Project
#include "MainWindow.h"
#include "ReaderWriterILDA.h"
#include "Frame.h"

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
			ReaderWriterILDA reader;
			_sequence = reader.readFile(fileName);
			createScene();
		}
	}
}

void MainWindow::createScene()
{
	if (!_sequence || _sequence->frameCount() == 0)
		return;

	Frame *frame = _sequence->frame(0);

	QGraphicsScene *scene = new QGraphicsScene();
	graphicsView->setScene(scene);

	QPen pen(Qt::white);

	for (int i=0; i<frame->pointCount()-1; ++i)
	{
		float x1, y1;
		float x2, y2;

		x1 = frame->pointX(i);
		y1 = frame->pointY(i);
		x2 = frame->pointX(i+1);
		y2 = frame->pointY(i+1);

		scene->addLine(x1, y1, x2, y2, pen);
	}

	resize(width()-1, height());
	resize(width()+1, height());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	float scalingFactor = (graphicsView->height() < graphicsView->width()) ? graphicsView->height() : graphicsView->width();
	scalingFactor /= 65535.0f;

	QMatrix matrix;
	matrix.translate(graphicsView->width() / 2.0f, graphicsView->height() / 2.0f);
	matrix.scale(scalingFactor, -scalingFactor);

	graphicsView->setMatrix(matrix);
}
