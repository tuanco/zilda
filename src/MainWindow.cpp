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

static QString CompanyName = "Andre Normann";
static QString ProductName = "zILDA";

//=======================================================================================

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, _isMaximized(false)
{
	setupUi(this);
	
	frameSlider->setPageStep(1);
	frameSlider->setSingleStep(1);
	
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(actionPangolin_palette, SIGNAL(triggered()), SLOT(usePangolinPalette()));
	connect(actionILDA_palette, SIGNAL(triggered()), SLOT(useILDAPalette()));
	connect(actionAbout, SIGNAL(triggered()), SLOT(about()));
	
	QGraphicsScene *scene = new QGraphicsScene();
	_noFileLoadedItem = scene->addText(tr("No ILDA sequence loaded"));		
	graphicsView->setScene(scene);

	_ildaPalette = loadPalette(":/data/ilda.palette");
	_pangolinPalette = loadPalette(":/data/pangolin.palette");
	_currentPalette = &_ildaPalette;

	readSettings();
}

//=======================================================================================

MainWindow::~MainWindow()
{
}

//=======================================================================================

void MainWindow::fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, 
													tr("Open ILDA file"), 
													_lastDirectory, 
													tr("ILDA files (*.ild);;All files (*.*)"));
	QFileInfo fileInfo(fileName);
	
	if (fileInfo.exists())
	{
		ReaderWriterILDA reader;

		_sequence = QSharedPointer<Sequence>(reader.readFile(fileName));
		_sequence->setPalette(*_currentPalette);
		
		_lastDirectory = fileInfo.absoluteFilePath();

		// Fill file statistics
		fileNameLabel->setText(fileInfo.fileName());
		fileSizeLabel->setText(getFileSize(fileInfo.size()));
		ildaFormatLabel->setText(reader.version());
		numberOfFramesLabel->setText(QString::number(_sequence->frameCount()));

		// Set the current drawing mode (FIXME: Do not query the GUI for such infos)
		if (normalRadioButton->isChecked())
			_sequence->setDrawMode(Sequence::DrawModeNormal);
		else if (diagnosticRadioButton->isChecked())
			_sequence->setDrawMode(Sequence::DrawModeDiagnostic);

		// Setup frame slider
		frameSlider->setRange(0, _sequence->frameCount()-1);
		_timeBar->setRange(0, _sequence->frameCount() / 1000.0 * 30);
		
//		_timeLine = new QTimeLine(40 * _sequence->frameCount(), this);
		_timeLine = _timeBar->timeLine();
		_timeLine->setDuration(30 * _sequence->frameCount());
		_timeLine->setFrameRange(0, _sequence->frameCount());
		_timeLine->setCurveShape(QTimeLine::LinearCurve);
		_timeLine->setLoopCount(0);
		connect(_timeLine, SIGNAL(frameChanged(int)), _sequence.data(), SLOT(setActiveFrame(int)));

		// Build the connections
		connect(_sequence.data(), SIGNAL(frameChanged(Frame*)), this, SLOT(frameChanged(Frame*)));
		connect(frameSlider, SIGNAL(valueChanged(int)), SLOT(frameSliderChanged(int)));
		connect(normalRadioButton, SIGNAL(clicked()), this, SLOT(drawModeChanged()));
		connect(diagnosticRadioButton, SIGNAL(clicked()), this, SLOT(drawModeChanged()));

		QGraphicsScene *scene = new QGraphicsScene();
		scene->addItem(_sequence.data());
		graphicsView->setScene(scene);
		
		_timeBar->update();

		// FIXME: Need to call this until a resize event happens.
		resizeEvent(NULL);
	}
}

//=======================================================================================

void MainWindow::resizeEvent(QResizeEvent*)
{
	QRect rect = graphicsView->viewport()->rect();
	qreal scalingFactor = (rect.height() < rect.width()) ? rect.height() : rect.width();
	scalingFactor /= 65535.0;
	
	// Create a matrix to transform points from ILDA coordinate system 
	// into screen coordinat system
	QMatrix matrix;
	matrix.translate(rect.center().x(), rect.center().y());
	matrix.scale(scalingFactor, -scalingFactor);
	
	graphicsView->setMatrix(matrix);	
}

