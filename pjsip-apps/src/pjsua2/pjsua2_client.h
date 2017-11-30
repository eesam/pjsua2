#pragma once

#include "pjsua2_client_recv.h"
#include "pjsua2_client_send.h"

class Pjsua2_Client
{
public:
	Pjsua2_Client();
	~Pjsua2_Client();

	bool start(Pj_Sock_Stream* sock);
	void stop();
private:
	Pjsua2_ClientRecv m_client_recv;
	Pjsua2_ClientSend m_client_send;
};
