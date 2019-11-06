//amazing_pacman_ai.cpp
#include "amazing_pacman_ai.h"
#include <iostream>

CAmazingPacManAi::CAmazingPacManAi()
{
	m_current_position[0] = 0;
	m_current_position[1] = 0; 
	memset(&walked_position_list, '\0', sizeof(walked_position_list));
//	std::cout << " memset walked_position_list : " << walked_position_list.position_list[0][0] << ", current_number : " << walked_position_list.current_number << std::endl;
}

CAmazingPacManAi::~CAmazingPacManAi()
{
       
}

bool CAmazingPacManAi::avoid_ghost(int* avoid_directions)
{
	int avoid_direction[2] = {0};
	int dangerous[3] = {0};
	int i = 0;
	int j = 0;
	int k = 0;
	int dangerous_position[3] = {0};
	int available_direction[4] = {0};
	int ghost_position[3][2] = {0};
	int current_position[2] = {0};
	bool isdangerous = false;

	*avoid_directions = -1;
	get_current_position(current_position);
	get_dangerous_from_ghost(go_stop, dangerous);
	for (i=0; i<3; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : current_dangerous[" << i << "] = " << dangerous[i] << std::endl;	
	}
	//判断有几个距离幽灵小于２
	for (i=0; i<3; i++) {
		if (dangerous[i] <= 3 && dangerous[i] > 0) {
			dangerous_position[j++] = i;
			isdangerous = true;
		}
	}
	get_three_ghost_position(ghost_position);
	//处理小于3的危险
	for (i=0; i<j; i++) {
		for (k=0; k<2; k++) {
			avoid_direction[k] = ghost_position[dangerous_position[i]][k] - current_position[k];
			if (k == 0) {
				if (avoid_direction[k] < 0) {
					avoid_direction[0] = -1;
					available_direction[3] = -1;
				}
				if (avoid_direction[k] > 0) {
					avoid_direction[0] = 1;
					available_direction[2] = -1;
				}
			}
			if (k == 1) {
				if (avoid_direction[k] < 0) {
					avoid_direction[0] = -1; 
					available_direction[0] = -1;
				}
				if (avoid_direction[k] > 0) {
					avoid_direction[0] = 1;
					available_direction[1] = -1;
				}
			}
		}		

	}
	for (i=0; i<4; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : available_direction[" << i << "] = " << available_direction[i] << std::endl;
		if (available_direction[i] >= 0 && is_direction_move(i)) {
			*avoid_directions = i;
		}
	}
	if (*avoid_directions == -1) {
		*avoid_directions = 4;
	}
	return isdangerous;
}

void CAmazingPacManAi::set_three_ghost_position()
{
	int position = 0;
	int i = 0;

	for (i=0; i<strlen(m_server_map); i++) {
		if ('G' == m_server_map[i]) {
			m_ghost_position[position][0] = i/15;
			m_ghost_position[position][1] = i%15;
			std::cout << "CAmazingPacManAi-> " << __func__ << " : ghost_position[" << position << "] = (" << m_ghost_position[position][0] << "," << m_ghost_position[position][1] << ")" << std::endl;
			position++;
		}
	}
	
}

void CAmazingPacManAi::get_three_ghost_position(int position[][2])
{
	int i = 0;
	for (i=0; i<3; i++) {
		position[i][0] = m_ghost_position[i][0];
		position[i][1] = m_ghost_position[i][1];
	}
}

void CAmazingPacManAi::set_server_map(const char* servermap)
{
	int i = 0;
	int current_position = 0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	memcpy(m_server_map, servermap, strlen(servermap));
	for (i=0; i<strlen(servermap); i++) {
		if ( m_server_map[i] == 's' || m_server_map[i] == 'd' || m_server_map[i] == 'w' || m_server_map[i] == 'a') {
			current_position = i;
			break;
		}
	}
	m_current_position[0] = current_position/15;
	m_current_position[1] = current_position%15;
	std::cout << "CAmazingPacManAi-> " << __func__ << " : Pacman get map, his current_position = " << current_position << " --> (" << m_current_position[0] << ", " << m_current_position[1] << ")" << std::endl;

	if (m_current_position[0] == walked_position_list.position_list[walked_position_list.current_number][0] && m_current_position[1] == walked_position_list.position_list[walked_position_list.current_number][1]) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : 位置未改变" << std::endl;
	} else {
		if (m_current_position[0] == walked_position_list.position_list[walked_position_list.current_number-1][0] && m_current_position[1] == walked_position_list.position_list[walked_position_list.current_number-1][1]) {
			std::cout << "CAmazingPacManAi-> " << __func__ << " : 和前一个位置相同进行了回退" << std::endl;
			remove_one_walked_position_fromlist();
		} else {
			std::cout << "CAmazingPacManAi-> " << __func__ << " : 行进了新的位置" << std::endl;
			add_one_walked_position_tolist(m_current_position);
		}
	}
	set_three_ghost_position();
	show_map();
}

