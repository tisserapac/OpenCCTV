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
#include "../VmsConnectInfo.hpp"
#include "TcpMpegDecoder.hpp"

using namespace std;
using namespace tcpsocket;

namespace zoneminder {

class ZoneminderClient : public VmsClient
{
private:
	TcpMpegDecoder* _pDecoderPtr;
	bool getZMLiveStreamDetails(const string& sServerName, const int iServerPort, const string& sUsername, const string& sPassword, const string& sCameraId, string& sAuth, string& sConnkey, string& sRand);

public:
	ZoneminderClient();
	virtual ~ZoneminderClient();

	bool init(VmsConnectInfo info);
	int produceImageObject(ThreadSafeQueue<Image>* pQueue);
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
