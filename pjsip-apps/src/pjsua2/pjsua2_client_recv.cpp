#include "pjsua2_client_recv.h"
#include "pjsua2_server.h"
#include <pj/log.h>
#include <winsock2.h>

Pjsua2_ClientRecv::Pjsua2_ClientRecv()
{
	m_running = false;
}

Pjsua2_ClientRecv::~Pjsua2_ClientRecv()
{
}

bool Pjsua2_ClientRecv::start(Pj_Sock_Stream* sock)
{
	if (m_running) {
		PJ_LOG(3, ("ClientRecv", "already start"));
		return true;
	}
	m_running = true;
	m_pj_sock = sock;
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv start create thread failed %d", ret));
		return false;
	}

	PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv start end"));
	return true;
}

void Pjsua2_ClientRecv::stop()
{
	if (m_running == false) {
		PJ_LOG(3, ("ClientRecv", "already stop"));
		return;
	}
	m_running = false;
	m_pj_sock = NULL;
	join();
	destroy();
	PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv stop end"));
}

int Pjsua2_ClientRecv::main()
{
	while (m_running) {
		PJ_LOG(3, ("ClientRecv", "recv thread wait..."));
		memset(m_buf, 0, 1024*1024);
		pj_ssize_t recv_len = m_pj_sock->recv((void *)m_buf, 1024*1024);
		PJ_LOG(3, ("ClientRecv", "recv len %d data %s", recv_len, m_buf));
		if (recv_len <= 0) {
			break;
		}
		getHandleInstance()->append_data(m_buf, recv_len);
	}
	PJ_LOG(3, ("ClientRecv", "recv thread end"));
	getHandleInstance()->client_over(m_pj_sock);
	return 0;
}
