#include "pjsua2_handle.h"
#include <pj/log.h>

Pjsua2_Handle::Pjsua2_Handle()
{
	m_running = false;
}

Pjsua2_Handle::~Pjsua2_Handle()
{
}

bool Pjsua2_Handle::start()
{
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = m_pj_semaphore.create(&m_pj_pool, 1);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("Handle", "Pjsua2_Handle start create semaphore failed %d", ret));
		return false;
	}

	m_running = true;
	ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("Handle", "Pjsua2_Handle start create thread failed %d", ret));
		return false;
	}

	PJ_LOG(3, ("Handle", "Pjsua2_Handle start end"));
	return true;
}

void Pjsua2_Handle::stop()
{
	m_running = false;
	m_pj_semaphore.post();

	join();
	destroy();

	m_pj_semaphore.destroy();
	PJ_LOG(3, ("Handle", "Pjsua2_Handle stop end"));
}

int Pjsua2_Handle::main()
{
	while (m_running) {
		pj_status_t ret = m_pj_semaphore.wait();
		if (ret != PJ_SUCCESS) {
			PJ_LOG(3, ("Handle", "send thread wait ret=%d", ret));
			break;
		}

		// TODO
		PJ_LOG(3, ("Handle", "m_pj_semaphore do something"));
	}
	PJ_LOG(3, ("Handle", "handle thread end"));
	return 0;
}
