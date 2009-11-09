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
	Q_PROPERTY(int frameNr READ frameNr WRITE setFrameNr)

public:
					Frame(bool is3D);
	virtual			~Frame();

	void			addPoint(const Point& point);
	int				pointCount() const { return _points.count(); }
	const Point&	point(int index) const { return _points[index]; }

	int				visiblePointCount() const { return _numberOfVisiblePoints; }
	int				hiddenPointCount() const { return _points.count() - _numberOfVisiblePoints; }
	bool			is3D() const { return _is3D; }

	void			setFrameNr(int nr) { _nr = nr; }
	int				frameNr() const { return _nr; }
	
	void			paintNormal(QPainter *painter) const;
	void			paintDiagnostic(QPainter *painter) const;

private:

	QList<Point> _points;
	int			 _nr;
	int			 _numberOfVisiblePoints;
	bool		 _is3D;
};
