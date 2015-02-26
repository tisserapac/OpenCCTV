/*
 * VmsClient.hpp
 *
 *  Created on: Feb 24, 2015
 *      Author: anjana
 */

#ifndef VMSCLIENT_HPP_
#define VMSCLIENT_HPP_

#include "ThreadSafeQueue.hpp"
#include "Image.hpp"
#include "VmsConnectInfo.hpp"

using namespace std;

class VmsClient
{
public:
	VmsClient(){}
	virtual ~VmsClient(){}
	virtual bool init(VmsConnectInfo info) = 0;
	virtual int produceImageObject(ThreadSafeQueue<Image>* pQueue) = 0;
	virtual bool isStillProducingImages() = 0;
};

// the type definitions for the class factories
typedef VmsClient* create_t();
typedef void destroy_t(VmsClient*);

#endif /* VMSCLIENT_HPP_ */
