#pragma once

#include <pj++/sock.hpp>
#include <pj++/os.hpp>
#include <pj++/pool.hpp>
#include "pjsua2_handle.h"
#include "pjsua2_client_recv.h"
#include "pjsua2_client_send.h"

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
	bool            m_running;
	Pjlib           m_pj_lib;
	Pj_Caching_Pool m_pj_caching_pool;
	Pj_Pool         m_pj_pool;
	Pj_Sock_Stream  m_pj_sock;
};

Pjsua2_Server* getServerInstance();
Pjsua2_Handle* getHandleInstance();
Pjsua2_ClientSend* getClientSendInstance();
Pjsua2_ClientRecv* getClientRecvInstance();
