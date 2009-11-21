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

	enum DrawModes
	{
		DrawModeNormal,
		DrawModeDiagnostic
	};

			Sequence();
	virtual ~Sequence();

	QRectF	boundingRect() const;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void	addFrame(const QSharedPointer<Frame>& frame);
	int		frameCount() const { return _frames.count(); }
	Frame*	frame(int index);

	void		setDrawMode(DrawModes mode);
	DrawModes	drawMode() const { return _drawMode; }

	void					setPalette(const QVector<QColor>& pal) { _palette = pal; }
	const QVector<QColor>&	palette() const { return _palette; }


public slots:

	void	setActiveFrame(int nr);


signals:

	void	frameChanged(Frame *newFrame);


private:

	QVector<QSharedPointer<Frame> >				_frames;
	QVector<QSharedPointer<Frame> >::iterator	_currentFrame;
	DrawModes									_drawMode;
	QVector<QColor>								_palette;
};

typedef QSharedPointer<Sequence> SequenceRef;
