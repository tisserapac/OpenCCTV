/*
 * TcpMpegDecoder.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#ifndef TCPMPEGDECODER_HPP_
#define TCPMPEGDECODER_HPP_

#include <vector>
#include "../tcpsocket/HttpClient.hpp"
#include "../ThreadSafeQueue.hpp"
#include "../Image.hpp"
#include "ZoneminderMessage.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;
using namespace tcpsocket;

namespace zoneminder {

class TcpMpegDecoder {
private:
	vector<char> _vCurrentImageData;
	unsigned int _iCurrentContentLength;
	string _sCurrentImageTimeStamp;
	HttpClient *_pHttpClientPtr;
	bool _bDecodeEnable;
	bool _bStillProcessing;
	unsigned int _iDecodedImageCount;

	const string CONT_LEN_HEADER = "Content-Length: ";
	const string CUR_TIME_HEADER = "Timestamp: ";

	void extractStreamHeaderData(const string& streamHeaders);

public:
	TcpMpegDecoder();
	bool init(string sServerName, string sServerPort, string sUrl);
	//void startDecoding(ThreadSafeQueue<Image>& queuePtr, unsigned int imageCount);
	void startDecoding(ThreadSafeQueue<Image>* pQueuePtr);
	bool isStillProcessing();
	virtual ~TcpMpegDecoder();
};

} /* namespace zoneminder */

#endif /* TCPMPEGDECODER_HPP_ */
