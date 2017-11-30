#include "pjsua2_client_recv.h"
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
	m_pj_sock = sock;
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = m_pj_semaphore.create(&m_pj_pool, 1);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv start create semaphore failed %d", ret));
		return false;
	}

	m_running = true;
	ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv start create thread failed %d", ret));
		return false;
	}

	PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv start end"));
	return true;
}

void Pjsua2_ClientRecv::stop()
{
	m_running = false;
	join();
	destroy();

	m_pj_semaphore.destroy();
	PJ_LOG(3, ("ClientRecv", "Pjsua2_ClientRecv stop end"));
}

int Pjsua2_ClientRecv::main()
{
	while (m_running) {
		PJ_LOG(3, ("ClientRecv", "recv thread wait..."));
		memset(m_buf, 0, 1024*1024);
		m_pj_sock->recv((void *)m_buf, 1024*1024);

		// TODO
		PJ_LOG(3, ("ClientRecv", "recv data %s", m_buf));
	}
	PJ_LOG(3, ("ClientRecv", "recv thread end"));
	return 0;
}
