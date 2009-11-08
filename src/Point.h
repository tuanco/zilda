/*
 *  Point.h
 *  zILDA
 *
 *  Created by Andre Normann on 06.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

// Qt
#include <QPointF>
#include <QColor>

class Point : public QPointF
{
public:
	
			Point(const QPointF& p, double z, const QColor& color, bool blanked);
	virtual ~Point();
	
	double	z() const { return _z; }
	bool	isBlanked() const { return _blanked; }
	QColor	color() const { return _color; } 
	
private:
	
	double	_z;
	QColor	_color;
	bool	_blanked;
};	
