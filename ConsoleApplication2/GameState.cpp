#include "GameState.h"
#include <math.h>



void assign_contact_velocity(GameObject * player, GameObject platform, int contact_type)
{
	int obj_type = platform.obj_num;

	if (obj_type == 2 || obj_type == 0)
	{
		if (contact_type == 1 || contact_type == 2)		//type 1 = touch the left wall with your right side
			player->vx = 0;
		else if (contact_type == 3 || contact_type == 4)	//type 3 = touch the top wall with your feet
			player->vy = 0;
	}
	else if (obj_type == 3)
	{
		if (contact_type == 1)	{	//type 1 = touch the left wall with your right side
			player->vx = -1.0*player->vx + platform.vx;
			player->vy = player->vy + .5*platform.vy;
		}
		else if (contact_type == 2){	//type 2 = touch the right wall with your left side
			player->vx = -1.0*player->vx + platform.vx;
			player->vy = player->vy + .5*platform.vy;
		}
		else if (contact_type == 3){	//type 3 = touch the top wall with your feet
			player->vy = -1.2*player->vy + platform.vy;
			player->vx = player->vx + 0.1*platform.vx;
		}
		else if (contact_type == 4){	//type 4 = touch the bottom wall with your head
			player->vy = -1.0*player->vy + platform.vy;
			player->vx = player->vx + 0.1*platform.vx;
		}
	}

	else if (obj_type == 4)
	{
		if (player->obj_num == 0)
		{
			if (contact_type == 1)
			{		//type 1 = touch the left wall with your right side
				player->vx = -1.0*player->vx;
				if (player->vy < -4.4)
					player->vy -= 9;
			}
			else if (contact_type == 2)	//type 2 = touch the right wall with your left side
			{
				player->vx = -1.0*player->vx;
				if (player->vy < -4.4)
					player->vy -= 9;
			}
			else if (contact_type == 3)	//type 3 = touch the top wall with your feet
				player->vy = -1.0*player->vy;
			else if (contact_type == 4)	//type 4 = touch the bottom wall with your head
				player->vy = -1.0*player->vy;
		}
		else
		{
			if (contact_type == 1 || contact_type == 2)		//type 1 = touch the left wall with your right side
				player->vx = 0;
			else if (contact_type == 3 || contact_type == 4)	//type 3 = touch the top wall with your feet
				player->vy = 0;
		}
	}

}

void apply_friction(GameObject * player, double friction_val)
{
	if (player->vx > 0)
		player->vx -= friction_val;
	if (player->vx < 0)
		player->vx += friction_val;
	if (player->vx > -1.0*friction_val && player->vx < friction_val)
		player->vx = 0;
}


int contact_type(GameObject moving_obj, GameObject  env_obj)
{
	
	int p_right_border = moving_obj.x + moving_obj.width / 2;				//load the values of the borders of the moving_obj
	int p_left_border = moving_obj.x - moving_obj.width / 2;
	int p_bottom_border = moving_obj.y + moving_obj.height / 2;
	int p_top_border = moving_obj.y - moving_obj.height / 2;

	int obj_right_border = env_obj.x + env_obj.width / 2;
	int obj_left_border = env_obj.x - env_obj.width / 2;
	int obj_bottom_border = env_obj.y + env_obj.height / 2;
	int obj_top_border = env_obj.y - env_obj.height / 2;
	
	if (p_right_border + moving_obj.vx >= obj_left_border && !(p_right_border > obj_right_border && p_left_border < obj_left_border) && p_left_border < obj_left_border && p_top_border < obj_bottom_border && p_bottom_border - 1 > obj_top_border)	//collision with obj left border 
		return 1;		//right border touching the left obj border
	if (p_left_border + moving_obj.vx <= obj_right_border  && !(p_right_border > obj_right_border && p_left_border < obj_left_border) && p_right_border > obj_right_border && p_top_border < obj_bottom_border && p_bottom_border - 1 > obj_top_border)//collision with obj right border
		return 2;		//left object border touching the 
	if (p_bottom_border + moving_obj.vy >= obj_top_border  && !(p_top_border < obj_top_border && p_bottom_border > obj_bottom_border) && p_top_border < obj_top_border && p_left_border < obj_right_border && p_right_border > obj_left_border)// && (p_bottom_border <= obj_top_border)  && (p_top_border >= obj_bottom_border) )
		return 3;	//feet
	if (p_top_border + moving_obj.vy <= obj_bottom_border  && !(p_top_border < obj_top_border && p_bottom_border > obj_bottom_border) && p_bottom_border > obj_bottom_border && p_left_border < obj_right_border && p_right_border > obj_left_border)
		return 4; //head
	return 0;
}


