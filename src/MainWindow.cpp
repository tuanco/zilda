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

	frameSlider->setPageStep(1);
	frameSlider->setSingleStep(1);
	
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
	QFileDialog ofd(this, tr("Open ILDA file"), "");
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

			_sequence = QSharedPointer<Sequence>(reader.readFile(fileName));

			// Fill file statistics
			fileNameLabel->setText(fileInfo.fileName());
			fileSizeLabel->setText(getFileSize(fileInfo.size()));
			ildaFormatLabel->setText(QString::number(reader.version()));
			numberOfFramesLabel->setText(QString::number(_sequence->frameCount()));

			// Set the current drawing mode (FIXME: Do not query the GUI for such infos)
			if (normalRadioButton->isChecked())
				_sequence->setDrawMode(Sequence::DrawModeNormal);
			else if (diagnosticRadioButton->isChecked())
				_sequence->setDrawMode(Sequence::DrawModeDiagnostic);

			// Setup frame slider
			frameSlider->setRange(0, _sequence->frameCount()-1);

			// Build the connections
			connect(_sequence.data(), SIGNAL(frameChanged(Frame*)), this, SLOT(frameChanged(Frame*)));
			connect(firstFrameButton, SIGNAL(clicked()), _sequence.data(), SLOT(gotoFirstFrame()));
			connect(lastFrameButton, SIGNAL(clicked()), _sequence.data(), SLOT(gotoLastFrame()));
			connect(stopButton, SIGNAL(clicked()), _sequence.data(), SLOT(stopPlayback()));
			connect(playButton, SIGNAL(clicked()), _sequence.data(), SLOT(startPlayback()));
			connect(frameSlider, SIGNAL(valueChanged(int)), _sequence.data(), SLOT(setActiveFrame(int)));
			connect(normalRadioButton, SIGNAL(clicked()), this, SLOT(drawModeChanged()));
			connect(diagnosticRadioButton, SIGNAL(clicked()), this, SLOT(drawModeChanged()));

			QGraphicsScene *scene = new QGraphicsScene();
			scene->addItem(_sequence.data());
			graphicsView->setScene(scene);

			// FIXME: Need to call this until a resize event happens.
			resizeEvent(NULL);
		}
	}
}

//=======================================================================================

void MainWindow::resizeEvent(QResizeEvent*)
{
	QRect rect = graphicsView->viewport()->rect();
	qreal scalingFactor = (rect.height() < rect.width()) ? rect.height() : rect.width();
	scalingFactor /= 65535.0;
	
	QMatrix matrix;
	matrix.translate(rect.center().x(), rect.center().y());
	matrix.scale(scalingFactor, -scalingFactor);
	
	graphicsView->setMatrix(matrix);	
}

//=======================================================================================

void MainWindow::frameChanged(Frame *newFrame)
{
	int newFrameNr = newFrame->frameNr();

	frameLabel->setText(QString::number(newFrameNr+1));
	frameSlider->setSliderPosition(newFrameNr);
	numberOfPointsLabel->setText(QString::number(newFrame->pointCount()));
	numberOfLinesLabel->setText(QString::number(newFrame->visiblePointCount()));
	numberOfHiddenLinesLabel->setText(QString::number(newFrame->hiddenPointCount()));

	QString frameFormat;
	if (newFrame->format() == Frame::FF_3D)
		frameFormat = "3D";
	else
		frameFormat = "2D";
	frameFormatLabel->setText(frameFormat);
}

//=======================================================================================

void MainWindow::drawModeChanged()
{
	if (normalRadioButton->isChecked())
		_sequence->setDrawMode(Sequence::DrawModeNormal);
	else if (diagnosticRadioButton->isChecked())
		_sequence->setDrawMode(Sequence::DrawModeDiagnostic);
}

//=======================================================================================

QString	MainWindow::getFileSize(qint64 size) const
{
	if ((size / qPow(2, 10)) < 1024)
		return QString::number((qint64)(size/qPow(2, 10))) + tr(" KB");
	else if ((size / qPow(2, 20)) < 1024)
		return QString::number((qint64)(size/qPow(2, 20))) + tr(" MB");
	else if ((size / qPow(2, 30)) < 1024)
		return QString::number((qint64)(size/qPow(2, 30))) + tr(" GB");

	return QString::number(size) + tr(" B");
}
