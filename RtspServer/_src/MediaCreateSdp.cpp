#include "stdafx.h"
#include "MediaCreateSdp.h"
#include "h264MediaStream.h"
#include <windows.h>

MediaCreateSdp* MediaCreateSdp::sdp = NULL;

MediaCreateSdp* MediaCreateSdp::GetInstance(const string& path)
{
	if (sdp == NULL)
	{
		sdp = new MediaCreateSdp(path);
		return sdp;
	}
	return sdp;
}


MediaCreateSdp::MediaCreateSdp(const string& path)
{
	char filename[_MAX_FNAME] = {0};
	_splitpath_s(path.c_str(), NULL, 0, NULL, 0, filename, _MAX_FNAME, NULL, 0);
	string *PFname = new string(filename);
	HANDLE m_hVideoFile = CreateFile(path.c_str(), 
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);	
	if (m_hVideoFile == INVALID_HANDLE_VALUE) 
	{
		perror(filename);
	}
	
	char valuetmp[2048] = {0};
	DWORD SizeGet = 0;
	bool fresult = true;
	fresult = ReadFile(m_hVideoFile, valuetmp, 2048, &SizeGet, NULL);
	if (!fresult || !SizeGet)
	{
		perror(filename);
	}
	h264MediaStream* st = new h264MediaStream();
	Buffer buf(2048);
	buf.FullBuffer(valuetmp, 2048);
	st->FindNal(buf, 0);
	_CrtDbgBreak();


}