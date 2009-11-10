/*
 *  Point.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 06.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#include "Point.h"

Point::Point(const QPointF& p, qreal z, const QColor& color, bool blanked, quint8 colorIndex)
: QPointF(p)
, _z(z)
, _color(color)
, _blanked(blanked)
, _colorIndex(colorIndex)
{
}

Point::~Point()
{
}	
