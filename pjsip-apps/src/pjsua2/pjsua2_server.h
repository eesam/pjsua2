#pragma once

#include <pj++/sock.hpp>
#include <pj++/os.hpp>
#include <pj++/pool.hpp>

// #include "json/json.h"

// pjsua2 server for deal UI message
class Pjsua2_Server : public Pj_Thread
{
public:
	Pjsua2_Server();
	~Pjsua2_Server();

	bool start();
	void stop();

	virtual int main();
private:
	Pjlib           m_pj_lib;
    Pj_Caching_Pool m_pj_caching_pool;
    Pj_Pool         m_pj_pool;
	Pj_Sock_Stream  m_pj_sock;
};
