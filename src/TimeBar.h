/*
 *  TimeBar.h
 *  zILDA
 *
 *  Created by Andre Normann on 11.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

//=======================================================================================

// Qt
#include <QWidget>
#include <QEvent>
#include <QDateTime>

// Project
#include "TimeSnap.h"

//=======================================================================================

class QGridLayout;
class QPushButton;
class QTimeSnap;
class QLabel;
class QSlider;
class QToolButton;

//=======================================================================================

class TimeBar: public QWidget
{
	Q_OBJECT
	
public:
	
			TimeBar(QWidget *parent=0);
	virtual ~TimeBar();

	void	setRange(qreal startTime, qreal endTime);
	
	void	snapTime();
	qreal	demoTime() { return _demoTime; }
	void	setDemoTime(qreal time) { _demoTime = time; }
	qreal	startTime() { return _timeSnap->startSecs(); }
	
	bool	snapFollow() { return _snapFollow; }
	void	setSnapFollow(bool snapFollow) { _snapFollow = snapFollow; }
	
	void	newTime(); // to be called to emit signals and repaint (after an external setDemoTime(); 
	
	
private:
	
	qreal _demoTime; // in seconds
	bool _isPlaying;
	QTime _prevTime;
	
	TimeSnap	*_timeSnap;
	QPushButton *_timePrev, 
				*_timeNext;
	QToolButton *_playPause, 
				*_stop,
				*_first,
				*_last;
	QGridLayout *_layout;
	QTimer      *_timerNext,
				*_timerPrev, 
				*_timerPlay;
	bool         _snapFollow;
	
	
signals:
	
	void timeChanged();
	
	
public slots:
	
	void snapChanged();
	void nextClicked();
	void prevClicked();
	void playClicked();
	void stopClicked();
	void playTimeout();
};

//=======================================================================================

