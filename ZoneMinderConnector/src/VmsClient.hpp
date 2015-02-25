/*
 * VmsClient.hpp
 *
 *  Created on: Feb 24, 2015
 *      Author: anjana
 */

#ifndef VMSCLIENT_HPP_
#define VMSCLIENT_HPP_

#include <string>
#include "ThreadSafeQueue.hpp"
#include "Image.hpp"

using namespace std;

class VmsClient
{
public:
	VmsClient(){}
	virtual ~VmsClient(){}
	virtual void produceImageObjects(ThreadSafeQueue<Image>* pQueue) = 0;
	virtual bool init(string sParamString) = 0;
	virtual bool isStillProducingImages() = 0;
};

// the type definitions for the class factories
typedef VmsClient* create_t();
typedef void destroy_t(VmsClient*);

#endif /* VMSCLIENT_HPP_ */
