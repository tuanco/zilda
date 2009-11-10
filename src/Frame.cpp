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
#include "Sequence.h"

// Qt
#include <QtGui>

//=======================================================================================

Frame::Frame(Sequence *parent, bool is3D)
: _nr(-1)
, _numberOfVisiblePoints(0)
, _parent(parent)
{
	if (is3D)
		_format = FF_3D;
	else
		_format = FF_2D;
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
	QList<Point>::const_iterator endIt = _points.end();

	for (QList<Point>::const_iterator it = _points.begin(); it < endIt-1; ++it)
	{
		if (!(it+1)->isBlanked())
		{
			painter->setPen(QPen(penColor(*(it+1))));
			painter->drawLine(*it, *(it+1));
		}
	}
}

//=======================================================================================

void Frame::paintDiagnostic(QPainter *painter) const
{
	QList<Point>::const_iterator endIt = _points.end();

	for (QList<Point>::const_iterator it = _points.begin(); it < endIt-1; ++it)
	{
		if (!(it+1)->isBlanked())
		{
			painter->setPen(QPen(penColor(*(it+1))));
			painter->drawLine(*it, *(it+1));
		}
		else
		{
			painter->setPen(QPen(Qt::lightGray));
			painter->drawLine(*it, *(it+1));
		}
	}

	const int CTRL_POINT_SIZE = 400;
	foreach (const Point& p, _points)
	{
		QRectF rect(p.x()-(CTRL_POINT_SIZE/2), 
					p.y()-(CTRL_POINT_SIZE/2), 
					CTRL_POINT_SIZE, 
					CTRL_POINT_SIZE);

		if (!p.isBlanked())
		{
			painter->fillRect(rect, penColor(p));
		}
		else
		{
			painter->fillRect(rect, Qt::lightGray);
		}
	}
}

//=======================================================================================

QColor Frame::penColor(const Point& point) const
{
	if (point.color().isValid())
	{
		return point.color();
	}
	else if (_parent)
	{
		const QVector<QColor>& pal = _parent->palette();
		quint8 index = point.colorIndex();

		if (pal.empty())
			return Qt::white;
		else if (index < pal.count())
			return pal.at(index);
	}

	return Qt::white;
}