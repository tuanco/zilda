/*
 *  Frame.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "Frame.h"

// Qt
#include <QtGui>

//=======================================================================================

Frame::Frame(bool is3D)
: _nr(-1)
, _numberOfVisiblePoints(0)
, _is3D(is3D)
{
}

//=======================================================================================

Frame::~Frame()
{
	_points.clear();
}

//=======================================================================================

void Frame::addPoint(const Point& point)
{
	_points.append(point);
	if (!point.isBlanked())
		_numberOfVisiblePoints++;
}

//=======================================================================================

void Frame::paintNormal(QPainter *painter) const
{
	QList<Point>::const_iterator it = _points.begin();
	QList<Point>::const_iterator endIt = _points.end();

	while (it != endIt)
	{
		if (!it->isBlanked())
		{
			QList<Point>::const_iterator p = it;
			QPainterPath path;
			path.moveTo(*it);
			
			while (++it < endIt && !it->isBlanked() && p->color() == it->color())
			{
				path.lineTo(*it);
			}
			
			if (it < endIt)
			{
				path.lineTo(*it);
			}

			painter->setPen(QPen(p->color()));
			painter->drawPath(path);
		}
		else
			it++;
	}
}

//=======================================================================================

void Frame::paintDiagnostic(QPainter *painter) const
{
	paintNormal(painter);

	const int CTRL_POINT_SIZE = 300;
	foreach (const Point& p, _points)
	{
		if (!p.isBlanked())
			painter->fillRect(p.x()-(CTRL_POINT_SIZE/2), 
							  p.y()-(CTRL_POINT_SIZE/2), 
							  CTRL_POINT_SIZE, 
							  CTRL_POINT_SIZE, 
							  p.color());
	}
}