void handle_collision(GameObject * moving_obj, GameObject  env_obj)
{
	int p_right_border = moving_obj->x + moving_obj->width / 2;				//load the values of the borders of the moving_obj
	int p_left_border = moving_obj->x - moving_obj->width / 2;
	int p_bottom_border = moving_obj->y + moving_obj->height / 2;
	int p_top_border = moving_obj->y - moving_obj->height / 2;

	int obj_right_border = env_obj.x + env_obj.width / 2;
	int obj_left_border = env_obj.x - env_obj.width / 2;
	int obj_bottom_border = env_obj.y + env_obj.height / 2;
	int obj_top_border = env_obj.y - env_obj.height / 2;


	if (p_right_border + moving_obj->vx > obj_left_border && !(p_right_border > obj_right_border && p_left_border < obj_left_border) && p_left_border < obj_left_border && p_top_border < obj_bottom_border && p_bottom_border - 1 > obj_top_border)	//collision with obj left border 
	{
		moving_obj->x = obj_left_border - moving_obj->width / 2;

		p_right_border = moving_obj->x + moving_obj->width / 2;				//load the values of the borders of the moving_obj
		p_left_border = moving_obj->x - moving_obj->width / 2;

		assign_contact_velocity(moving_obj, env_obj, 1);

	}
	if (p_left_border + moving_obj->vx < obj_right_border  && !(p_right_border > obj_right_border && p_left_border < obj_left_border) && p_right_border > obj_right_border && p_top_border < obj_bottom_border && p_bottom_border - 1 > obj_top_border)//collision with obj right border
	{
		moving_obj->x = obj_right_border + moving_obj->width / 2;

		p_right_border = moving_obj->x + moving_obj->width / 2;				//load the values of the borders of the moving_obj
		p_left_border = moving_obj->x - moving_obj->width / 2;

		assign_contact_velocity(moving_obj, env_obj, 2);
	}

	if (p_bottom_border + moving_obj->vy > obj_top_border  && !(p_top_border < obj_top_border && p_bottom_border > obj_bottom_border) && p_top_border < obj_top_border && p_left_border < obj_right_border && p_right_border > obj_left_border)// && (p_bottom_border <= obj_top_border)  && (p_top_border >= obj_bottom_border) )
	{
		moving_obj->y = obj_top_border - moving_obj->height / 2;

		p_bottom_border = moving_obj->y + moving_obj->height / 2;
		p_top_border = moving_obj->y - moving_obj->height / 2;

		assign_contact_velocity(moving_obj, env_obj, 3);

	}
	if (p_top_border + moving_obj->vy < obj_bottom_border  && !(p_top_border < obj_top_border && p_bottom_border > obj_bottom_border) && p_bottom_border > obj_bottom_border && p_left_border < obj_right_border && p_right_border > obj_left_border)
	{

		moving_obj->y = obj_bottom_border + moving_obj->height / 2;

		p_bottom_border = moving_obj->y + moving_obj->height / 2;
		p_top_border = moving_obj->y - moving_obj->height / 2;

		assign_contact_velocity(moving_obj, env_obj, 4);

	}

}



void update_player_laser_state(int FIRE, GameObject * laser, GameObject player, int player_dir, GameObject * barriers, int num_barriers, GameObject * enemies)
{
	if (FIRE)
	{
		laser->width = 10;

		if (player_dir == 0)
			laser->vx = 20.0;
		else if (player_dir == 1)
			laser->vx = -20.0;

		laser->x += laser->vx;
		laser->y += laser->vy;
		
		for (int i = 0; i < num_barriers; i++)
		{
			handle_collision(laser, barriers[i]);
			if (contact_type(*laser, barriers[i]) == 1 || contact_type(*laser, barriers[i]) == 2)
			{
				laser->width = 0;
				laser->x = player.x;
				laser->y = player.y;
			}
		}
	}
}