//=======================================================================================

void MainWindow::closeEvent(QCloseEvent *event)
{
	writeSettings();
	event->accept();
}

//=======================================================================================

void MainWindow::frameSliderChanged(int pos)
{
	if (_timeLine->state() != QTimeLine::Running)
		_timeLine->setCurrentTime(pos * 40);
}

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

//=======================================================================================

void MainWindow::usePangolinPalette()
{
	_currentPalette = &_pangolinPalette;
	actionILDA_palette->setChecked(false);
	actionPangolin_palette->setChecked(true);

	if (!_sequence.isNull())
	{
		_sequence->setPalette(_pangolinPalette);
		_sequence->update();
	}
}

//=======================================================================================

void MainWindow::useILDAPalette()
{
	_currentPalette = &_ildaPalette;
	actionILDA_palette->setChecked(true);
	actionPangolin_palette->setChecked(false);

	if (!_sequence.isNull())
	{
		_sequence->setPalette(_ildaPalette);
		_sequence->update();
	}
}

//=======================================================================================

QVector<QColor> MainWindow::loadPalette(const QString& fileName) const
{
	QVector<QColor> pal;
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream stream(&file);
		stream.setByteOrder(QDataStream::BigEndian);

		quint32 magic;
		stream >> magic;
		if (magic == 0x7A50414C)
		{
			quint32 version;
			stream >> version;

			if (version <= 1)
			{
				stream.setVersion(QDataStream::Qt_4_0);
				stream >> pal;
			}
		}

		file.close();
	}

	return pal;
}

//=======================================================================================

bool MainWindow::savePalette(const QString& fileName, const QVector<QColor>& pal) const
{
	QFile file(fileName);

	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream stream(&file);
		stream.setByteOrder(QDataStream::BigEndian);
		stream << (quint32)0x7A50414C;
		stream << (quint32)1;
		stream.setVersion(QDataStream::Qt_4_0);
		stream << pal;
		file.close();
		return true;
	}

	return false;
}

//=======================================================================================

void MainWindow::about()
{
	
	QString text;
	text = tr(
			  "<h3>About zILDA</h3>"
              "<p>Version 0.1</p>"
			  "<p>OpenSource ILDA file viewer</p>"
			  "<p>Copyright (c) 2009 by Andre Normann</p>"
			  "<a href=\"http://code.google.com/p/zilda/\"><span style=\"text-decoration: underline; color:#ffaa00;\">http://code.google.com/p/zilda/</span></a>"
			  );
	QMessageBox::about(this, "About zILDA", text);
}

//=======================================================================================

void MainWindow::writeSettings() const
{
	QSettings settings(CompanyName, ProductName);

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
    settings.setValue("pos", pos());	
	settings.setValue("maximized", _isMaximized);
	settings.setValue("lastDirectory", _lastDirectory);
	settings.endGroup();

	QString palette;
	if (_currentPalette == &_ildaPalette)
		palette = "ilda";
	else if (_currentPalette == &_pangolinPalette)
		palette = "pangolin";
	settings.setValue("palette", palette);
}

//=======================================================================================

void MainWindow::readSettings()
{
	QSettings settings(CompanyName, ProductName);

	 settings.beginGroup("MainWindow");
	 resize(settings.value("size", QSize(400, 400)).toSize());
	 move(settings.value("pos", QPoint(200, 200)).toPoint());
	 if (settings.value("maximized", false).toBool())
		 showMaximized();
	_lastDirectory = settings.value("lastDirectory", "").toString();
	 settings.endGroup();

	 QString palette = settings.value("palette", "ilda").toString();
	 if (palette == "ilda")
	 {
		 useILDAPalette();
	 }
	 else if (palette == "pangolin")
	 {
		 usePangolinPalette();
	 }
}

//=======================================================================================
