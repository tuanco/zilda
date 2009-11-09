/*
 *  Frame.h
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

// Qt
#include <QList>
#include <QGraphicsItem>

#include "Point.h"

class Frame
{
public:
					Frame();
	virtual			~Frame();

	void			addPoint(const Point& point);
	int				pointCount() const { return _points.count(); }
	const Point&	point(int index) const { return _points[index]; }
	
	void			paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
		                  QWidget *widget);

private:

	QList<Point> _points;
};
