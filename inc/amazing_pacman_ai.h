//amazing_pacman_ai.h
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>

//-------------------------------------------------------------------
// The four direction for server command
//-------------------------------------------------------------------
#define GO_W "[w]"
#define GO_S "[s]"
#define GO_A "[a]"
#define GO_D "[d]"
#define DONOT_MOVE "[n]"

/** This structure describes the current position states
 *  dangerous : This parameter tells you Distance from ghost
 *  iswalk : This parameter tells you whether this position can walk
 *  weight : This parameter tells you this position score weight
 */
typedef struct {
	int dangerous[3]; 
	bool iswalk;
	int weight;
}CURRENT_POSITION_STATES;

/** This structure describes the current position states
 *  dangerous : This parameter tells you Distance from ghost
 *  around_weight : This parameter tells you around this position
 *  score weight
 */
typedef struct {
	int dangerous;
	int around_weight;
}NEXT_POSITION_STATES;

/** This structure describes a previous move action list
 *  position_list : This parameter tells you walked position list
 *  current_number : This parameter tells where we are new in list
 */
typedef struct {
	int position_list[400][2];
	int current_number;
}WALKED_POSITION_LIST;

//-------------------------------------------------------------------
// Just define four direction numbers
//-------------------------------------------------------------------
enum FourDirection {
	go_up = 0,
	go_down = 1,
	go_right = 2,
	go_left = 3,
	go_stop = 4
};

/** This const parmeters describes the operations to be performed in
 *  four directions, if you want to go up, just add it.
 */
static int up[2] = {-1,0};
static int down[2] = {1,0};
static int right[2] = {0,1};
static int left[2] = {0,-1};
static int stop[2] = {0,0};

static CURRENT_POSITION_STATES current_position_states[4];
static NEXT_POSITION_STATES next_position_states[4]; 
static WALKED_POSITION_LIST walked_position_list;
/**
 * @CAmazingPacManAi : Judge pacman next walking plan
 * Function:
 * set_server_map() : Just set new server map
 * set_four_current_position_states() : Just set the pacman four
direction states weight
 * set_four_next_position_states() : Just set the pacman four
direction average states weight
 * get_position_weight() : Just get current position position
 * get_position_number() : Just get converted map location
 * get_next_godirection() : Just get next walking direction
 * move_position() : Just move pacmanai to four direction one step
 * is_direction_move : judge whether this position can walk
 * show_map() : Just show the map
 *
 * Parameters:
 * m_server_map[300] : This is server map
 * m_current_position[2] : This is pacman current position
 */
class CAmazingPacManAi
{
public:
	CAmazingPacManAi();
	~CAmazingPacManAi();
	void set_server_map(const char* servermap);
	void set_four_current_position_states();
	void set_four_next_position_states();
	void set_current_position(int* position);
	void set_three_ghost_position();

	void get_dangerous_from_ghost(int direction, int* dangerous);
	void get_three_ghost_position(int position[][2]);
	void get_current_position(int* position);
	int get_position_weight(int position_number);
	int get_position_number(int direction);
	int get_next_max_weight(int* direction);
	int get_current_max_weight(int* direction, int* max_weight);
	const char* get_next_godirection();	

	bool avoid_ghost(int* avoid_directions);
	bool add_one_walked_position_tolist(int* position);
	bool remove_one_walked_position_fromlist();
	void move_position(int direction);
	bool is_direction_move(int direction);
	void show_map();
	void output_message(const char* outstr);	
private:
	
	char m_server_map[300];
	int m_current_position[2];
	int m_ghost_position[3][2];
};
