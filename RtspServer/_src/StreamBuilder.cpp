#include "stdhead.h"
#include "StreamBuilder.h"

media_stream_ptr 
MediaStreamBuild::CreateNew(const string& MediaExt, MediaType& Type)
{
	MediaStream* Builder = NULL;
	Type = UNDEFINED;
	if (!MediaExt.compare(".h264") || !MediaExt.compare(".H264"))
	{
		Builder =  new h264MediaStream();
		Type = H264;
	}

	if (Builder)
	{
		media_stream_ptr p(Builder);
		return p;
	}
    media_stream_ptr p;
    p.reset();
    return p;
}
