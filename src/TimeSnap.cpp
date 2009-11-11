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

//=======================================================================================

TimeSnap::TimeSnap(TimeBar *parent) 
: QWidget(parent)
, _startSecs(0.0)
, _timeBar(parent)
{
	_drag = false;
	_timeVisualized = (qreal)((rect().width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	
}

//=======================================================================================

void TimeSnap::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QRect rc=rect();
	
	painter.setPen(QColor(80, 80, 80));
	painter.drawRect(rc);
	painter.fillRect(rc.adjusted(1, 1, -2, -2), QColor(92, 92, 92));
    painter.setPen(QColor(149, 149, 149));
	
	qreal nSecs = (int)((rc.width() + 10) / 40.9999); // each second is 40 pixel lenght (10 = 5 pixel spacing left+5pixel spacing right)
	_timeVisualized = nSecs;
	
	int startx = (int)((rc.width() - nSecs*40) / 2);
	for(int i=0; i<=nSecs*10; i++)
	{
		int curx = startx + i*4;
		int endy = 10;
		
		if ((i % 10) == 0)
			endy = 17;
		
		painter.drawLine(curx, 2, curx, endy);
		
		if (endy == 17)
		{
			QString tmp = QString::number((i / 10.0f) + _startSecs);
			
			painter.setPen(Qt::white);
			painter.drawText(curx - 5, endy + 15, tmp);
			painter.setPen(QColor(119, 119, 119));
		}
	}
	
	qreal demoTime = _timeBar->demoTime();
	if (demoTime - _startSecs < nSecs+0.01 && 
	    demoTime >= _startSecs)
	{
		int barx = (int)((_timeBar->demoTime() - _startSecs) * 40 + startx);
		painter.setPen(Qt::red);
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
