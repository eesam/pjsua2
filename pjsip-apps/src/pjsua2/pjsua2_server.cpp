#include "pjsua2_server.h"
#include "pjsua2_client.h"
#include <pj/log.h>
#include <winsock2.h>
#include <list>

Pjsua2_Server::Pjsua2_Server()
{
	m_running = false;
}

Pjsua2_Server::~Pjsua2_Server()
{
}

bool Pjsua2_Server::start()
{
	m_running = true;
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server start create thread failed"));
		return false;
	}

	PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server start end"));
	return true;
}

void Pjsua2_Server::stop()
{
	m_running = false;
	m_pj_sock.close();
	join();
	destroy();
	PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server stop end"));
}

int Pjsua2_Server::main()
{
	Pj_Inet_Addr addr;
	addr.set_address("127.0.0.1");
	addr.set_port_number(12345);

	pj_status_t ret = m_pj_sock.create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, ("pjsua2_server", "create socket failed %d", ret));
		return -1;
	}

	ret = m_pj_sock.bind(addr);
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, ("pjsua2_server", "bind addr 127.0.0.1:12345 failed %d", ret));
		return -1;
	}

	ret = m_pj_sock.listen();
	if (PJ_SUCCESS != ret) {
		PJ_LOG(3, ("pjsua2_server", "listen failed %d", ret));
		return -1;
	}

	std::list<Pjsua2_Client*> list_client;
	while (m_running) {
		PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server accept"));
		Pj_Sock_Stream sock = m_pj_sock.accept();
		if (sock.get_handle() == -1) {
			PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server accept failed"));
			break;
		}

		Pj_Sock_Stream newsock = sock;
		PJ_LOG(3, ("pjsua2_server", "Pjsua2_Server newsock %d", newsock.get_handle()));
		Pjsua2_Client *client = new Pjsua2_Client();
		client->start(&newsock);
		list_client.push_back(client);
		//Pj_Inet_Addr peer_addr;
		//ret = newsock.getpeername(&peer_addr);
		//PJ_LOG(3, ("pjsua2_server", "ret %d. come newsock %s:%d", ret, peer_addr.get_address(), peer_addr.get_port_number()));
	}

	std::list<Pjsua2_Client*>::iterator iter = list_client.begin();
	for (; iter != list_client.end(); ++iter) {
		delete *iter;
	}
	return 0;
}
