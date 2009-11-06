#include "Sequence.h"
#include "Frame.h"

Sequence::Sequence()
{
}

Sequence::~Sequence()
{
}

void Sequence::addFrame(Frame *frame)
{
	_frames.append(frame);
}

Frame* Sequence::frame(int index)
{
	return _frames[index];
}
