#pragma once

#include <QList>

class Frame;

class Sequence
{
public:

			Sequence();
	virtual ~Sequence();

	void	addFrame(Frame *frame);
	int		frameCount() const { return _frames.count(); }
	Frame*	frame(int index);


private:

	QList<Frame*> _frames;
};