//%s/vivian/sky/g 字符串替换命令

bool CAmazingPacManAi::add_one_walked_position_tolist(int* position)
{
	if (walked_position_list.current_number < 399 && walked_position_list.current_number >= 0) {
		walked_position_list.current_number++;
		walked_position_list.position_list[walked_position_list.current_number][0] = position[0];
		walked_position_list.position_list[walked_position_list.current_number][1] = position[1];
		return true;
	} else {
		return false;
	}
}

bool CAmazingPacManAi::remove_one_walked_position_fromlist()
{
	if (walked_position_list.current_number > 0 && walked_position_list.current_number < 400) {
		walked_position_list.current_number--;
		return true;
	} else {
		return false;
	}
}

void CAmazingPacManAi::get_dangerous_from_ghost(int direction, int* dangerous)
{
	int i = 0;
	int j = 0;
	int current_position[2] = {0};
	int mindanger = 0;

	get_current_position(current_position);
	move_position(direction);
	for (i=0; i<3; i++) {
		dangerous[i] = abs(m_current_position[0]-m_ghost_position[i][0]) + abs(m_current_position[1]-m_ghost_position[i][1]);
		std::cout << "CAmazingPacManAi-> " << __func__ << " : dangerous[" << i << "] = " << dangerous[i] << std::endl;
	}
	set_current_position(current_position);
}

void CAmazingPacManAi::set_current_position(int* position)
{
	std::cout << "CAmazingPacManAi-> " << __func__ << " : position =  (" << position[0] << "," << position[1] << ")" << std::endl;
	m_current_position[0] = position[0];
	m_current_position[1] = position[1];
}

void CAmazingPacManAi::set_four_current_position_states()
{
	int i = 0;
	int position_number =  0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	for (i=0; i<4; i++) {
		get_dangerous_from_ghost(i,current_position_states[i].dangerous); //this is test 
//		std::cout << "CAmazingPacManAi-> " << __func__ << " : current_position[" << i << "].dangerous = " << current_position_states[i].dangerous << std::endl;
		current_position_states[i].iswalk = is_direction_move(i);
		position_number = get_position_number(i);
		if (position_number < 0 || position_number >=225) {
			current_position_states[i].weight = -1;
		} else {		
			current_position_states[i].weight = get_position_weight(position_number); //m_server_map[position_number];
		}
		std::cout << "CAmazingPacManAi-> " << __func__ << " : current_position_states[i].weight = " << current_position_states[i].weight << std::endl;

	}
}

void CAmazingPacManAi::set_four_next_position_states()
{
	int position_number = 0;
	int current_position[2];
	int average_weight[3] = {0};
	int i = 0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	get_current_position(current_position);
	move_position(go_up);
	position_number = get_position_number(go_up);
	average_weight[0] = get_position_weight(position_number);
	position_number = get_position_number(go_right);
	average_weight[1] = get_position_weight(position_number);
	position_number = get_position_number(go_left);
	average_weight[2] = get_position_weight(position_number);
	next_position_states[0].around_weight = (average_weight[0] + average_weight[1] + average_weight[2])/3;
//	next_position_states[0].dangerous = 0; //this is test
	set_current_position(current_position);
	for (i=0; i<3; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : average_weight[" << i << "] = " << average_weight[i] << std::endl;
	}
	move_position(go_down);
	position_number = get_position_number(go_down);
	average_weight[0] = get_position_weight(position_number);
	position_number = get_position_number(go_right);
	average_weight[1] = get_position_weight(position_number);
	position_number = get_position_number(go_left);
	average_weight[2] = get_position_weight(position_number);
	next_position_states[1].around_weight = (average_weight[0] + average_weight[1] + average_weight[2])/3;
//	next_position_states[1].dangerous = 0; //this is test
	set_current_position(current_position);
	for (i=0; i<3; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : average_weight[" << i << "] = " << average_weight[i] << std::endl;
	}
	
	move_position(go_right);
	position_number = get_position_number(go_right);
	average_weight[0] = get_position_weight(position_number);
	position_number = get_position_number(go_up);
	average_weight[1] = get_position_weight(position_number);
	position_number = get_position_number(go_down);
	average_weight[2] = get_position_weight(position_number);
	next_position_states[2].around_weight = (average_weight[0] + average_weight[1] + average_weight[2])/3;
//	next_position_states[2].dangerous = 0; //this is test
	set_current_position(current_position);
	for (i=0; i<3; i++) {
	std::cout << "CAmazingPacManAi-> " << __func__ << " : average_weight[" << i << "] = " << average_weight[i] << std::endl;
	}

	move_position(go_left);
	position_number = get_position_number(go_left);
	average_weight[0] = get_position_weight(position_number);
	position_number = get_position_number(go_up);
	average_weight[1] = get_position_weight(position_number);
	position_number = get_position_number(go_down);
	average_weight[2] = get_position_weight(position_number);
	next_position_states[3].around_weight = (average_weight[0] + average_weight[1] + average_weight[2])/3;
//	next_position_states[3].dangerous = 0; //this is test
	set_current_position(current_position);
	for (i=0; i<3; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : average_weight[" << i << "] = " << average_weight[i] << std::endl;
	}
}

