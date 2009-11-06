#include "Frame.h"

Frame::Frame()
{
}

Frame::~Frame()
{
}

void Frame::addPoint(float x, float y, float z)
{
	_points.append(x);
	_points.append(y);
	_points.append(z);
}
