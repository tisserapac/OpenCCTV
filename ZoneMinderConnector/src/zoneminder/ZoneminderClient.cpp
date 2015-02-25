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
	_sIP = "";
	_sCameraId = "";
	_sFramesPerSecond = "";
}

bool ZoneminderClient::init(string sParamString)
{
	if (!setParameters(sParamString)){
		return false;
	}

	//TODO : Remove hard auth, connkey and rand values of the url
	stringstream ssUrl;
	ssUrl << "/cgi-bin/nph-zms?mode=jpeg&monitor=" << _sCameraId;
	ssUrl << "&scale=100&maxfps=5&buffer=1000&auth=";
	ssUrl << "ae73e7cd5cc6490044f42a8aa097bd90";
	ssUrl << "&connkey=123565&rand=1424830256";
	_sUrl = ssUrl.str();

	_pDecoderPtr = new TcpMpegDecoder();

	if (!_pDecoderPtr->init(_sServerName,_sServerPort,_sUrl))
	{
		delete _pDecoderPtr; _pDecoderPtr = NULL;
		return false;
	}

	return true;
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

bool ZoneminderClient::setParameters(string sParamString)
{
	/***** A sample XML string expected in sParamString ******
	<?xml version="1.0" encoding="utf-8"?>
	<ConecterParams>
		<ServerName>ServerName</ServerName>
		<ServerIP>ServerIP</ServerIP>
		<ServerPort>80</ServerPort>
		<Username>username</Username>
		<Password>password</Password>
		<CamraID>7</CamraID>
		<StreamWidth>320</StreamWidth>
		<StreamHeight>240</StreamHeight>
		<CompressionRate>0</CompressionRate>
		<AspectRatio>1</AspectRatio>
		<FramesPerSecond>5</FramesPerSecond>
	</ConecterParams>
	********************************************************/

	bool bResult = false;

	ptree pt;
	istringstream iss(sParamString);
	try
	{
		read_xml(iss, pt);
		_sServerName = pt.get<string>("ConecterParams.ServerName"); trim(_sServerName);
		_sServerPort = pt.get<string>("ConecterParams.ServerPort"); trim(_sServerPort);
		_sUsername = pt.get<string>("ConecterParams.Username"); trim(_sUsername);
		_sPassword = pt.get<string>("ConecterParams.Password"); trim(_sPassword);
		_sCameraId = pt.get<string>("ConecterParams.CamraID"); trim(_sCameraId);
		_sFramesPerSecond = pt.get<string>("ConecterParams.FramesPerSecond"); trim(_sFramesPerSecond);

		bResult = true;

	}catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << "ZoneminderClient::setParameters: "<<e.what() << std::endl;
		return false;

	}catch(boost::property_tree::ptree_bad_path &e)
	{
		std::cerr << "ZoneminderClient::setParameters: "<< e.what() << std::endl;
		return false;
	}

	return bResult;
}

ZoneminderClient::~ZoneminderClient()
{
	delete _pDecoderPtr; _pDecoderPtr = NULL;
}

} /* namespace zoneminder */