int CAmazingPacManAi::get_position_weight(int position_number)
{
	int weight = (int)(m_server_map[position_number] - '0');
	std::cout << "CAmazingPacManAi-> " << __func__ << " : char m_server_map[" << position_number << "] = " << m_server_map[position_number] <<  " -> change char to int m_server_map[" << position_number << "] = " << weight << std::endl;
	if ((m_server_map[position_number] == 'G') || (m_server_map[position_number] == '9')) {
		weight = -1;
	}
	if (position_number < 0 || position_number > 224) {
		weight = -1;
	}
	return weight;
}

int CAmazingPacManAi::get_position_number(int direction)
{
	int position_number = 0;
	int next_position[2] = {0};
	
	get_current_position(next_position);	
	switch(direction)
        {
                case go_up:
			next_position[0] = m_current_position[0] + up[0];
			next_position[1] = m_current_position[1] + up[1];
                        break;
                case go_down:
			next_position[0] = m_current_position[0] + down[0];
                        next_position[1] = m_current_position[1] + down[1];
                        break;
                case go_right:
			next_position[0] = m_current_position[0] + right[0];
                        next_position[1] = m_current_position[1] + right[1];
                        break;
                case go_left:
			next_position[0] = m_current_position[0] + left[0];
                        next_position[1] = m_current_position[1] + left[1];
                        break;
		case go_stop:
			next_position[0] = m_current_position[0] + stop[0];
			next_position[0] = m_current_position[0] + stop[1];
                default:
                        break;
        }
	if (next_position[0] < 0 || next_position[1] < 0 || next_position[0] > 14 || next_position[1] > 14) {
		position_number = -1;
	} else {
		position_number = 15*(next_position[0]) + next_position[1];
	}
	return position_number;
}

void CAmazingPacManAi::get_current_position(int* position)
{
	position[0] = m_current_position[0];
	position[1] = m_current_position[1];
}

void CAmazingPacManAi::move_position(int direction)
{
	bool ismove = false;

	std::cout << "CAmazingPacManAi-> " << __func__ << " : move the direction is = " << direction << " ";
	switch(direction)
	{
		case go_up:
			std::cout << "--> go_up" << std::endl;
			ismove = is_direction_move(go_up);
			if (ismove) {
				m_current_position[0] += up[0];
				m_current_position[1] += up[1];
			}
			break;
		case go_down:
			std::cout << "--> go_down" << std::endl;
			ismove = is_direction_move(go_down);
			if (ismove) {
				m_current_position[0] += down[0];
				m_current_position[1] += down[1];
			}
			break;
		case go_right:
			std::cout << "--> go_right" << std::endl;
			ismove = is_direction_move(go_right);
			if (ismove) {
				m_current_position[0] += right[0];
				m_current_position[1] += right[1];
			}
			break;
		case go_left:
			std::cout << "--> go_left" << std::endl;
			ismove = is_direction_move(go_left);
			if (ismove) {
				m_current_position[0] += left[0];
				m_current_position[1] += left[1];
			}
			break;
		case go_stop:
			std::cout << "--> go_stop" << std::endl;
			m_current_position[0] += stop[0];
			m_current_position[1] += stop[1];
		default:
			break;
	}
	std::cout << "CAmazingPacManAi-> " << __func__ << " : now we are here = (" << m_current_position[0] << "," << m_current_position[1] << ")" << std::endl;
}

