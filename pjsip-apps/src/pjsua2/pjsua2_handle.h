#pragma once

#include <pj++/sock.hpp>
#include <pj++/os.hpp>
#include <pj++/pool.hpp>

class Pjsua2_Handle : public Pj_Thread
{
public:
	Pjsua2_Handle();
	~Pjsua2_Handle();

	bool start();
	void stop();

	virtual int main();
private:
	bool            m_running;
	Pj_Caching_Pool m_pj_caching_pool;
	Pj_Pool         m_pj_pool;
	Pj_Semaphore    m_pj_semaphore;
};
