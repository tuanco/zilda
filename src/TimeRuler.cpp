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
, _startSecs(0.0)
, _timeBar(parent)
, _startTime(0.0)
, _endTime(std::numeric_limits<qreal>::max())
{
	_drag = false;
	_timeVisualized = (qreal)((rect().width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	
}

//=======================================================================================

TimeRuler::~TimeRuler()
{
}

void TimeRuler::setRange(qreal startTime, qreal endTime)
{
	_startTime = startTime;
	_endTime = endTime;
}

//=======================================================================================

void TimeRuler::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QPalette pal = palette();
	QRect rc=rect();
	QColor lineColor(119, 119, 119);
	QColor textColor = pal.color(QPalette::WindowText);
	QFont font("Arial", 10);
	
	// Draw background
	painter.setPen(QColor(80, 80, 80));
	painter.drawRect(rc);
	painter.fillRect(rc.adjusted(1, 1, -2, -2), QColor(92, 92, 92));
	
	qreal nSecs = (int)((rc.width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	//nSecs = qMin(nSecs, _endTime);
	_timeVisualized = nSecs;
	
	int startx = (int)((rc.width() - nSecs*40) / 2);

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

			QRect rect = painter.fontMetrics().boundingRect(tmp);
			
			painter.setPen(textColor);
			painter.setFont(font);
			painter.drawText(curx - (rect.width() / 2), endy + 3, rect.width(), rect.height(), Qt::AlignHCenter | Qt::AlignTop, tmp);
		}
	}

	// Draw the current time marker
	qreal demoTime = _timeBar->demoTime();
	if (demoTime - _startSecs < nSecs+0.01 && 
	    demoTime >= _startSecs)
	{
		int barx = (int)((_timeBar->demoTime() - _startSecs) * 40 + startx);
		QPen pen(Qt::red);

		pen.setWidth(2);
		painter.setPen(pen);
		painter.drawLine(barx, 2, barx, 17);
	}
}

//=======================================================================================

void TimeRuler::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() & Qt::LeftButton)
	{
		_drag = true;
		mouseMoveEvent(ev);
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
	if (ev->button() & Qt::LeftButton)
	{
		QRect rc = rect();
		qreal nSecs = (int)((rc.width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
		//nSecs = qMin(nSecs, _endTime);
		int startx = (int)((rc.width() - nSecs*40) / 2) - 1;
		
		if (_drag && 
			ev->pos().x() > startx && 
			ev->pos().x() < rc.width()-startx)
		{
			qreal time = (ev->pos().x() - startx) / 40.0 + _startSecs;
			
			// small hack to fix some float<->int rounding in coordinate
			if(_timeBar->demoTime() > _startSecs+nSecs)
				time = _startSecs + nSecs;
			
			_timeBar->setDemoTime(time);
			emit timeChanged();
			repaint();
		}
	}
}

//=======================================================================================
