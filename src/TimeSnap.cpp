/*
 *  TimeSnap.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 11.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "TimeSnap.h"
#include "TimeBar.h"

// Qt
#include <QPainter>
#include <QMouseEvent>

#include <limits>

//=======================================================================================

TimeSnap::TimeSnap(TimeBar *parent) 
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

TimeSnap::~TimeSnap()
{
}

void TimeSnap::setRange(qreal startTime, qreal endTime)
{
	_startTime = startTime;
	_endTime = endTime;
}

//=======================================================================================

void TimeSnap::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QRect rc=rect();
	QColor lineColor(119, 119, 119);
	QColor textColor = palette().color(QPalette::WindowText);
	
	// Draw background
	painter.setPen(QColor(80, 80, 80));
	painter.drawRect(rc);
	painter.fillRect(rc.adjusted(1, 1, -2, -2), QColor(92, 92, 92));
	
	qreal nSecs = (int)((rc.width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	nSecs = qMin(nSecs, _endTime);
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
			QString tmp = QString::number((i / 10.0f) + _startSecs);
			
			painter.setPen(textColor);
			painter.drawText(curx - 9, endy + 5, 20, 20, Qt::AlignHCenter | Qt::AlignTop, tmp);
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

void TimeSnap::mousePressEvent(QMouseEvent *ev)
{
	_drag = true;
	mouseMoveEvent(ev);
}

//=======================================================================================

void TimeSnap::mouseReleaseEvent(QMouseEvent *)
{
	_drag = false;
}

//=======================================================================================

void TimeSnap::mouseMoveEvent(QMouseEvent *ev)
{
	QRect rc = rect();
	int nSecs = (int)((rc.width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
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

//=======================================================================================
