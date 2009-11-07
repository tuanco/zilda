/*
 *  Point.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 06.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#include "Point.h"

Point::Point(const QPointF& p, double z, const QColor& color, bool blanked)
: _point(p)
, _z(z)
, _color(color)
, _blanked(blanked)
{
}

Point::~Point()
{
}	
