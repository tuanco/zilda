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
	
			Point(const QPointF& p, qreal z, const QColor& color, bool blanked, quint8 colorIndex=0);
	virtual ~Point();
	
	qreal	z() const { return _z; }
	bool	isBlanked() const { return _blanked; }
	QColor	color() const { return _color; } 
	void	setColor(const QColor& color) { _color = color; }
	quint8	colorIndex() const { return _colorIndex; }
	
private:
	
	qreal	_z;
	QColor	_color;
	bool	_blanked;
	quint8  _colorIndex;
};	
