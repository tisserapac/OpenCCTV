/*
 * VmsConnectInfo.hpp
 *
 *  Created on: Feb 25, 2015
 *      Author: marikhu
 */

#ifndef VMSCONNECTINFO_HPP_
#define VMSCONNECTINFO_HPP_

#include <string>
using namespace std;

typedef struct
{
	string sServerIp;
	int iServerPort;
	string sUsername;
	string sPassword;
	string sCameraId;
	int iWidth;
	int iHeight;
	bool bKeepAspectRatio;
	bool bAllowUpsizing;
	int iCompressionRate;
} VmsConnectInfo;

#endif /* VMSCONNECTINFO_HPP_ */
