#pragma once
#include <iostream>
#include <map>
using namespace std;

class MediaCreateSdp
{
public:
	static MediaCreateSdp* GetInstance(const string& path);
	const string& GetSdp(const string& filename);
protected:
	MediaCreateSdp(const string& path);
	~MediaCreateSdp();
private:
	map<string*, string*> *SdpList;
	static MediaCreateSdp* sdp;
};

