/*
 *  TimeBar.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 11.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "TimeBar.h"

// Qt
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>


//=======================================================================================

#define TIMEBAR_HEIGHT 35
#define TIMEBAR_BUTWIDTH 15
#define TIMEBAR_BUTWIDTH3 30
#define TIMEBAR_BUTWIDTH2 100

//=======================================================================================

TimeBar::TimeBar(QWidget *parent) 
: QWidget(parent)
, _demoTime(0.0)
, _isPlaying(true)
{
	_snapFollow = false; 
	
	setFixedHeight(TIMEBAR_HEIGHT);
	
	_layout = new QGridLayout(this);
	_layout->setMargin(0);
	_layout->setSpacing(2);
		
	_timePrev = new QPushButton(this);
	_timePrev->setText("<");
	_timePrev->setFixedHeight(TIMEBAR_HEIGHT);
	_timePrev->setFixedWidth(TIMEBAR_BUTWIDTH);
	
	_timeNext = new QPushButton(this);
	_timeNext->setText(">");
	_timeNext->setFixedHeight(TIMEBAR_HEIGHT);
	_timeNext->setFixedWidth(TIMEBAR_BUTWIDTH);
	
	_timeSnap = new TimeSnap(this);
	
	_playPause = new QPushButton(this);
	_playPause->setText("Play");
	_playPause->setFixedWidth(TIMEBAR_BUTWIDTH2);
	
	_stop = new QPushButton(this);
	_stop->setText("Stop");
	_stop->setFixedWidth(TIMEBAR_BUTWIDTH2);
	
	_layout->addWidget(_timePrev,   0, 0, 2, 1);
	_layout->addWidget(_timeSnap,   0, 1, 2, 1);	
	_layout->addWidget(_timeNext,   0, 2, 2, 1);	
	_layout->addWidget(_playPause,  0, 3, 1, 1);	
	_layout->addWidget(_stop,       1, 3, 1, 1);	
	
	_timerNext = new QTimer(this);
	_timerNext->setInterval(45);
	connect(_timeNext, SIGNAL(pressed()), _timerNext, SLOT(start()));
	connect(_timeNext, SIGNAL(released()), _timerNext, SLOT(stop()));
	connect(_timerNext, SIGNAL(timeout()), this, SLOT(nextClicked()));
	
	_timerPrev = new QTimer(this);
	_timerPrev->setInterval(45);
	connect(_timePrev, SIGNAL(pressed()), _timerPrev, SLOT(start()));
	connect(_timePrev, SIGNAL(released()), _timerPrev, SLOT(stop()));
	connect(_timerPrev, SIGNAL(timeout()), this, SLOT(prevClicked()));
	
	_timerPlay = new QTimer(this);
	_timerPlay->setInterval(15);
	connect(_playPause, SIGNAL(clicked()), this, SLOT(playClicked()));
	connect(_timerPlay, SIGNAL(timeout()), this, SLOT(playTimeout()));
	
	connect(_stop, SIGNAL(clicked()), this, SLOT(stopClicked()));
	connect(_timeSnap, SIGNAL(timeChanged()), this, SLOT(snapChanged()));
}

//=======================================================================================

void TimeBar::snapChanged()
{
	emit timeChanged();
}

//=======================================================================================

void TimeBar::nextClicked()
{
	_timeSnap->setStartSecs(_timeSnap->startSecs() + 1);
	_timeSnap->repaint();
	emit timeChanged();
}

//=======================================================================================

void TimeBar::prevClicked()
{
	if (_timeSnap->startSecs() > 0)
		_timeSnap->setStartSecs(_timeSnap->startSecs() - 1);
	
	_timeSnap->repaint();
	emit timeChanged();
}

//=======================================================================================

void TimeBar::playClicked()
{
	if(_timerPlay->isActive())
	{
		_playPause->setText(tr("Play"));
		_timerPlay->stop();
	}
	else
	{
		_playPause->setText(tr("Pause"));
		_timerPlay->start();
		_prevTime = QTime::currentTime();
	}
	
	_timeSnap->repaint();
	
	emit timeChanged();
}

//=======================================================================================

void TimeBar::stopClicked()
{
	setDemoTime(0);
	
	_timeSnap->repaint();
	_timerPlay->stop();
	_playPause->setText(tr("Play"));
	
	emit timeChanged();
}


//=======================================================================================

void TimeBar::playTimeout()
{
	QTime curTime = QTime::currentTime();
	qreal demoTime = _prevTime.msecsTo(curTime)/1000.0 + TimeBar::demoTime();
	
	if(_snapFollow && demoTime > _timeSnap->startSecs() + _timeSnap->timeVisualized())
		_timeSnap->setStartSecs(_timeSnap->startSecs() + (int)(_timeSnap->timeVisualized()*0.75));
	
	setDemoTime(demoTime);
	
	_timeSnap->repaint();
	_prevTime = curTime;
	emit timeChanged();
}

//=======================================================================================

void TimeBar::newTime()
{
	repaint();	
	emit timeChanged();
}

//=======================================================================================
