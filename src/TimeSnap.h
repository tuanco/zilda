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
	virtual ~TimeSnap();

	void	setRange(qreal startTime, qreal endTime);
	qreal	startTime() const { return _startTime; }
	qreal	endTime() const { return _endTime; }
	
	qreal	startSecs() { return _startSecs; }
	void	setStartSecs(qreal startSecs) { _startSecs = startSecs; }
	qreal	timeVisualized() { return _timeVisualized; }


protected:

	void	paintEvent(QPaintEvent *);
	void	mousePressEvent(QMouseEvent *ev);
	void	mouseReleaseEvent(QMouseEvent *ev);
	void	mouseMoveEvent(QMouseEvent *ev);
		
	
Q_SIGNALS:
	
	void timeChanged();
	
	
private:
	
	qreal _startTime,
		  _endTime;
	qreal _startSecs;
	qreal _timeVisualized;
	bool _drag;
	TimeBar *_timeBar;
};

//=======================================================================================
