#include "pjsua2_server.h"
#include <pj/log.h>

#define THIS_FILE 	"pjsua2_server.cpp"

Pjsua2_Server::Pjsua2_Server()
{
	
}

Pjsua2_Server::~Pjsua2_Server()
{
			  
}

bool Pjsua2_Server::start()
{
	PJ_LOG(3, (THIS_FILE, "##########################Pjsua2_Server start##########################"));
	Pj_Inet_Addr addr;
	addr.set_address("192.168.1.18");
	addr.set_port_number(9345);
	if (PJ_SUCCESS != m_sock.bind(addr)) {
		PJ_LOG(3, (THIS_FILE, "##########################bind addr 127.0.0.1:12345 failed##########################"));
		return false;
	}

	if (PJ_SUCCESS != m_sock.listen()) {
		PJ_LOG(3, (THIS_FILE, "##########################listen failed##########################"));
		return false;
	}

	while (1) {
		PJ_LOG(3, (THIS_FILE, "##########################Pjsua2_Server accept##########################"));
		Pj_Sock_Stream newsock = m_sock.accept();
		PJ_LOG(3, (THIS_FILE, "##########################come newsock##########################"));
	}

	PJ_LOG(3, (THIS_FILE, "##########################Pjsua2_Server start end##########################"));
	return true;
}

void Pjsua2_Server::stop()
{

}