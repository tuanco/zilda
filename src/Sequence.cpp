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
{
	_timer = QPointer<QTimer>(new QTimer());
	connect(_timer.data(), SIGNAL(timeout()), this, SLOT(timerTriggered()));
	_timer->start(42);
}

//=======================================================================================

Sequence::~Sequence()
{
	_timer->stop();

	foreach(Frame *frame, _frames)
	{
		delete frame;
	}

	_frames.clear();
}

//=======================================================================================

void Sequence::addFrame(Frame *frame)
{
	_frames.append(frame);
	_currentFrame = _frames.begin();
}

//=======================================================================================

Frame* Sequence::frame(int index)
{
	return _frames[index];
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
		(*_currentFrame)->paint(painter, option, widget);
}

//=======================================================================================

void Sequence::timerTriggered()
{
	_currentFrame++;
	
	if (_currentFrame == _frames.end())
		_currentFrame = _frames.begin();

	emit frameChanged(_frames.indexOf(*_currentFrame));

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

	if (_frames.indexOf(*_currentFrame) != nr)
	{
		_currentFrame = _frames.begin() + nr;
		update();
		emit frameChanged(nr);
	}
}

//=======================================================================================
