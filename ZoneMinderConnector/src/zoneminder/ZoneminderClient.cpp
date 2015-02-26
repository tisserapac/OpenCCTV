/*
 * ZoneminderClient.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: anjana
 */

#include "ZoneminderClient.hpp"

namespace zoneminder {

ZoneminderClient::ZoneminderClient() : VmsClient()
{
	_pDecoderPtr = NULL;	//Initialized in the init() function
}

bool ZoneminderClient::init(VmsConnectInfo info)
{
	string sAuth, sConnKey, sRand;

	if(!getZMLiveStreamDetails(info.sServerIp, info.iServerPort, info.sUsername, info.sPassword, info.sCameraId, sAuth, sConnKey, sRand))
	{
		return false;
	}

	stringstream ssUrl;
	ssUrl << "/cgi-bin/nph-zms?mode=jpeg&monitor=" << info.sCameraId;
	ssUrl << "&scale=100&maxfps=5&buffer=1000&auth=" << sAuth;
	ssUrl << "&connkey=" << sConnKey;
	ssUrl << "&rand=" << sRand;
	string _sUrl = ssUrl.str();

	//cout << "_sUrl : " << _sUrl << endl;

	_pDecoderPtr = new TcpMpegDecoder();

	if (!_pDecoderPtr->init(info.sServerIp,info.iServerPort,_sUrl))
	{
		delete _pDecoderPtr; _pDecoderPtr = NULL;
		return false;
	}

	return true;
}

int ZoneminderClient::produceImageObject(ThreadSafeQueue<Image>* pQueue)
{
	if(_pDecoderPtr)
	{
		(*_pDecoderPtr).startDecoding(pQueue);
		return 1;

	}else{
		return -1;
	}

}

bool ZoneminderClient::isStillProducingImages()
{

	if(_pDecoderPtr)
	{
		return (*_pDecoderPtr).isStillProcessing();
	}else
	{
		return false;
	}
}

bool ZoneminderClient::getZMLiveStreamDetails(const string& sServerName, const int iServerPort, const string& sUsername, const string& sPassword, const string& sCameraId, string& sAuth, string& sConnkey, string& sRand)
{
	FILE *in;
	char buff[512];

	string sScriptPath = "/home/anjana/eclipse-git-workspace/OpenCCTV/ZoneMinderConnector/Debug/zm_conn.rb";

	size_t pos = sServerName.find("http://");
	string sServerPath = sServerName;
	if (pos == string::npos)
	{
		sServerPath = "http://" + sServerPath;
	}

	stringstream ssCommand;
	ssCommand << "ruby "<< sScriptPath << " ";
	ssCommand << sServerPath << ":" << iServerPort << " ";
	ssCommand << sUsername << " ";
	ssCommand << sPassword << " ";
	ssCommand << sCameraId;

	string sCommand = ssCommand.str();

	//pCommand = "ruby /home/anjana/eclipse-git-workspace/OpenCCTV/ZoneMinderConnector/Debug/zm_conn.rb http://zoneminder.cs.ait.ac.th lakindu OpenCCTV@1 7";

	if(!(in = popen(sCommand.c_str(), "r"))){
		return false;
	}

	string  sResult = "";

	while(fgets(buff, sizeof(buff), in) != NULL){
		sResult += buff;
	}
	pclose(in);

	sResult = sResult.substr(0, sResult.length()-1);

	//Sample Result :
	//http://servername/cgi-bin/nph-zms?mode=jpeg&monitor=7&scale=100&maxfps=5&buffer=1000&auth=1b9e06f5b96209322d89bc0c50c7de99&connkey=770280&rand=1424845345
	//cout << "sResult Length : " << sResult.length() << endl;
	//cout << "sResult : " << sResult << endl;

	size_t pos1 = sResult.find("auth=");
	size_t pos2 = sResult.find("&connkey=");
	size_t pos3 = sResult.find("&rand=");

	if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
	{
		pos1 += 5;
		sAuth = sResult.substr(pos1,pos2-pos1);

		pos2 += 9;
		sConnkey = sResult.substr (pos2,pos3-pos2);

		pos3 += 6;
		sRand = sResult.substr (pos3);
	}
	else
	{
		return false;
	}
	return true;
}

ZoneminderClient::~ZoneminderClient()
{
	delete _pDecoderPtr; _pDecoderPtr = NULL;
}

} /* namespace zoneminder */
