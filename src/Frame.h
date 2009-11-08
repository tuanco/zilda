#pragma once

// Qt
#include <QList>
#include <QGraphicsItem>

#include "Point.h"

class Frame : public QGraphicsItem
{
public:
			Frame();
	virtual ~Frame();

	void			addPoint(const Point& point);
	int				pointCount() const { return _points.count(); }
	const Point&	point(int index) const { return _points[index]; }
	
	QRectF			boundingRect() const;
	void			paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

	QList<Point> _points;
};
