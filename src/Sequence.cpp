/*
 *  Sequence.cpp
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

// Project
#include "Sequence.h"
#include "Frame.h"

//=======================================================================================

Sequence::Sequence()
: _currentFrame(_frames.end())
, _drawMode(DrawModeNormal)
{
}

//=======================================================================================

Sequence::~Sequence()
{
}

//=======================================================================================

void Sequence::addFrame(const QSharedPointer<Frame>& frame)
{
	_frames.append(QSharedPointer<Frame>(frame));
	_currentFrame = _frames.begin();
}

//=======================================================================================

Frame* Sequence::frame(int index)
{
	return _frames[index].data();
}

//=======================================================================================

QRectF Sequence::boundingRect() const
{
	return QRectF(-32767.5f, -32767.5f, 65535.0f, 65535.0f);
}

//=======================================================================================

void Sequence::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
					 QWidget *widget)
{
	if (_currentFrame != _frames.end())
	{
		switch (_drawMode)
		{
			case DrawModeNormal:
				(*_currentFrame)->paintNormal(painter);
				break;

			case DrawModeDiagnostic:
				(*_currentFrame)->paintDiagnostic(painter);
				break;
		}
	}
}

//=======================================================================================

void Sequence::setActiveFrame(int nr)
{
	if (nr >= _frames.count())
		return;

	_currentFrame = _frames.begin() + nr;
	update();
	emit frameChanged((*_currentFrame).data());
}

//=======================================================================================

void Sequence::setDrawMode(DrawModes mode)
{
	_drawMode = mode;
	update();
}
