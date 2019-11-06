#include <iostream>
#include "my_socket_client.h"
#include "amazing_pacman_ai.h"
#include "main.h"
void show_map(char* map)
{
	int i,j;

	for (i=0; i< 15; i++) {
		for (j =0; j< 15; j++) {
			std::cout << map[15*i + j] << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	bool result_socket_active = true;
	char newmap[1024] = {0};
	CAmazingPacManAi* amazing_pacman = new CAmazingPacManAi();
	CMySocketClient* sockettest = new CMySocketClient();
	
	while (!sockettest->create_socket()) {
		sleep(1);
		continue;
	}
	while (!sockettest->my_connect(MYSERVERIP, MYPROT)) {
		sleep(1);
		continue;
	}
	while (true)
	{
		result_socket_active = sockettest->my_recv(newmap);
		if (result_socket_active) {
			std::cout << __func__ << " : get server map success " << std::endl;
		} else {
			std::cout << __func__ << " : recv result_socket_active = " << result_socket_active << std::endl;
			break;
		}
		amazing_pacman->set_server_map(newmap);
//		show_map(newmap);
		result_socket_active = sockettest->my_send(amazing_pacman->get_next_godirection());
		if (result_socket_active) {
			std::cout << __func__ << " : send command success" << std::endl;
		} else {
			std::cout << __func__ << " : recv result_socket_active = " << result_socket_active << std::endl;
			std::cout << __func__ << " : game is over" << std::endl;
			break;
		}
	
	}
	delete sockettest;

	return 0;
}
