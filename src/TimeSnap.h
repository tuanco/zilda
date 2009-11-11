/*
 *  TimeSnap.h
 *  zILDA
 *
 *  Created by Andre Normann on 11.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

//=======================================================================================

#include <QWidget>

//=======================================================================================

class TimeBar;

//=======================================================================================

class TimeSnap : public QWidget
{
	Q_OBJECT
	
public:
	
			TimeSnap(TimeBar *parent=0);
	
	void	paintEvent(QPaintEvent *);
	void	mousePressEvent(QMouseEvent *ev);
	void	mouseReleaseEvent(QMouseEvent *ev);
	void	mouseMoveEvent(QMouseEvent *ev);
	
	qreal	startSecs() { return _startSecs; }
	void	setStartSecs(qreal startSecs) { _startSecs = startSecs; }
	qreal	timeVisualized() { return _timeVisualized; }
	
	
signals:
	
	void timeChanged();
	
	
private:
	
	qreal _startSecs;
	qreal _timeVisualized;
	bool _drag;
	TimeBar *_timeBar;
};

//=======================================================================================
