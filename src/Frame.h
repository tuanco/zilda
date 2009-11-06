#pragma once

// Qt
#include <QList>

#include "Point.h"

class Frame
{
public:
			Frame();
	virtual ~Frame();

	void	addPoint(const Point& point);
	int		pointCount() const { return _points.count(); }
	Point	point(int index) { return _points[index]; }

private:

	QList<Point> _points;
};
