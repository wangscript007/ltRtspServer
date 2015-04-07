#pragma once
#include "MediaBuffer.h"
#include <iostream>
#include <queue>

using namespace std;

class MediaStream
{
public:
	MediaStream();

	virtual void	DevNode(const Buffer& buf, unsigned pos);
	virtual buf_share_ptr GetNode();
	const unsigned GetListLen()const;

	virtual void	AddNode();

	virtual ~MediaStream();
protected:
	queue<Buffer *> bufferlist;
	Buffer* unablebuf;
};


