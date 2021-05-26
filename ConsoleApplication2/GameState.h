#ifndef GAMESTATE_H
#define GAMESTATE_H



typedef struct 
{
	int x;
	int y;
	int width;
	int height;
	int obj_num;

	float vx;
	float vy;

	unsigned int anim_step;

}GameObject;


typedef struct
{
	int x;
	int y;
	double length;
}Rope;


void update_player_laser_state(int FIRE, GameObject * laser, GameObject player, int player_dir, GameObject * barriers, int num_barriers, GameObject * enemies);


void assign_contact_velocity(GameObject * player, GameObject platform, int contact_type);
void update_player_animation_step(GameObject * player, int contact, int dir, int used_thrusters);
void update_player_position(GameObject * obj_list, int num_obj, GameObject * player, int key_press, int * foot_contact, int * thrusters_count, int * attack_count, int * player_dir, int * allow_walljump, int suppress_walljump_ability);			//maybe change key press to buf?
void update_moving_platform_position(GameObject * platform, int xmax, int ymax, int xmin, int ymin, float ax, float ay);
void update_ball_position(GameObject ball, GameObject player);
void update_droneball_position(GameObject * droneball, GameObject player, GameObject * obj_list, int num_obj);
void assign_player_rope_velocity(GameObject * player, Rope * rope, double gravity);
void handle_collision(GameObject * moving_obj, GameObject  env_obj);
int contact_type(GameObject moving_obj, GameObject  env_obj);


#endif //GAMESTATE_H