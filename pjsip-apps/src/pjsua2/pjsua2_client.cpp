#include "pjsua2_client.h"
#include <pj/log.h>

Pjsua2_Client::Pjsua2_Client()
{
}

Pjsua2_Client::~Pjsua2_Client()
{
	stop();
}

bool Pjsua2_Client::start(Pj_Sock_Stream* sock)
{
	m_pj_sock = sock;
	m_client_recv.start(sock);
	m_client_send.start(sock);
	return true;
}

void Pjsua2_Client::stop()
{
	if (m_pj_sock) {
		m_pj_sock->close();
		m_pj_sock = NULL;
	}

	m_client_recv.stop();
	m_client_send.stop();
}
