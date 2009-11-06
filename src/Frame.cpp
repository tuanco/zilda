#include "Frame.h"

Frame::Frame()
{
}

Frame::~Frame()
{
}

void Frame::addPoint(const Point& point)
{
	_points.append(point);
}
