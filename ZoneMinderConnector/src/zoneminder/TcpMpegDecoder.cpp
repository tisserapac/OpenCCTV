/*
 * TcpMpegDecoder.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "TcpMpegDecoder.hpp"

namespace zoneminder {

TcpMpegDecoder::TcpMpegDecoder()
{
	_iCurrentContentLength = 0;
	_sCurrentImageTimeStamp = "";
	_pHttpClientPtr = NULL;
	_bDecodeEnable = false;
	_bStillProcessing = false;
	_iDecodedImageCount = 0;
}

bool TcpMpegDecoder::init(string sServerName, string sServerPort, string sUrl)
{
	bool bResult = true;

	_pHttpClientPtr = new HttpClient(sServerName, sServerPort);
	if((*_pHttpClientPtr).sendGetRequest(sServerName, sUrl))
	{
		map<string,string> mapHttpHeaderDetails = (*_pHttpClientPtr).processHttpHeader();
		map<string,string>::iterator it = mapHttpHeaderDetails.find("Status");

		if(mapHttpHeaderDetails.size() != 0 && it != mapHttpHeaderDetails.end()){
			bResult = ZoneminderMessage::isConnectionSuccessful(mapHttpHeaderDetails["Status"]);
		}else{
			bResult = false;
		}
	}else
	{
		bResult = false;
	}

	return bResult;
}

void TcpMpegDecoder::startDecoding(ThreadSafeQueue<Image>* pQueuePtr)
{
	char* prevBytePtr = NULL;
	char* curBytePtr = NULL;
	_bDecodeEnable = true;
	_bStillProcessing = true;

	//TODO : Remove the temp variable imageCount
	unsigned int imageCount = 10;

	while(_bDecodeEnable)
	{
		if(_iDecodedImageCount >= imageCount)
		{
			_bStillProcessing = false;
			break;
		}
		ReceivedDataStruct* dataStructPtr = (*_pHttpClientPtr).readBytes();

		ostringstream strOtherThanImageData;
		size_t i = 0;
		while(i < (*dataStructPtr).length)
		{
			curBytePtr = (*dataStructPtr).dataPtr + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == -1 && (*curBytePtr) == -40) //start of jpeg
			{
				string metaDataStr = strOtherThanImageData.str();
				extractStreamHeaderData(metaDataStr);

				if(_iCurrentContentLength > ((*dataStructPtr).length - i))
				{
					_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + (*dataStructPtr).length - i + 1));
					i = (*dataStructPtr).length;
				}
				else
				{
					_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + _iCurrentContentLength));
					i += _iCurrentContentLength - 1;
				}
			}
			else if (_vCurrentImageData.size() > 0)
			{
				if(_vCurrentImageData.size() < _iCurrentContentLength)
				{
					if(((*dataStructPtr).length - i) <= (_iCurrentContentLength - _vCurrentImageData.size()))
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + ((*dataStructPtr).length - i - 1 + 1)));
						i = (*dataStructPtr).length;
					}
					else
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + (_iCurrentContentLength - _vCurrentImageData.size() - 1 + 1)));
						i += (_iCurrentContentLength - _vCurrentImageData.size());
					}
				}
				else if(_vCurrentImageData.size() == _iCurrentContentLength)
				{
					if(_vCurrentImageData[_iCurrentContentLength - 2] == -1 && _vCurrentImageData[_iCurrentContentLength - 1] == -39) //end of jpeg
					{
						//got jpeg
						_iDecodedImageCount++;
						Image image(_vCurrentImageData, _sCurrentImageTimeStamp);
						(*pQueuePtr).push(image);
						//cout << "Decoded image no : " << _iDecodedImageCount << endl;
					}
					else //error
					{
						cerr << "TcpMpegDecoder:startDecoding: Error in decoding Mpeg stream." << std::endl;
					}
					_vCurrentImageData.clear();
				}
			}
			else if(_vCurrentImageData.size() == 0)
			{
				strOtherThanImageData << *curBytePtr;
				i++;
			}
			else
			{
				cerr << "TcpMpegDecoder:startDecoding: Error - cannot reach this part of the program." << std::endl;
			}
			prevBytePtr = curBytePtr;
		}
	}
}

void TcpMpegDecoder::extractStreamHeaderData(const string& streamHeaders)
{
	bool extractedContentLength = false;
	bool extractedTimestamp = false;
	vector<string> subStrs;
	size_t strPos;
	boost::split(subStrs, streamHeaders, boost::is_any_of("\n\r"));
	for (std::vector<std::string>::iterator subStrPtr = subStrs.begin(); subStrPtr != subStrs.end(); ++subStrPtr)
	{
		//TODO : substr can be simplified???
		if((strPos = (*subStrPtr).find(CONT_LEN_HEADER)) != string::npos)
		{
			string dataStr;
			dataStr = (*subStrPtr).substr((strPos + CONT_LEN_HEADER.length()), ((*subStrPtr).length() - CONT_LEN_HEADER.length()));
			_iCurrentContentLength= boost::lexical_cast<unsigned int>(dataStr);
			extractedContentLength = true;
		}

		if((strPos = (*subStrPtr).find(CUR_TIME_HEADER)) != string::npos)
		{
			_sCurrentImageTimeStamp = (*subStrPtr).substr((strPos + CUR_TIME_HEADER.length()), ((*subStrPtr).length() - CUR_TIME_HEADER.length()));
			extractedTimestamp = true;
		}
		else //if the time stamp is not in the image header, set it to the image decoded time stamp
		{
			ostringstream oss;
			oss << time(0);
			_sCurrentImageTimeStamp = oss.str();
			extractedTimestamp = true;
		}
	}
	if(!(extractedContentLength && extractedTimestamp))
	{
		std::cerr << "TcpMpegDecoder:extractStreamHeaderData: Error - cannot reach this part of the program." << std::endl;
	}
}

bool TcpMpegDecoder::isStillProcessing()
{
	return _bStillProcessing;
}

TcpMpegDecoder::~TcpMpegDecoder()
{
	delete _pHttpClientPtr; _pHttpClientPtr = NULL;
}

} /* namespace zoneminder */
