/*
 *  Sequence.h
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

#pragma once

// Qt
#include <QtGui>

class Frame;
class QTimer;

class Sequence 
	: public QObject
	, public QGraphicsItem
{
	Q_OBJECT

public:

			Sequence();
	virtual ~Sequence();

	QRectF	boundingRect() const;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void	addFrame(Frame *frame);
	int		frameCount() const { return _frames.count(); }
	Frame*	frame(int index);


protected slots:

	void	timerTriggered();


private:

	QVector<Frame*>				_frames;
	QVector<Frame*>::iterator	_currentFrame;
	QPointer<QTimer>			_timer;
};