void update_player_animation_step(GameObject * player, int foot_contact, int dir, int used_thrusters)
{
	//|| player->vx <= -0.1

	if ((player->vx >= 0.1) && foot_contact == 1)	//moving left or right, 3 frame animation, starting location 1
	{
		int starting_location = 2; int ending_location = 36;

		player->anim_step++;

		if (player->anim_step > ending_location || player->anim_step < starting_location)
			player->anim_step = starting_location;



	}
	else if ((player->vx <= -0.1) && foot_contact == 1)	//moving left or right, 3 frame animation, starting location 1
	{
		int starting_location = 37; int ending_location = 71;

		player->anim_step++;

		if (player->anim_step > ending_location || player->anim_step < starting_location)
			player->anim_step = starting_location;

	}



	else if (foot_contact == 0 && player->vx >= 1.0 && player->vy <= -1.0)   //jumping up and to the right
	{
		if (used_thrusters == 0)
			player->anim_step = 151;
		else
			player->anim_step = 157;

	}
	else if (foot_contact == 0 && player->vx <= -1.0 && player->vy <= -1.0)	//jumping up and to the left
	{
		if (used_thrusters == 0)
			player->anim_step = 152;
		else
			player->anim_step = 158;
	}
	
	else if (foot_contact == 0 && dir == 0 && player->vy >= 1.0)	//falling down and to the right
		player->anim_step = 153;
	else if (foot_contact == 0 && dir == 1 && player->vy >= 1.0)	//falling down and to the left
		player->anim_step = 154;

	else if (foot_contact == 0 && player->vy <= -0.1)	//straight up
	{
		if (used_thrusters == 0)
		{
			if (dir == 0)
				player->anim_step = 155;
			else
				player->anim_step = 156;
		}
		else
		{
			if (dir == 0)
				player->anim_step = 159;
			else
				player->anim_step = 160;
		}

	}

	//else		//if ((int)player->vx == 0 && (int)player->vy == 0)	//resting, single frame animation.
	//	player->anim_step = 0;

	else if (dir == 1)
		player->anim_step = 1;	//facing to the left
	else if (dir == 0)
		player->anim_step = 0;		//facing to the right
}







//may want to break this into 2 separate functions, velocity update and collision handling
void update_player_position(GameObject * obj_list, int num_obj, GameObject * player, int key_press, int * foot_contact, int * thrusters_count, int * attack_count, int * player_dir, int * allow_walljump, int suppress_walljump_ability)			//maybe change key press to buf?
{

	//bad opencv player control bloc begin




	int wall_touch = 0;
	*foot_contact = 0;
	for (int i = 0; i < num_obj; i++)
	{

		int type_of_contact = contact_type(*player, obj_list[i]);
		if (type_of_contact == 1)
			wall_touch = type_of_contact;		//one means grabbin the left
		else if (type_of_contact == 2)
			wall_touch = type_of_contact;		//two means grabbing the right
		//if (wall_touch != 0 && obj_list[i].height < player->height / 4)
		//	wall_touch = 0;

		if (type_of_contact == 3)
		{
			*foot_contact = 1;
			if (!((key_press & 0x00ff) == 0x0001 || (key_press & 0xff00) == 0x0100) && !((key_press & 0x00ff) == 0x0002 || (key_press & 0xff00) == 0x0200))		//if the feet touch a surface, apply FRICTION
			{
				if (obj_list[i].obj_num == 2)
					apply_friction(player, 0.3);
				else if (obj_list[i].obj_num != 2)
					apply_friction(player, .1);
			}
		}
	
		handle_collision(player, obj_list[i]);

	}
	if (!suppress_walljump_ability)
	{
		if (wall_touch != 1 && wall_touch != 2 || *foot_contact == 1)
			*allow_walljump = 1;

		if (*foot_contact == 0 && wall_touch == 1 && (key_press & 0x00ff) == 0x0002 || (key_press & 0xff00) == 0x0200 && *allow_walljump == 1)
			player->vy = 0;		//and add animation step?
		if (*foot_contact == 0 && wall_touch == 2 && (key_press & 0x00ff) == 0x0001 || (key_press & 0xff00) == 0x0100 && *allow_walljump == 1)
			player->vy = 0;
	}
	else 
		*allow_walljump = 0;
	
	
	int used_thusters = 0;
	
	if (((key_press & 0x00ff) == 0x0003 || (key_press & 0xff00) == 0x0300) && *foot_contact == 1)
		player->vy -= 15;
	else if (((key_press & 0x00ff) == 0x0003 || (key_press & 0xff00) == 0x0300) && wall_touch != 0 && *allow_walljump == 1)
	{
		
		if (*foot_contact == 0)
			*allow_walljump = 0;
		if (suppress_walljump_ability == 0)
			player->vy -= 15;
	}
	else if ( ((key_press & 0x00ff) == 0x0003 || (key_press & 0xff00) == 0x0300) && *thrusters_count > 0)
	{
		used_thusters = 1;
		player->vy -= 15;
		*thrusters_count = *thrusters_count - 1;
	}
	
	update_player_animation_step(player, *foot_contact, *player_dir, used_thusters);				//IMPORTANT. THIS IS NECESSARY TO ANIMATE THE CHARACTER
	
	
	if ((key_press & 0x00ff) == 0x0001 || (key_press & 0xff00) == 0x0100)
	{
		if (wall_touch != 2)
			player->vx--;
		*player_dir = 1;
	}
	if ((key_press & 0x00ff) == 0x0002 || (key_press & 0xff00) == 0x0200)
	{
		if (wall_touch != 1)
			player->vx++;
		*player_dir = 0;
	}
	if (player->vx < -15 && ((key_press & 0x00ff) == 0x0001 || (key_press & 0xff00) == 0x0100))
		player->vx = -15;
	else if (player->vx > 15 && ((key_press & 0x00ff) == 0x0002 || (key_press & 0xff00) == 0x0200))
		player->vx = 15;

	//test rt/better keyboard input bloc end
	
	

	if (player->vy < -30)  //cap upward velocity
		player->vy = -30;
	else if (player->vy > 30)
		player->vy = 30;

	//GRAVITY!!!! IT'S ONLY ONE LINE easy to disable! 
	player->vy += 0.35;



	if (player->vx >= .1 || player->vx <= -.1)
		player->x += (player->vx);
	player->y += (player->vy);



}



