#pragma once

// Qt
#include <QList>

class Frame
{
public:
			Frame();
	virtual ~Frame();

	void	addPoint(float x, float y, float z);
	int		pointCount() const { return _points.count() / 3; }
	float	pointX(int index) const { return _points[index * 3]; }
	float	pointY(int index) const { return _points[index * 3 + 1]; }
	float	pointZ(int index) const { return _points[index * 3 + 2]; }


private:

	QList<float> _points;
};
