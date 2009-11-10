/*
 *  MainWindow.h
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

// Qt
#include <QtGui>

// Project
#include "ui_MainWindow.h"

class Sequence;
class Frame;

class MainWindow : 
	public QMainWindow,
	protected Ui_MainWindow
{
	Q_OBJECT
	
public:
	
			MainWindow(QWidget *parent = 0L, Qt::WFlags flags = 0);
	virtual ~MainWindow();


protected:

	void	resizeEvent(QResizeEvent *event);
	void	closeEvent(QCloseEvent *event);

	
protected slots:

	void	fileOpen();
	void	drawModeChanged();
	void	usePangolinPalette();
	void	useILDAPalette();
	void	about();
	
	void	frameChanged(Frame *newFrame);


private:

	QString			getFileSize(qint64 size) const;
	QVector<QColor> loadPalette(const QString& fileName) const;
	bool			savePalette(const QString& fileName, 
								const QVector<QColor>& pal) const;

	void			writeSettings() const;
	void			readSettings();

	QSharedPointer<Sequence>	_sequence;
	QGraphicsTextItem			*_noFileLoadedItem;
	QVector<QColor>				_ildaPalette;
	QVector<QColor>				_pangolinPalette;
	QVector<QColor>				*_currentPalette;
};