void update_moving_platform_position(GameObject * platform, int xmax, int ymax, int xmin, int ymin, float ax, float ay)
{

	if (platform->x > xmax || platform->x < xmin)
		platform->vx = -1 * platform->vx;
	if (platform->y > ymax || platform->y < ymin)
		platform->vy = -1 * platform->vy;


	platform->x += platform->vx;
	platform->y += platform->vy;

}

void update_ball_position(GameObject ball, GameObject player)
{

}




void update_droneball_position(GameObject * droneball, GameObject player, GameObject * obj_list, int num_obj)
{
	double theta;
	double vmag;
	theta = atan2(long double(droneball->y - (player.y - player.height/1)), long double(droneball->x - player.x));
	vmag = sqrt(double((droneball->x - player.x)*(droneball->x - player.x) + (droneball->y - (player.y - player.height/1))*(droneball->y - (player.y - player.height/1))));

	double max_v_thresh = 40 * sqrt(2.0);
	if (vmag > max_v_thresh)
		vmag = max_v_thresh;


	droneball->vx = -0.3*(vmag)*cos(theta);
	droneball->vy = -0.3*(vmag)*sin(theta);

	//droneball->vy += .35;


	
	int tmp = 0;
	handle_collision(droneball, player);
	for (int i = 0; i < num_obj; i++)
		handle_collision(droneball, obj_list[i]);
	
	//note that tmp is not properly handling contact now.

	if (droneball->vx >= .1 || droneball->vx <= -.1)
		droneball->x += (droneball->vx);
	droneball->y += (droneball->vy);


}


void assign_player_rope_velocity(GameObject * player, Rope * rope, double gravity)
{
	double theta = atan2(long double(player->y - rope->y), long double(player->x - rope->x));		//first thing to try is switching this order if something doesn't work (r - p vs p - r)

	theta = theta + 3.14159265359;

	//gravity is a positive downward pointing vector, which updates the player player velocity by (currently) +.35 pixels per frame, every frame
	double mag = (gravity*-1.0) / sin(theta);

	//add exceptions to this rule whenever the distance between the player and the rope tether point is less than the rope length

	player->vx += mag*cos(theta);
	player->vy += mag*sin(theta);



}














/*

else if (obj_type == 3)
{
if (contact_type == 0)		//type 0 = touch the left wall with your right side
player->vx = -1.0*player->vx;
else if (contact_type == 1)	//type 1 = touch the right wall with your left side
player->vx = -1.0*player->vx;
else if (contact_type == 2)	//type 2 = touch the top wall with your feet
player->vy = -1.0*player->vy;
else if (contact_type == 3)	//type 3 = touch the bottom wall with your head
player->vy = -1.0*player->vy;

}
*/