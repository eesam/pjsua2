#include "pjsua2_handle.h"
#include "pjsua2_server.h"
#include <pj/log.h>
#include "json/json.h"
#include <iostream>
#include <string>

const unsigned long CMD_REGISTER = 0x00000001;

const Pjsua2_Handle::HandlerEntry Pjsua2_Handle::mHandlers[] =
{
	{ CMD_REGISTER,	 &Pjsua2_Handle::onRegister },

};

Pjsua2_Handle::Pjsua2_Handle()
{
	m_running = false;
}

Pjsua2_Handle::~Pjsua2_Handle()
{
}

bool Pjsua2_Handle::start()
{
	m_client_over = false;
	m_running = true;
	m_pj_pool.attach(m_pj_caching_pool.create_pool(10*1024*1024, 0));

	pj_status_t ret = m_pj_semaphore.create(&m_pj_pool, 1);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("Pjsua2_Handle", "Pjsua2_Handle start create semaphore failed %d", ret));
		PJ_LOG(3, ("Handle", "Pjsua2_Handle start create semaphore failed %d", ret));
		return false;
	}

	ret = m_pj_mutex.create(&m_pj_pool, Pj_Mutex::DEFAULT);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("Handle", "Pjsua2_Handle start create mutex failed %d", ret));
		return false;
	}

	ret = create(&m_pj_pool, 0);
	if (ret != PJ_SUCCESS) {
		PJ_LOG(3, ("Pjsua2_Handle", "Pjsua2_Handle start create thread failed %d", ret));
		PJ_LOG(3, ("Handle", "Pjsua2_Handle start create thread failed %d", ret));
		return false;
	}

	PJ_LOG(3, ("Pjsua2_Handle", "Pjsua2_Handle start end"));
	m_running = true;
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
	PJ_LOG(3, ("Pjsua2_Handle", "Pjsua2_Handle stop end"));
	m_pj_mutex.destroy();
	PJ_LOG(3, ("Handle", "Pjsua2_Handle stop end"));
}

void Pjsua2_Handle::client_over(Pj_Sock_Stream* sock) {
	sock->close();

	m_client_over = true;
	m_pj_semaphore.post();
}

void Pjsua2_Handle::append_data(unsigned char* data, int data_len) {
	m_pj_mutex.acquire();
	PJ_LOG(3, ("Handle", "1 append_data len %d", data_len));
	if ( (m_data_offset + data_len) >= DATA_MAX_BUFSIZE ) {
		return;
	}
	memcpy(m_buffer, data, data_len);
	m_data_offset += data_len;
	m_pj_mutex.release();
	PJ_LOG(3, ("Handle", "2 append_data len %d", m_data_offset));

	m_pj_semaphore.post();
}

int Pjsua2_Handle::main()
{
	while (m_running) {
		PJ_LOG(3, ("Pjsua2_Handle", "handle thread wait..."));
		pj_status_t ret = m_pj_semaphore.wait();
		if (ret != PJ_SUCCESS) {
			PJ_LOG(3, ("Handle", "send thread wait ret=%d", ret));
			break;
		}

		PJ_LOG(3, ("Pjsua2_Handle", "m_pj_semaphore do something"));
		if (m_client_over) {
			m_client_over = false;
			PJ_LOG(3, ("Pjsua2_Handle", "1"));
			getClientRecvInstance()->stop();
			PJ_LOG(3, ("Pjsua2_Handle", "2"));
			getClientSendInstance()->stop();
			PJ_LOG(3, ("Pjsua2_Handle", "3"));
		}

		std::list<std::string> msg_list;
		m_pj_mutex.acquire();
		while (m_data_offset > 10) {
			// Find Packet Flag: PJSUA2
			findToHeader();

			if (m_data_offset <= 10) {
				break;
			}

			std::string msg;
			if (false == parseData(msg)) {
				break;
			}

			msg_list.push_back(msg);
			eraseData(msg.size() + 10);
		}
		m_pj_mutex.release();

		std::list<std::string>::iterator iter = msg_list.begin();
		for (; iter != msg_list.end(); ++iter) {
			process(*iter);
		}
	}
	PJ_LOG(3, ("Pjsua2_Handle", "handle thread end"));
	PJ_LOG(3, ("Handle", "handle thread end"));
	return 0;
}

bool Pjsua2_Handle::parseData(std::string& msg) {
	int offset = 6;
	char temp[5] = {0};
	memcpy(temp, m_buffer+offset, 4);
	int data_len = atoi(temp);
	offset += 4;
	PJ_LOG(3, ("Handle", "parseData data_len %d", data_len));

	if (m_data_offset < data_len + offset) {
		PJ_LOG(3, ("Handle", "handlecur data size(%d) not enough. need(%d)", m_data_offset, data_len));
		return false;
	}

	msg.assign((const char*)m_buffer+offset, data_len);
	return true;
}

void Pjsua2_Handle::findToHeader() {
	int i_offset = 0;
	for (; (i_offset+6) < m_data_offset; i_offset++) {
		if ( (m_buffer[i_offset] == 'P') &&
			 (m_buffer[i_offset+1] == 'J') &&
			 (m_buffer[i_offset+2] == 'S') &&
			 (m_buffer[i_offset+3] == 'U') &&
			 (m_buffer[i_offset+4] == 'A') &&
			 (m_buffer[i_offset+5] == '2') ) {
			break;
		}
	}
	eraseData(i_offset);
}

void Pjsua2_Handle::eraseData(int size) {
	if (size <= 0) return;
	if (m_data_offset - size > 0) {
		memmove(m_buffer, m_buffer + size, m_data_offset - size);
	}
	m_data_offset -= size;
}

void Pjsua2_Handle::process(std::string& msg) {
	PJ_LOG(3, ("Handle", "process %s", msg.c_str()));
	//Json::Reader reader;
	//Json::Value root_read;
	//reader.parse(msg, root_read, false);
	//std::cout << root_read["key"];
}

int Pjsua2_Handle::onRegister(unsigned char* data, int len) {
	return 0;
}
