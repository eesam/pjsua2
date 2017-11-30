#include "pjsua2_client.h"
#include <pj/log.h>

Pjsua2_Client::Pjsua2_Client()
{
}

Pjsua2_Client::~Pjsua2_Client()
{
}

bool Pjsua2_Client::start(Pj_Sock_Stream* sock)
{
	m_client_recv.start(sock);
	m_client_send.start(sock);
	return true;
}

void Pjsua2_Client::stop()
{
	m_client_recv.stop();
	m_client_send.stop();
}
