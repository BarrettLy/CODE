//my_socket_client.cpp
#include "my_socket_client.h"
#include <iostream>

CMySocketClient::CMySocketClient()
{
	m_local_socket = -1;
}

CMySocketClient::~CMySocketClient()
{
	close_socket();
}

void CMySocketClient::close_socket()
{
	if (m_local_socket != -1) {
		close(m_local_socket);	//关闭socket连接
	}

	m_local_socket = -1;
}

//创建一个socket
bool CMySocketClient::create_socket()
{
	if (m_local_socket == -1) {
		m_local_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_local_socket < 0) {
			output_message("客服端socket创建失败");
			m_local_socket = -1;
			return false;
		}
	}
	output_message("客户端socket创建成功");
	return true;
}

bool CMySocketClient::my_connect(const char* ip, const unsigned short prot)
{
	int nret = 1;
	if (m_local_socket != -1) {
		sockaddr_in m_serveraddr;
		memset(&m_serveraddr, 0, sizeof(m_serveraddr));
		m_serveraddr.sin_family = AF_INET;
		m_serveraddr.sin_port = htons(prot);
		m_serveraddr.sin_addr.s_addr = inet_addr(ip);
		nret = connect(m_local_socket, (sockaddr*)&m_serveraddr, sizeof(m_serveraddr));

		if (nret < 0) {
			output_message("服务器连接失败！");
			return false;
		}
		output_message("服务器连接成功！");

		my_send(SENDKEY);
		return true;
	}

	return false;
}

bool CMySocketClient::my_recv(char* newmap)
{
	char *ppos = m_server_data;
	time_t start ,end ;  
	double cost;    
	int result_recv = 0;
	int receive_length = 0;
	int server_datasize = 227;
	char server_buff[GET_SERVER_BUFSIZE] = {0};
	if (m_local_socket != -1)
	{
//		memset(server_buff, 0, GET_SERVER_BUFSIZE);
		time(&start);
		std::cout << "CMySocketClient-> " << __func__ <<  " --> start recv server_datasize = " << server_datasize << " receive_length = " << receive_length << std::endl;
		while(server_datasize != 0){
			memset(server_buff, '\0', GET_SERVER_BUFSIZE);
			result_recv = recv(m_local_socket, server_buff, GET_SERVER_BUFSIZE, 0);
	//		std::cout << "CMySocketClient-> " << __func__ << " : result_recv = " << result_recv << " strlen(server_buff) = " << strlen(server_buff) << std::endl;
			if (result_recv > 0) {
				if (0 == strcmp("OK", server_buff)) {
					std::cout << "CMySocketClient-> " << __func__ << " : serverdata is OK " << std::endl;
					continue;
				}
			//	output_message(server_buff);
				server_datasize -= result_recv;
				memcpy(ppos + receive_length, server_buff, result_recv);
//				memset(server_buff, 0, GET_SERVER_BUFSIZE);
				receive_length += result_recv;
				std::cout << "CMySocketClient-> " << __func__ << " : result_recv = " << result_recv << " server_datsize = " << server_datasize << " receive_length = " << receive_length << std::endl;
			} else {
				std::cout << "CMySocketClient-> " << __func__ << " : " << server_buff << std::endl;
				return false;
			}
		}
		time(&end);
                cost = difftime(end,start);
		std::cout << m_server_data << std::endl;
		std::cout << "CMySocketClient-> " << __func__ << " : end recv server_datasize = " << server_datasize << " receive_length = " << receive_length << std::endl;
		std::cout << "CMySocketClient-> " << __func__ << " : sizeof(m_server_data) = " << sizeof(m_server_data) << "  strlen(m_server_data) = " << strlen(m_server_data) << " timediff : " << cost << "\n" << std::endl;
		receive_length = 0;
		server_datasize = 227;
		memcpy(newmap, ppos+1, server_datasize-2);
		memset(ppos, 0, sizeof(m_server_data));
		
		return true;
		//resultRecv = read(m_nLocalSocket, ppos, sizeof(m_message));
	} else {
		output_message("当前与服务器未连接！");
		return false;
	}
}

bool CMySocketClient::my_send(const char* command)
{
	int result_recv = 0;
	char server_buff[GET_SERVER_BUFSIZE] = {0};

	if (m_local_socket != -1)
	{
		send(m_local_socket, command, strlen(command), 0);
		return true;
	} else {
		output_message("当前与服务器未连接");
		return false;
	}
}

void CMySocketClient::output_message(const char* outstr)
{
	std::cout << "CMySocketClient-> " << outstr << std::endl;
}
