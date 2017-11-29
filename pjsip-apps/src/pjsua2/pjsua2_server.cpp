#include "pjsua2_server.h"
#include <pj/log.h>
#include <pj/compat/errno.h>
#include <winsock2.h>

#define THIS_FILE 	"pjsua2_server"

Pjsua2_Server::Pjsua2_Server()
{
}

Pjsua2_Server::~Pjsua2_Server()
{
}

bool Pjsua2_Server::start()
{
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0, "m_pj_pool", NULL));
	create(&m_pj_pool, 0, "m_pj_thread");
	PJ_LOG(3, (THIS_FILE, "Pjsua2_Server start end"));
	return true;
}

void Pjsua2_Server::stop()
{
	PJ_LOG(3, (THIS_FILE, "Pjsua2_Server stop end"));
}

int Pjsua2_Server::main()
{
	printf("Pjsua2_Server::%s\n", __FUNCTION__);
	Pj_Inet_Addr addr;
	addr.set_address("127.0.0.1");
	addr.set_port_number(12345);

	pj_status_t ret = m_pj_sock.create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, (THIS_FILE, "create socket failed %d", ret));
		return -1;
	}

	ret = m_pj_sock.bind(addr);
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, (THIS_FILE, "bind addr 127.0.0.1:12345 failed %d", ret));
		return -1;
	}

	ret = m_pj_sock.listen();
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, (THIS_FILE, "listen failed %d", ret));
		return -1;
	}

	while (1) {
		PJ_LOG(3, (THIS_FILE, "Pjsua2_Server accept"));
		Pj_Sock_Stream newsock = m_pj_sock.accept();
		PJ_LOG(3, (THIS_FILE, "come newsock"));
	}
	return 0;
}
