/*
 * ZoneminderClient.hpp
 *
 *  Created on: Feb 3, 2015
 *      Author: anjana
 */

#ifndef ZONEMINDERCLIENT_HPP_
#define ZONEMINDERCLIENT_HPP_

#include <iostream>
#include <string>
#include "../VmsClient.hpp"
#include "TcpMpegDecoder.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::algorithm;
using namespace tcpsocket;

namespace zoneminder {

class ZoneminderClient : public VmsClient
{
private:
	TcpMpegDecoder* _pDecoderPtr;
	string _sUsername;
	string _sPassword;
	string _sServerName;
	string _sServerPort;
	string _sUrl;
	string _sIP;
	string _sCameraId;
	string _sFramesPerSecond;

	bool setParameters(string sParamString);

public:
	ZoneminderClient();
	virtual ~ZoneminderClient();

	bool init(string sParamString);
	void produceImageObjects(ThreadSafeQueue<Image>* pQueue);
	bool isStillProducingImages();
};

// the class factories
extern "C" VmsClient* create() {
    return new ZoneminderClient;
}

extern "C" void destroy(VmsClient* pVmsClient) {
    delete pVmsClient; pVmsClient = 0;
}

} /* namespace zoneminder */

#endif /* ZONEMINDERCLIENT_HPP_ */
