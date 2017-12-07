#pragma once

#include <pj++/sock.hpp>
#include <pj++/os.hpp>
#include <pj++/pool.hpp>
#include <list>

#define DATA_MAX_BUFSIZE 10240

class Pjsua2_Handle : public Pj_Thread
{
public:
	Pjsua2_Handle();
	~Pjsua2_Handle();

	bool start();
	void stop();

	void append_data(unsigned char* data, int data_len);
	void client_over(Pj_Sock_Stream* sock);
	virtual int main();
private:
	int  onRegister(unsigned char* data, int len);

	bool parseData(std::string& msg);
	void findToHeader();
	void eraseData(int size);
	void process(std::string& msg);

	bool            m_running;
	Pj_Caching_Pool m_pj_caching_pool;
	Pj_Pool         m_pj_pool;
	Pj_Semaphore    m_pj_semaphore;
	Pj_Mutex        m_pj_mutex;
	bool            m_client_over;

	typedef int (Pjsua2_Handle::*PMFHANDLER)(unsigned char*, int);
	struct HandlerEntry
	{
		unsigned long command;
		PMFHANDLER pmfHandler;
	};
	int m_data_offset;
	unsigned char m_buffer[DATA_MAX_BUFSIZE];
	static const HandlerEntry mHandlers[];

};
