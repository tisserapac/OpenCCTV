/*
 * HttpClient.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#ifndef HTTPCLIENT_HPP_
#define HTTPCLIENT_HPP_

#include <boost/asio.hpp>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

namespace tcpsocket {

struct ReceivedDataStruct
{
	char* dataPtr;
	size_t length;
};

class HttpClient {

private:
	boost::asio::ip::tcp::socket *_socket;
	boost::asio::streambuf _streambufTemp;
	boost::array<char, 2048> _buffer;
	ReceivedDataStruct *_pCurrentDataStruct;
	map<string, string> _mapHttpHeaderLines;
	bool isResidualAvailable;

public:
	HttpClient(string sServerName, string sServerPort);
	int sendGetRequest(const string &host, const string &url);
	map<string,string> processHttpHeader();
	void write(string message);
	ReceivedDataStruct* readBytes();
	virtual ~HttpClient();

};

} /* namespace tcpsocket */

#endif /* HTTPCLIENT_HPP_ */
