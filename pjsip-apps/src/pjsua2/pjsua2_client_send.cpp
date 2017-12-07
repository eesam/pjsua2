#include "pjsua2_client_send.h"
#include <pj/log.h>
#include <winsock2.h>

Pjsua2_ClientSend::Pjsua2_ClientSend()
{
	m_running = false;
}

Pjsua2_ClientSend::~Pjsua2_ClientSend()
{
}

bool Pjsua2_ClientSend::start(Pj_Sock_Stream* sock)
{
	if (m_running) {
		PJ_LOG(3, ("ClientSend", "already start"));
		return true;
	}
	m_running = true;
	m_pj_sock = sock;
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = m_pj_semaphore.create(&m_pj_pool, 1);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("ClientSend", "Pjsua2_ClientSend start create semaphore failed %d", ret));
		return false;
	}
	ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("ClientSend", "Pjsua2_ClientSend start create thread failed %d", ret));
		return false;
	}

	PJ_LOG(3, ("ClientSend", "Pjsua2_ClientSend start end"));
	return true;
}

void Pjsua2_ClientSend::stop()
{
	if (m_running == false) {
		PJ_LOG(3, ("ClientSend", "already stop"));
		return;
	}
	m_running = false;
	m_pj_sock = NULL;
	m_pj_semaphore.post();

	join();
	destroy();
	m_pj_semaphore.destroy();
	PJ_LOG(3, ("ClientSend", "Pjsua2_ClientSend stop end"));
}

int Pjsua2_ClientSend::main()
{
	while (m_running) {
		pj_status_t ret = m_pj_semaphore.wait();
		if (ret != PJ_SUCCESS) {
			PJ_LOG(3, ("ClientSend", "send thread wait ret=%d", ret));
			break;
		}
		PJ_LOG(3, ("ClientSend", "send data..."));
	}
	PJ_LOG(3, ("ClientSend", "send thread end"));
	return 0;
}