bool CAmazingPacManAi::is_direction_move(int direction)
{
	int position_number = 0;
	int weight = 0;

	position_number = get_position_number(direction);	
	std::cout << "CAmazingPacManAi-> " << __func__ << " : I want the direction is = " << direction << ", it's position_number is = " << position_number << ", " ;
	if (position_number < 0 || position_number > 224) {
		std::cout << "but this position_number is over map, pacman can't move return false" << std::endl;
		return false;
	} else {
		std::cout << "this position_number between 0-224, m_server_map[position_number] = " << m_server_map[position_number] << " ";
	}
	if (('9' == m_server_map[position_number]) || ('G' == m_server_map[position_number])) {
		std::cout << "but this position m_server_map[position_number] = 9 or G, pacman can't move return false" << std::endl;
		return false;
	} else {
		std::cout << "return true" << std::endl;
		return true;
	}
}

int CAmazingPacManAi::get_current_max_weight(int* direction, int* max_weight)
{

	int weight[4][2] = {0};
	int i = 0;
	int j = 0;
	int tmp[2] = {0};
	int numbermax = 0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	for (i=0; i<4; i++) {
		weight[i][0] = i;
		if (current_position_states[i].iswalk) {
			weight[i][1] = current_position_states[i].weight;
		} else {
			weight[i][1] = -1;
		}
	}
	
	for (i=0; i<4; i++){
		std::cout << "CAmazingPacManAi-> " << __func__ <<  " : --current  weight[" << i << "] : " << weight[i][1] << " go_direction : " << weight[i][0] << std::endl;
	}
	//冒泡
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			if (weight[j][1] < weight[j+1][1]) {
				tmp[1] = weight[j][1];
				tmp[0] = weight[j][0];
				weight[j][1] = weight[j+1][1];
				weight[j][0] = weight[j+1][0];
				weight[j+1][1] = tmp[1];
				weight[j+1][0] = tmp[0];
			}
		}
	}

	if (weight[0][1] != -1) {
		if (weight[0][1] == weight[1][1]) {
			if (weight[0][1] == weight[2][1]) {
				if (weight[0][1] == weight[3][1]) {
					numbermax = 4;
					direction[0] = weight[0][0];
					direction[1] = weight[1][0];
					direction[2] = weight[2][0];
					direction[3] = weight[3][0];
				} else {
					numbermax = 3;
					direction[0] = weight[0][0];
					direction[1] = weight[1][0];
					direction[2] = weight[2][0];
				}
			} else {
				numbermax = 2;
				direction[0] = weight[0][0];
				direction[1] = weight[1][0];
			}
		} else {
			numbermax = 1;
			direction[0] = weight[0][0];
		}
	} else {
		numbermax = 0;
		direction[0] = -1;
	}
	
	for (i=0; i<4; i++) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : --after Bubble sort current weight[" << i << "] = " << weight[i][1] << " go_direction : " << weight[i][0] << std::endl;
	}
	 *max_weight = weight[0][1];
	return numbermax;
}

int CAmazingPacManAi::get_next_max_weight(int* direction)
{
	int weight[4][2] = {0};
	int numbermax = 0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	return numbermax;
}

