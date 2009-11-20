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
#include <QToolButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QTimeLine>
#include <QtGui>

//=======================================================================================

#define TIMEBAR_HEIGHT 55
#define TIMEBAR_BUTWIDTH 15
#define TIMEBAR_BUTWIDTH3 30
#define TIMEBAR_BUTWIDTH2 100

//=======================================================================================

TimeBar::TimeBar(QWidget *parent) 
: QWidget(parent)
, _isPlaying(true)
, _timeLine(new QTimeLine())
{
	_snapFollow = true; 
	
	_timeLine->setDuration(1);
	
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
	
	_timeRuler = new TimeRuler(this);
	
	_playPause = new QToolButton(this);
    _playPause->setIcon(QIcon(QString::fromUtf8(":/data/icons/Play.png")));

	_stop = new QToolButton(this);
    _stop->setIcon(QIcon(QString::fromUtf8(":/data/icons/Stop.png")));

	_first = new QToolButton(this);
    _first->setIcon(QIcon(QString::fromUtf8(":/data/icons/First.png")));
	
	_last = new QToolButton(this);
    _last->setIcon(QIcon(QString::fromUtf8(":/data/icons/Last.png")));
	
//	QScrollBar *bar = new QScrollBar(Qt::Horizontal, this);
	
	
	_layout->addWidget(_timePrev,   0, 0, 2, 1);
	_layout->addWidget(_timeRuler,  0, 1, 2, 1);	
//	_layout->addWidget(bar,			1, 1);
	_layout->addWidget(_timeNext,   0, 2, 2, 1);	
	_layout->addWidget(_playPause,	0, 4);
	_layout->addWidget(_stop,		0, 5);
	_layout->addWidget(_first,		1, 4);
	_layout->addWidget(_last,		1, 5);
	
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
	
	connect(_timeRuler, SIGNAL(timeChanged()), this, SLOT(snapChanged()));
	connect(_playPause, SIGNAL(clicked()), SLOT(playClicked()));
	connect(_stop, SIGNAL(clicked()), SLOT(stopClicked()));	
	connect(_first, SIGNAL(clicked()), SLOT(firstClicked()));
	connect(_last, SIGNAL(clicked()), SLOT(lastClicked()));
	connect(_timeLine, SIGNAL(valueChanged(qreal)), SLOT(playTimeout(qreal)));
}

//=======================================================================================

TimeBar::~TimeBar()
{
}

//=======================================================================================

void TimeBar::snapChanged()
{
	emit timeChanged();
}

//=======================================================================================

void TimeBar::nextClicked()
{
//	qDebug() << "endTime = " << _timeRuler->endTime() << ", endSecs" << _timeRuler->startSecs() + _timeRuler->timeVisualized();
	if (_timeRuler->endTime() > _timeRuler->startSecs() + 1 + _timeRuler->timeVisualized())
		_timeRuler->setStartSecs(_timeRuler->startSecs() + 1);

	_timeRuler->repaint();
	emit timeChanged();
}

//=======================================================================================

void TimeBar::prevClicked()
{
	if (_timeRuler->startSecs() > 0)
		_timeRuler->setStartSecs(_timeRuler->startSecs() - 1);
	
	_timeRuler->repaint();
	emit timeChanged();
}

//=======================================================================================

void TimeBar::playClicked()
{
	QToolButton *btn = qobject_cast<QToolButton*>(sender());
	QIcon icon;

	if (_timeLine->state() == QTimeLine::Running)
	{
		icon.addFile(QString::fromUtf8(":/data/icons/Play.png"), QSize(), QIcon::Normal, QIcon::Off);
		btn->setIcon(icon);
		_timeLine->stop();
	}
	else
	{
		icon.addFile(QString::fromUtf8(":/data/icons/Pause.png"), QSize(), QIcon::Normal, QIcon::Off);
		btn->setIcon(icon);
		_timeLine->start();
	}
	
	_timeRuler->repaint();
	
	emit timeChanged();
}

//=======================================================================================

void TimeBar::stopClicked()
{
	_timeLine->stop();
	_timeLine->setCurrentTime(0);
	
	_timeRuler->repaint();

	_playPause->setIcon(QIcon(QString::fromUtf8(":/data/icons/Play.png")));

	emit timeChanged();
}


//=======================================================================================

void TimeBar::playTimeout(qreal value)
{
	if (_snapFollow && 
		value * _timeLine->duration() / 1000.0 > _timeRuler->startSecs() + _timeRuler->timeVisualized())
	{
//		_timeRuler->setStartSecs(_timeRuler->startSecs() + (int)(_timeRuler->timeVisualized()*0.75));
//		_timeRuler->setStartSecs(_timeRuler->startSecs() + (int)(_timeRuler->timeVisualized()+1));
	}
	/* else if (_snapFollow &&
			 value * _timeLine->duration() / 1000.0 < _timeRuler->startSecs() + _timeRuler->timeVisualized())
	{
		_timeRuler->setStartSecs(_timeRuler->startSecs() - (int)(_timeRuler->timeVisualized()*0.75));
	}
	 */	
	int t1 = _timeRuler->startSecs() + 1 + _timeRuler->timeVisualized();
	int t2 = _timeLine->currentTime() / 1000.0;
	if (_snapFollow && _timeRuler->startSecs() + 1 + _timeRuler->timeVisualized() < _timeLine->currentTime() / 1000.0)
		_timeRuler->setStartSecs(_timeRuler->startSecs() + 1);
	
	_timeRuler->repaint();
	emit timeChanged();
}

//=======================================================================================

void TimeBar::newTime()
{
	repaint();	
	emit timeChanged();
}

//=======================================================================================

void TimeBar::setRange(qreal startTime, qreal endTime)
{
	_timeRuler->setRange(startTime, endTime);
}

//=======================================================================================

void TimeBar::firstClicked()
{
	_timeLine->setCurrentTime(0);
	_timeRuler->setStartSecs(0.0);
	_timeRuler->repaint();
}

//=======================================================================================

void TimeBar::lastClicked()
{
	_timeLine->setCurrentTime(_timeLine->duration()-1);
	_timeRuler->setStartSecs(qMax(0.0, _timeLine->duration() / 1000 - _timeRuler->timeVisualized()));
	_timeRuler->repaint();
}

//=======================================================================================

