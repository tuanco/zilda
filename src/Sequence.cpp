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
	_timer = QPointer<QTimer>(new QTimer());
	connect(_timer.data(), SIGNAL(timeout()), this, SLOT(timerTriggered()));
	_timer->start(42);
}

//=======================================================================================

Sequence::~Sequence()
{
	_timer->stop();
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

void Sequence::timerTriggered()
{
	_currentFrame++;
	
	if (_currentFrame == _frames.end())
		_currentFrame = _frames.begin();

	emit frameChanged((*_currentFrame).data());

	update();
}

//=======================================================================================

void Sequence::gotoFirstFrame()
{
	stopPlayback();
	setActiveFrame(0);
}

//=======================================================================================

void Sequence::gotoLastFrame()
{
	stopPlayback();
	setActiveFrame(_frames.count()-1);
}

//=======================================================================================

void Sequence::stopPlayback()
{
	if (!_timer.isNull())
	{
		_timer->stop();
	}
}

//=======================================================================================

void Sequence::startPlayback()
{
	if (!_timer.isNull() && !_timer->isActive())
	{
		_timer->start(42);
	}
}

//=======================================================================================

void Sequence::setActiveFrame(int nr)
{
	if (nr >= _frames.count())
		return;

	if ((*_currentFrame)->frameNr() != nr)
	{
		_currentFrame = _frames.begin() + nr;
		update();
	emit frameChanged((*_currentFrame).data());
	}
}

//=======================================================================================

void Sequence::setDrawMode(DrawModes mode)
{
	_drawMode = mode;
	update();
}