const char* CAmazingPacManAi::get_next_godirection()
{
	int move_back[2];
	int maxposition_number = 0;
	int maxposition[4] = {0};
	int previous_location[2] = {0};
	int current_position[2] = {0};
	int max_weight = 0;
	bool isdangerous = false;
	int avoid_direction = 0;
	int i = 0;
	int tmpmax_weight = 0;

	std::cout << "CAmazingPacManAi-> " << __func__ << " ---" << std::endl;
	set_four_current_position_states();
	set_four_next_position_states();
	maxposition_number = get_current_max_weight(maxposition, &max_weight);
	tmpmax_weight = next_position_states[maxposition[0]].around_weight;
	switch(maxposition_number){

		case 0:
		case 1:
			if (max_weight == 0 ) { //|| current_position_states[maxposition[0]].dangerous <= 2) {
				get_current_position(current_position);
				previous_location[0] = walked_position_list.position_list[walked_position_list.current_number-1][0];
				previous_location[1] = walked_position_list.position_list[walked_position_list.current_number-1][1];
				move_back[0] = previous_location[0] - current_position[0];
				move_back[1] = previous_location[1] - current_position[1];
				if (up[0] == move_back[0] && up[1] == move_back[1]) {
					maxposition[0] = go_up;
				}
				if (down[0] == move_back[0] && down[1] == move_back[1]) {
					maxposition[0] = go_down;
				}
				if (right[0] == move_back[0] && right[1] == move_back[1]) {
					maxposition[0] = go_right;
				}
				if (left[0] == move_back[0] && left[1] == move_back[1]) {
					maxposition[0] = go_left;
				}
			}
			break;

		case 2:
			for (i=0; i<2; i++) {
				if (tmpmax_weight < next_position_states[maxposition[i]].around_weight) {
					tmpmax_weight = next_position_states[maxposition[i]].around_weight;
					maxposition[0] = maxposition[i];
				}
			}
			break;
		case 3:
			for (i=0; i<3; i++) {
				if (tmpmax_weight < next_position_states[maxposition[i]].around_weight) {
					tmpmax_weight = next_position_states[maxposition[i]].around_weight;
					maxposition[0] = maxposition[i];
				}
			}
			break;
		case 4:
			for (i=0; i<4; i++) {
				if (tmpmax_weight < next_position_states[maxposition[i]].around_weight) {
					tmpmax_weight = next_position_states[maxposition[i]].around_weight;
					maxposition[0] = maxposition[i];
				}
			}
			break;
		default:
			break;
		
	}
	std::cout << "CAmazingPacManAi-> " << __func__ << " : maxposition_number : " << maxposition_number << std::endl;
	std::cout << "------action_list : " << walked_position_list.position_list[walked_position_list.current_number-1][0] << ","  << walked_position_list.position_list[walked_position_list.current_number-1][1] << std::endl;
//	std::cout << "      --------    GO_W size = " << sizeof(GO_W) << " , strlen(GO_W) = " << strlen(GO_W) << std::endl;
	//如果四周都是０那么进行回退
/*	if (max_weight == 0 ) { //|| current_position_states[maxposition[0]].dangerous <= 2) {
		get_current_position(current_position);
		previous_location[0] = walked_position_list.position_list[walked_position_list.current_number-1][0];
		previous_location[1] = walked_position_list.position_list[walked_position_list.current_number-1][1];
		move_back[0] = previous_location[0] - current_position[0];
		move_back[1] = previous_location[1] - current_position[1];
		if (up[0] == move_back[0] && up[1] == move_back[1]) {
			maxposition[0] = go_up;
		}
		if (down[0] == move_back[0] && down[1] == move_back[1]) {
			maxposition[0] = go_down;
		}
		if (right[0] == move_back[0] && right[1] == move_back[1]) {
			maxposition[0] = go_right;
		}
		if (left[0] == move_back[0] && left[1] == move_back[1]) {
			maxposition[0] = go_left;
		}
	}
*/
	//如果距离幽灵小于等于２格子，进行规避
	isdangerous = avoid_ghost(&avoid_direction);
	if (isdangerous) {
		std::cout << "CAmazingPacManAi-> " << __func__ << " : 幽灵小于3个格子，很危险马上规避" << std::endl;
		maxposition[0] =  avoid_direction;
	}		
	switch(maxposition[0])
	{
		case go_up:
			std::cout << "CAmazingPacManAi-> " << __func__ << " : Send command go_up--> " << GO_W << std::endl;
			return GO_W;
		case go_down:
			std::cout << "CAmazingPacManAi-> " << __func__ << " : Send command go_down--> " << GO_S << std::endl;
			return GO_S;
		case go_right:
			std::cout << "CAmazingPacManAi-> " << __func__ << " : Send command go_right--> " << GO_D << std::endl;
			return GO_D;
		case go_left:
			std::cout << "CAmazingPacManAi-> " << __func__ << " : Send command go_left--> " << GO_A << std::endl;
			return GO_A;
		case go_stop:
			std::cout << "CAmazingPacManAi-> " << __func__ << " : Send command donot_move--> " << DONOT_MOVE << std::endl;
		default:
			return DONOT_MOVE;
	}
}

void CAmazingPacManAi::show_map()
{
	int i,j;
	for (i=0; i<15; i++) {
		for(j=0; j<15; j++) {
			std::cout << m_server_map[15*i + j] << " ";
		}
		std::cout << std::endl;
	}
}

void CAmazingPacManAi::output_message(const char* outstr)
{
        std::cout << "CAmazingPacManAi-> " << outstr << std::endl;
}

