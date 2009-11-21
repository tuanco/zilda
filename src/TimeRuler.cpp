/*
 *  TimeSnap.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 11.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "TimeRuler.h"
#include "TimeBar.h"

// Qt
#include <QPainter>
#include <QMouseEvent>
#include <QtGui>

#include <limits>

//=======================================================================================

TimeRuler::TimeRuler(TimeBar *parent) 
: QWidget(parent)
, _startTime(0.0)
, _endTime(0.0/*std::numeric_limits<qreal>::max()*/)
, _inMarker(0.0)
, _outMarker(0.0)
, _startSecs(0.0)
, _timeBar(parent)
, _font("Arial", 10)
{
	QFontMetrics fm(_font);
	_textWidth = fm.width("00:00");
	_drag = false;
	_timeVisualized = (qreal)((rect().width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	_actionInMarker = new QAction(tr("Set IN marker"), this);
	_actionOutMarker = new QAction(tr("Set OUT marker"), this);
	connect(_actionInMarker, SIGNAL(triggered()), SLOT(setInMarker()));
	connect(_actionOutMarker, SIGNAL(triggered()), SLOT(setOutMarker()));
}

//=======================================================================================

TimeRuler::~TimeRuler()
{
}

void TimeRuler::setRange(qreal startTime, qreal endTime)
{
	_startTime = _inMarker = startTime;
	_endTime = _outMarker = endTime;
}

//=======================================================================================

void TimeRuler::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QPalette pal = palette();
	QRect rc=rect();
	QColor lineColor = Qt::gray;
	QColor textColor = pal.color(QPalette::WindowText);
	QFontMetrics fm(_font);
	
	// Draw background
	painter.setPen(QColor(80, 80, 80));
	painter.drawRect(rc);
	painter.fillRect(rc.adjusted(1, 1, -2, -2), QColor(92, 92, 92));
	
	
	qreal nSecs = (int)((rc.width() - _textWidth + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	_timeVisualized = nSecs;
		
	qreal currentTime = _timeBar->timeLine()->currentTime() / 1000.0;
	int startx = (int)((rc.width() - nSecs*40) / 2);
		
	qreal delta = ((_timeBar->timeLine()->duration() / 1000.0) - _startSecs)*40;
	
	// Draw ilda file length
	if (delta > _timeVisualized * 40)
		delta = _timeVisualized * 40;

	painter.setPen(lineColor);
	painter.drawRect(startx, 37, delta, 14);


	int x = 0, width = 0;
	
	if (_inMarker >= _startSecs && _inMarker <= _startSecs + _timeVisualized &&
		_outMarker > _inMarker && _outMarker <= _startSecs + _timeVisualized)
	{
		x = startx + (_inMarker-_startSecs)*40;
		width = (_outMarker-_startSecs)*40;
	}
	else if (_inMarker >= _startSecs && _inMarker <= _startSecs + _timeVisualized &&
			 _outMarker > _startSecs + _timeVisualized)
	{
		x = startx + (_inMarker-_startSecs)*40;
		width = delta;
	}
	else if (_inMarker < _startSecs &&
			 _outMarker > _startSecs && _outMarker <= _startSecs + _timeVisualized)
	{
		x = startx;
		width = (_outMarker-_startSecs)*40;
	}
	else if (_inMarker < _startSecs && _outMarker > _startSecs + _timeVisualized)
	{
		x = startx;
		width = delta;
	}

	if (x > 0 && width > 0)
		painter.fillRect(x, 37, width - (x - startx), 14, lineColor);
	
	
	for (int i=0; i<=nSecs*10; i++)
	{
		int curx = startx + i*4;
		int endy = 10;
		
		if ((i % 10) == 0)
			endy = 17;
		else if ((i % 5) == 0)
			endy = 13;
		
		painter.setPen(lineColor);
		painter.drawLine(curx, 2, curx, endy);
		
		if (endy == 17)
		{
			int time = (i / 10.0f) + _startSecs;
			int minutes = (time / 60);
			int seconds = (time % 60);

			QString tmp;
			tmp.sprintf("%02d:%02d", minutes, seconds);

			QRect rect = fm.boundingRect(tmp);
			
			painter.setPen(textColor);
			painter.setFont(_font);
			painter.drawText(curx - (rect.width() / 2), endy + 3, rect.width(), rect.height(), Qt::AlignHCenter | Qt::AlignTop, tmp);
		}
	}

	// Draw the current time marker
	qreal demoTime = (qreal)_timeBar->timeLine()->currentTime() / 1000.0;
	if (demoTime - _startSecs < nSecs+0.01 && 
	    demoTime >= _startSecs)
	{
		int barx = (int)((demoTime - _startSecs) * 40 + startx);
		QPen pen(Qt::red);

		pen.setWidth(2);
		painter.setPen(pen);
		painter.drawLine(barx, 2, barx, 51);
	}
}

//=======================================================================================

void TimeRuler::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() & Qt::LeftButton && _timeBar->timeLine()->state() != QTimeLine::Running)
	{
		_drag = true;
		mouseMoveEvent(ev);
	}
	else if (ev->button() & Qt::RightButton)
	{
		QRect rc = rect();
		qreal nSecs = (int)((rc.width() - _textWidth + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
		qreal delta = ((_timeBar->timeLine()->duration() / 1000.0) - _startSecs)*40;
		int startx = (int)((rc.width() - nSecs*40) / 2);

		QRect r(startx, 0, delta, 55);

		if (r.contains(ev->pos()))
		{
			QMenu menu;
			menu.addAction(_actionInMarker);
			menu.addAction(_actionOutMarker);
			menu.exec(ev->globalPos());
		}
	}
}

//=======================================================================================

void TimeRuler::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() & Qt::LeftButton)
	{
		_drag = false;
	}
}

//=======================================================================================

void TimeRuler::mouseMoveEvent(QMouseEvent *ev)
{
	if (_drag)
	{
		QRect rc = rect();
		qreal nSecs = (int)((rc.width() - _textWidth + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
		int startx = (int)((rc.width() - nSecs*40) / 2) - 1;
		
		if (ev->pos().x() > startx && 
			ev->pos().x() < rc.width()-startx)
		{
			qreal time = (ev->pos().x() - startx) / 40.0 + _startSecs;
			
			// small hack to fix some float<->int rounding in coordinate
			if(_timeBar->timeLine()->currentTime() / 1000.0 > _startSecs+nSecs)
			{
				time = _startSecs + nSecs;
			}
			
			if (time > (_timeBar->timeLine()->duration()-1) / 1000.0)
				time = (_timeBar->timeLine()->duration()-1) / 1000.0;
			
			int intTime = time * 1000;
			_timeBar->timeLine()->setCurrentTime(intTime);
			emit timeChanged();
			repaint();
		}
		else if (ev->pos().x() > rc.width()-startx)
		{
			_timeBar->timeLine()->setCurrentTime(_timeBar->timeLine()->duration()-1);
			emit timeChanged();
			repaint();
		}
		else if (ev->pos().x() < startx)
		{
			_timeBar->timeLine()->setCurrentTime(0);
			emit timeChanged();
			repaint();
		}
	}
}

//=======================================================================================

void TimeRuler::setInMarker()
{
	_inMarker = _timeBar->timeLine()->currentTime() / 1000.0;
	repaint();
}

//=======================================================================================

void TimeRuler::setOutMarker()
{
	_outMarker = _timeBar->timeLine()->currentTime() / 1000.0;
	repaint();
}

//=======================================================================================
