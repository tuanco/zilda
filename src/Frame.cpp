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

Frame::Frame()
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
}

//=======================================================================================

void Frame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	int i = 0;
	int pointCount = _points.count();
	
	while (i < pointCount)
	{
		const Point& p = _points[i];
		
		if (!p.isBlanked())
		{
			QPainterPath path;
			painter->setPen(QPen(p.color()));
			path.moveTo(p);
			
			while (++i < pointCount && ! _points[i].isBlanked())
			{
				//painter->setPen(QPen(_points[i].color()));
				path.lineTo( _points[i]);
			}
			
			if (i < pointCount)
			{
				//painter->setPen(QPen(_points[i].color()));
				path.lineTo(_points[i]);
			}

			painter->drawPath(path);
		}
		else
			i++;
	}
	
}

//=======================================================================================
