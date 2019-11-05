//my_socket_client.h
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>

#define GET_SERVER_BUFSIZE 300
#define SENDKEY "(8f32184c8d2c427084202c5b49a984ae)"

class CMySocketClient
{
public:
	CMySocketClient();
	~CMySocketClient();

	bool create_socket();
	void close_socket();

	bool my_connect(const char* ip, const unsigned short prot);
	bool my_send(const char* command);
	bool my_recv(char* newmap);

	void output_message(const char* outstr);

private:
	int m_local_socket;
	char m_server_data[1024];
};
