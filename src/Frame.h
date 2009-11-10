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

// Project
#include "Point.h"

class Sequence;

class Frame
{
	Q_PROPERTY(int frameNr READ frameNr WRITE setFrameNr)

public:
	
	enum FrameFormat 
	{
		FF_3D,
		FF_2D
	};
					Frame(Sequence *parent, bool is3D);
	virtual			~Frame();

	void			addPoint(const Point& point);
	int				pointCount() const { return _points.count(); }
	const Point&	point(int index) const { return _points[index]; }

	int				visiblePointCount() const { return _numberOfVisiblePoints; }
	int				hiddenPointCount() const { return _points.count() - _numberOfVisiblePoints; }
	FrameFormat		format() const { return _format; }

	void			setFrameNr(int nr) { _nr = nr; }
	int				frameNr() const { return _nr; }
	
	void			paintNormal(QPainter *painter) const;
	void			paintDiagnostic(QPainter *painter) const;


private:

	QColor			penColor(const Point& point) const;

	QList<Point> _points;
	int			 _nr;
	int			 _numberOfVisiblePoints;
	FrameFormat	 _format;
	Sequence	 *_parent;
};
