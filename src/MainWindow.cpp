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
, _timer(0L)
{
	setupUi(this);
	
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	
	QGraphicsScene *scene = new QGraphicsScene();
	_noFileLoadedItem = scene->addText(tr("No ILDA sequence loaded"));		
	graphicsView->setScene(scene);
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
			setSequence(_sequence, 0);
			
			_frame = 0;
			_maxFrames = _sequence->frameCount();
			
			if (_timer)
			{
				_timer->stop();
				delete _timer;
				_timer = 0L;
			}
			
			_timer = new QTimer(this);
			connect(_timer, SIGNAL(timeout()), this, SLOT(timerTriggered()));
			_timer->start(42);
		}
	}
}

//=======================================================================================

void MainWindow::setSequence(Sequence *seq, int index)
{
	if (!seq || seq->frameCount() == 0)
		return;

	/*
	QGraphicsScene *scene = graphicsView->scene();
	scene->removeItem(_noFileLoadedItem);
	
	if (index == 0)
		scene->removeItem(seq->frame(seq->frameCount()-1));
	else
		scene->removeItem(seq->frame(index--));

	Frame *frame = seq->frame(index);
	scene->addItem(frame);
	*/

	Frame *frame = seq->frame(index);
	QGraphicsScene *scene = new QGraphicsScene();
//	scene->setSceneRect(65535.0f / 2.0f * -1.0f, 65535.0f / 2.0f * -1.0f, 65535.0f, 65535.0f);
	scene->addItem(frame);

	QRect rect = graphicsView->viewport()->rect();
	float scalingFactor = (rect.height() < rect.width()) ? rect.height() : rect.width();
	scalingFactor /= 65535.0f;
	
	QMatrix matrix;
	matrix.translate(rect.center().x(), rect.center().y());
	matrix.scale(scalingFactor, -scalingFactor);
	
	graphicsView->setMatrix(matrix);	
	graphicsView->setScene(scene);
}

//=======================================================================================

void MainWindow::resizeEvent(QResizeEvent*)
{
}

//=======================================================================================

void MainWindow::timerTriggered()
{
	if (_frame < _maxFrames-1)
		_frame++;
	else
		_frame = 0;
	
	setSequence(_sequence, _frame);
}
