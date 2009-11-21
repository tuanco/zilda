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
#include "TimeRuler.h"

//=======================================================================================

class QGridLayout;
class QPushButton;
class QTimeSnap;
class QLabel;
class QSlider;
class QToolButton;
class QTimeLine;

//=======================================================================================

class TimeBar: public QWidget
{
	Q_OBJECT
	
public:
	
			TimeBar(QWidget *parent=0);
	virtual ~TimeBar();

	void	setRange(qreal startTime, qreal endTime);
	
	void	snapTime();
	qreal	startTime() { return _timeRuler->startSecs(); }
	
	bool	snapFollow() { return _snapFollow; }
	void	setSnapFollow(bool snapFollow) { _snapFollow = snapFollow; }
	
	void	newTime(); // to be called to emit signals and repaint (after an external setDemoTime(); 
	
	QTimeLine*	timeLine() { return _timeLine; }
	
	qreal	inMarker() const { return _timeRuler->inMarker(); }
	qreal	outMarker() const { return _timeRuler->outMarker(); }
	
	
private:
	
	qreal _demoTime; // in seconds
	bool _isPlaying;
	
	TimeRuler	*_timeRuler;
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
	QTimeLine	*_timeLine;
	
	
signals:
	
	void timeChanged();
	
	
public slots:
	
	void snapChanged();
	void nextClicked();
	void prevClicked();
	void playClicked();
	void stopClicked();
	void playTimeout(qreal value);
	void firstClicked();
	void lastClicked();
};

//=======================================================================================

