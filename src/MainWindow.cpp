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
#include <QtOpenGL>

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

	menuView->addAction(fileBrowserDock->toggleViewAction());
	menuView->addAction(informationDock->toggleViewAction());
	menuView->addAction(drawingDock->toggleViewAction());

	// Load and initialize palettes
	_ildaPalette = loadPalette(":/data/ilda.palette");
	_pangolinPalette = loadPalette(":/data/pangolin.palette");
	_currentPalette = &_ildaPalette;

	// File browser
    QFileSystemModel *fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(QDir::rootPath());
    fsModel->setNameFilterDisables(false);
    fsModel->setNameFilters(QStringList() << "*.ild");

	treeView->setModel(fsModel);
	treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	//treeView->setSortingEnabled(true);
	//treeView->sortByColumn(0, Qt::AscendingOrder);
	treeView->setColumnHidden(2, true);
	treeView->setColumnHidden(3, true);
	//treeView->setRootIndex(fsModel->index(QDir::homePath()));

	// Create the connections
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(actionPangolin_palette, SIGNAL(triggered()), SLOT(usePangolinPalette()));
	connect(actionILDA_palette, SIGNAL(triggered()), SLOT(useILDAPalette()));
	connect(actionAbout, SIGNAL(triggered()), SLOT(about()));	
    connect(treeView, SIGNAL(clicked(QModelIndex)), SLOT(fileBrowserItemClicked(QModelIndex)));
    connect(treeView, SIGNAL(doubleClicked(QModelIndex)), SLOT(fileBrowserItemDblClicked(QModelIndex)));
    connect(normalRadioButton, SIGNAL(clicked()), SLOT(drawModeChanged()));
    connect(diagnosticRadioButton, SIGNAL(clicked()), SLOT(drawModeChanged()));
	
	// restore settings
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
        openFile(fileName);
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

	QAbstractItemModel *model = treeView->model();
	treeView->setModel(0);
	delete model;
	model = 0;

	event->accept();
}

//=======================================================================================

void MainWindow::fileBrowserItemClicked(const QModelIndex& index)
{
    QFileSystemModel *fsModel = qobject_cast<QFileSystemModel*>(treeView->model());
    _lastFileBrowserItem = fsModel->fileInfo(index).absoluteFilePath();
}

//=======================================================================================

void MainWindow::fileBrowserItemDblClicked(const QModelIndex& index)
{
    QFileSystemModel *fsModel = qobject_cast<QFileSystemModel*>(treeView->model());
    _lastFileBrowserItem = fsModel->fileInfo(index).absoluteFilePath();

	QFileInfo fileInfo(_lastFileBrowserItem);

	if (fileInfo.exists() && fileInfo.isFile())
		openFile(_lastFileBrowserItem);
}

//=======================================================================================

void MainWindow::frameChanged(Frame *newFrame)
{
	int newFrameNr = newFrame->frameNr();

	frameLabel->setText(QString::number(newFrameNr+1));
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

void MainWindow::openFile(const QString& fileName)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	_timeBar->stopClicked();
	
    QFileInfo fileInfo(fileName);
    ReaderWriterILDA reader;

    _lastDirectory = fileInfo.absoluteFilePath();

    _sequence = reader.readFile(fileName);
    _sequence->setPalette(*_currentPalette);

    // Fill file statistics
    fileNameLabel->setText(fileInfo.fileName());
    fileSizeLabel->setText(getFileSize(fileInfo.size()));
    ildaFormatLabel->setText(reader.version());
    numberOfFramesLabel->setText(QString::number(_sequence->frameCount()));

    // Set the current drawing mode (FIXME: Do not query the GUI for such infos)
	drawModeChanged();

    // Setup timeline
    _timeBar->setRange(0, _sequence->frameCount() / 1000.0 * 30);
    _timeLine = _timeBar->timeLine();
    _timeLine->setDuration(30 * _sequence->frameCount());
    _timeLine->setFrameRange(0, _sequence->frameCount());
    _timeLine->setCurveShape(QTimeLine::LinearCurve);
    _timeLine->setLoopCount(0);

    // Build the connections
    connect(_timeLine, SIGNAL(frameChanged(int)), _sequence.data(), SLOT(setActiveFrame(int)));
    connect(_sequence.data(), SIGNAL(frameChanged(Frame*)), SLOT(frameChanged(Frame*)));

	// Create scene and attach to graphics view
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addItem(_sequence.data());
    graphicsView->setScene(scene);

    frameChanged(_sequence->frame(0));
    _timeBar->update();

    // FIXME: Need to call this until a resize event happens.
    resizeEvent(0);

	QApplication::restoreOverrideCursor();
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
              "<p>Version 0.2 ALPHA</p>"
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
	settings.setValue("lastDirectory", _lastDirectory);
    settings.setValue("lastFileBrowserItem", _lastFileBrowserItem);
	settings.setValue("state", saveState());
	settings.setValue("geometry", saveGeometry());
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

	_lastDirectory = settings.value("lastDirectory", "").toString();
    _lastFileBrowserItem = settings.value("lastFileBrowserItem", "").toString();

	restoreState(settings.value("state").toByteArray());
	restoreGeometry(settings.value("geometry").toByteArray());

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

    if (!_lastFileBrowserItem.isEmpty())
    {
        QFileSystemModel *model = qobject_cast<QFileSystemModel*>(treeView->model());
        QModelIndex index = model->index(_lastFileBrowserItem);
        treeView->setCurrentIndex(index);
        treeView->scrollTo(index);
    }
}

//=======================================================================================

void MainWindow::fileSaveAs()
{
	saveFile("");
}

void MainWindow::saveFile(const QString& fileName)
{
	int firstFrame = _timeLine->frameForTime(_timeBar->inMarker() * 1000);
	int lastFrame = _timeLine->frameForTime(_timeBar->outMarker() * 1000);
	
	qDebug() << "FirstFrame: " << firstFrame;
	qDebug() << "LastFrame : " << lastFrame;
}
