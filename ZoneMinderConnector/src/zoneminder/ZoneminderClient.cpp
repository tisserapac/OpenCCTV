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
	_sUsername = "";
	_sPassword = "";
	_sServerName = "";
	_sServerPort = "";
}


bool ZoneminderClient::init(string sParamString)
{
	bool bResult = true;

	//TODO : Remove hard coded server name, port and url
	_sServerName = "";
	_sServerPort = "";
	_sUrl = "";

	_pDecoderPtr = new TcpMpegDecoder();

	bResult = _pDecoderPtr->init(_sServerName,_sServerPort,_sUrl);
	if (!bResult)
	{
		delete _pDecoderPtr; _pDecoderPtr = NULL;
	}
	return bResult;
}

void ZoneminderClient::produceImageObjects(ThreadSafeQueue<Image>* pQueue)
{
	if(_pDecoderPtr)
	{
		(*_pDecoderPtr).startDecoding(pQueue);
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

ZoneminderClient::~ZoneminderClient()
{
	delete _pDecoderPtr; _pDecoderPtr = NULL;
}

} /* namespace zoneminder */
