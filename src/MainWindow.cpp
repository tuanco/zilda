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
			_sequence->setPalette(*_currentPalette);

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
	settings.setValue("maximized", isMaximized());
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
