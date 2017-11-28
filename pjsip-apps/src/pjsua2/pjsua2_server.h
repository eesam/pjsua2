#pragma once

#include <pj++/sock.hpp>

#include "json/json.h"

// pjsua2 server for deal UI message
class Pjsua2_Server
{
public:
	Pjsua2_Server();
	~Pjsua2_Server();

	bool start();
	void stop();
private:
	Pj_Sock_Stream m_sock;
};
