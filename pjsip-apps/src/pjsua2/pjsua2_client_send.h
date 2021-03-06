#pragma once

#include <pj++/sock.hpp>
#include <pj++/os.hpp>
#include <pj++/pool.hpp>

class Pjsua2_ClientSend : public Pj_Thread
{
public:
	Pjsua2_ClientSend();
	~Pjsua2_ClientSend();

	bool start(Pj_Sock_Stream* sock);
	void stop();

	virtual int main();
private:
	bool            m_running;
	Pj_Caching_Pool m_pj_caching_pool;
	Pj_Pool         m_pj_pool;
	Pj_Semaphore    m_pj_semaphore;
	Pj_Sock_Stream* m_pj_sock;
};
