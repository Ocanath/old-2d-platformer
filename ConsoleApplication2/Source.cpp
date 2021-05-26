//#include <SDL.h> 

#include <iostream>
#include <string>
#include <fstream>

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2\opencv.hpp>



#include "GameState.h"
#include "display_game_functions.h"

#include <stdint.h>


using namespace std;
using namespace cv;






int main(int argc, char** argv)
{
	int game_is_won = 0;

	GameObject player;
	player.width = 42;
	player.height = 127;
	player.x = 50;
	player.y = 480/2-player.height/2;
	//player.x = 6076;
	//player.y = -2280;
	player.vx = 0;
	player.vy = 0;
	player.obj_num = 0;		//player number
	player.anim_step = 0;
	


	int allow_walljump = 0;
	int thrusters_count = 0;
	int attack_count;
	int player_dir;




	//another tracking possibilitys
	GameObject camera_drone;
	camera_drone.width = 40;
	camera_drone.height = 40;
	camera_drone.x = 50;
	camera_drone.y = 480 / 2 - player.height / 2;
	camera_drone.vx = 0;
	camera_drone.vy = 0;
	camera_drone.obj_num = 15;	//reserved droneball-type object!!
	camera_drone.anim_step = 200;	//reserved for droneball








	GameObject droneball;
	droneball.width = 40;
	droneball.height = 40; 
	droneball.x = 0;
	droneball.y = 20;
	droneball.vx = 0;
	droneball.vy = 0;
	droneball.obj_num = 15;	//reserved droneball-type object!!
	droneball.anim_step = 200;	//reserved for droneball

	Rope rope;
	rope.x = 0;
	rope.y = 150;
	rope.length = 0;	//assign when the rope is fired

	GameObject fake_player_for_droneball;
	fake_player_for_droneball.width = 42;
	fake_player_for_droneball.height = 127;
	fake_player_for_droneball.x = 0;
	fake_player_for_droneball.y = -150;
	fake_player_for_droneball.vx = 0;
	fake_player_for_droneball.vy = 0;


	GameObject obj_list[1500];
	int num_obstacles = 15;
	
	
	obj_list[0].width = 90 * 42;	//bottom floor
	obj_list[0].height = 10;
	obj_list[0].y = 480 + obj_list[0].height / 2;
	obj_list[0].x = 0 + obj_list[0].width / 2;
	obj_list[0].obj_num = 2;

	obj_list[1].width = 10;		//initial left wall
	obj_list[1].height = 8*127;
	obj_list[1].y = 480 - obj_list[1].height / 2;
	obj_list[1].x = 0 + obj_list[1].width / 2;
	obj_list[1].obj_num = 2;

	obj_list[2].width = 67 * 42;		//corridor ceiling
	obj_list[2].height = 10;
	obj_list[2].y = 127*2 + obj_list[2].height / 2;
	obj_list[2].x = 16*42 + obj_list[2].width / 2;
	obj_list[2].obj_num = 2;

	obj_list[3].width = 10;		//left corridor bouncy wall
	obj_list[3].height = 17*127+20;
	obj_list[3].y = 127 * 2 - obj_list[3].height / 2;
	obj_list[3].x = 16 * 42 + obj_list[2].width +obj_list[3].width / 2-10;
	obj_list[3].obj_num = 4;

	obj_list[4].width = 10;		//right corridor bouncy wall
	obj_list[4].height = 17*127;
	obj_list[4].y = 480 - obj_list[4].height / 2;
	obj_list[4].x = 90 * 42 - 5;
	obj_list[4].obj_num = 4;

	obj_list[5].width = 20*42;		//bouncy corridor ceiling
	obj_list[5].height = 10;
	obj_list[5].y = 480-obj_list[4].height+obj_list[5].height/2 - 127*2;
	obj_list[5].x = obj_list[3].x+obj_list[5].width/2;
	obj_list[5].obj_num = 2;

	obj_list[6].width = 90*42;		//bouncy corridor exit floor
	obj_list[6].height = 10;
	obj_list[6].y = 480 - obj_list[4].height + obj_list[6].height / 2;
	obj_list[6].x = obj_list[4].x + obj_list[6].width / 2;
	obj_list[6].obj_num = 2;

	obj_list[7].width = 10;			//left wall big chamber
	obj_list[7].height = 8 * 127;
	obj_list[7].x = obj_list[5].width / 2 + obj_list[5].x;
	obj_list[7].y = obj_list[5].y - obj_list[7].height/2;
	obj_list[7].obj_num = 2;

	obj_list[8].width = 10;			//big chamber right wall
	obj_list[8].height = 20 * 127;
	obj_list[8].x = obj_list[6].width / 2 + obj_list[6].x;
	obj_list[8].y = obj_list[6].y - obj_list[8].height / 2;
	obj_list[8].obj_num = 2;

	obj_list[9].width = 42*50;			//big chamber ceiling
	obj_list[9].height = 10;
	obj_list[9].x = obj_list[7].x + obj_list[9].width / 2;
	obj_list[9].y = obj_list[7].y - obj_list[7].height/2;
	obj_list[9].obj_num = 2;

	obj_list[10].width = 42 * 7;			//platform 1 for upgrade
	obj_list[10].height = 10;
	obj_list[10].x = 6677;
	obj_list[10].y = -1958;
	obj_list[10].obj_num = 2;


	obj_list[11].width = 42 * 7;			//platform
	obj_list[11].height = 10;
	obj_list[11].x = 6059;
	obj_list[11].y = -2212;
	obj_list[11].obj_num = 2;


	///////////////////////////////////////////////////////////////////////////////////////////////
	int has_thrusters_upgrade = 0;							//change to 0 to put the upgrade back
	int suppress_walljump_ability = 1;
	///////////////////////////////////////////////////////////////////////////////////////////////

	GameObject thrusters_upgrade;
	thrusters_upgrade.x = 6008;
	thrusters_upgrade.y = -2559;
	thrusters_upgrade.width = 30;
	thrusters_upgrade.height = 30;
	thrusters_upgrade.obj_num = 7;
	
	obj_list[12].width = 42 * 7;			//platform
	obj_list[12].height = 10;
	obj_list[12].x = 7529+30 - obj_list[12].width/2;
	obj_list[12].y = -2260;
	obj_list[12].obj_num = 2;

	obj_list[13].width = 10;			//platform
	obj_list[13].height = 15*127;
	obj_list[13].x = obj_list[7].x;
	obj_list[13].y = obj_list[7].y-obj_list[7].height/2 - obj_list[13].height/2;
	obj_list[13].obj_num = 2;

	obj_list[14].width = 96;			//platform
	obj_list[14].height = 40;
	obj_list[14].x = 4396;
	obj_list[14].y = obj_list[9].y-obj_list[9].height/2-obj_list[14].height/2;
	obj_list[14].obj_num = 2;
	
	GameObject endgoal;
	endgoal.width = 40;
	endgoal.height = 30;
	endgoal.x = obj_list[14].x;
	endgoal.y = obj_list[14].y - obj_list[14].height - 30 - endgoal.height / 2;
	endgoal.obj_num = 7;
	
	GameObject laser;
	laser.width = 0;
	laser.height = 5;
	laser.vx = 0;
	laser.vy = 0;
	laser.x = 0;
	laser.y = 0;
	laser.obj_num = 8;
	laser.anim_step = 0;




	
	
	//player.height /= 2;
	//player.width /= 2; 
	//player.x /= 2;
	//player.y /= 2;
	//thrusters_upgrade.x /= 2;
	//thrusters_upgrade.y /= 2;
	//thrusters_upgrade.width /= 2;
	//thrusters_upgrade.height /= 2;
	//endgoal.x /= 2;
	//endgoal.y /= 2;
	//endgoal.width /= 2;
	//endgoal.height /= 2;




	int camera_y_offset = 0;
	int camera_x_offset = 0;
	



	int contact = 1;

	int banish_droneball = 1;
	














	//UNNECESSARY FOR THE VERIloG GAME
	int zoom_multiplier = 1;
	Size frame_size = Size(640 * zoom_multiplier, 480 * zoom_multiplier);
	Mat background = Mat::zeros(frame_size, CV_8UC3);
	background = imread("Image_lib/background.jpg", CV_LOAD_IMAGE_COLOR);
	resize(background, background, frame_size, 0, 0, 1);
	Mat frame;
	frame = Mat::zeros(frame_size, CV_8UC3);


	init_player_frame_library();
	
	cout << "done" << endl;

		
	int key_buf = 0xf;


	//for (int i = 2; i < 71; i+=7)
	//print_processed_frame_to_text(160, 21);

	



	
	while (1)
	{
		
		int key_press = waitKey(1);
		key_buf = key_press;
		if (key_press == 'a')
			key_buf = 0x01;
		else if (key_press == 'd')
			key_buf = 0x02;
		else if (key_press == 'w')
			key_buf = 0x03;
		else if (key_press == 'q')
			key_buf = 0x0103;
		else if (key_press == 'e')
			key_buf = 0x0203;
		
		
		


		//printf("%x\n", key_buf);



		background.copyTo(frame);
		
		if (key_press == 'p')
			cout << "x = " << player.x << " y = " << player.y << "bottom y =" << player.y + player.height / 2 << endl;
		if (key_press == 'j')
			suppress_walljump_ability = !suppress_walljump_ability;


		if (key_press == 'b')
			banish_droneball = !banish_droneball;
		if (banish_droneball == 0)
		{
			update_droneball_position(&droneball, player, obj_list, num_obstacles);
			fake_player_for_droneball.x = player.x;
			fake_player_for_droneball.y = player.y;
		}
		else if (banish_droneball == 1)
			update_droneball_position(&droneball, fake_player_for_droneball, obj_list, num_obstacles);



		

		/*
		if (key_press == 'r')
		put_player_on_rope = !put_player_on_rope;
		if (put_player_on_rope == 1)
		{
		if (have_re_init_player_pos_and_v == 0)
		{
		player.x = -30;
		player.y = 300;
		player.vx = 0;
		player.vy = 0;
		have_re_init_player_pos_and_v = 1;
		}
		assign_player_rope_velocity(&player, &rope, 0.35);
		}
		else if (put_player_on_rope == 0)
		have_re_init_player_pos_and_v = 0;
		*/

		//		else if (debug_control_counter > 145 && debug_control_counter < 160)
		//			key_press = 0x0301;


		update_moving_platform_position(obj_list, 0, 480 - 50, 0, 480 - 250, 0, 0);


		update_player_position(obj_list, num_obstacles, &player, key_buf, &contact, &thrusters_count, &attack_count, &player_dir, &allow_walljump, suppress_walljump_ability);
		
		//GameObject tmp;
		//update_player_laser_state(1, &laser, player, player_dir, obj_list, num_obstacles, &tmp);
		



		if (contact_type(player, thrusters_upgrade) != 0)
		{
			thrusters_count = 1;
			has_thrusters_upgrade = 1;
		}
		if (has_thrusters_upgrade && contact == 1)
			thrusters_count = 1;
		




		if (game_is_won == 0)
		{

			update_droneball_position(&camera_drone, player, obj_list, 0);		//this has an interesting feel to it! i might keep
			camera_x_offset = 640 / 2 * zoom_multiplier - camera_drone.x;
			camera_y_offset = (480 * zoom_multiplier - player.height*2 - 10) - camera_drone.y;
			


		//	camera_x_offset = 640 / 2 * zoom_multiplier - player.x;
		//	camera_y_offset = (480 * zoom_multiplier - player.height - 10) - player.y;			//change for better feel... need thresholds		HHHEEEYYY GET RID OF THIS 4000 NUMBER WHEN YOU REMOVE THE ZOOOM OUT
		
		//	camera_x_offset = 640/2;
		//	camera_y_offset = 480-player.height-10;

		}
		else
		{
			camera_x_offset = 640 / 2 *zoom_multiplier - (4465);
			camera_y_offset = (480 * zoom_multiplier - player.height - 10) - (-3012);
		}
		if (key_press == 'r')
			game_is_won = 0;
		//			camera_x_offset = 640 / 2 * 8 / 5 * zoom_multiplier - (4465);
		//			camera_y_offset = (480 * 8 / 5 * zoom_multiplier - player.height - 10) - (-3012);

		//		if (player.x > camera_right_border)
		//			camera_x_offset = camera_right_border - player.x;	
		//		else if (player.x < 640 / 4)
		//			camera_x_offset = 640 / 4 - player.x;


		//cout<< hex << "cx = " << camera_x_offset << ", cy = " << hex << camera_y_offset << endl;
		


		//draw_obj(frame, laser, camera_x_offset, camera_y_offset);

		for (int i = 0; i < num_obstacles; i++)
			draw_obj(frame, obj_list[i], camera_x_offset, camera_y_offset);
		draw_obj(frame, droneball, camera_x_offset, camera_y_offset);
		if (has_thrusters_upgrade == 0)
			draw_obj(frame, thrusters_upgrade, camera_x_offset, camera_y_offset);

		if (contact_type(player, endgoal) != 0)
			game_is_won = 1;

		draw_obj(frame, endgoal, camera_x_offset, camera_y_offset);

		//draw_rope(frame, player, rope, camera_x_offset);
		draw_obj(frame, player, camera_x_offset, camera_y_offset);

		if (zoom_multiplier > 1)
			resize(frame, frame, Size(896, 672), 0, 0, 1);		//for zooming out purposes
		imshow("M80", frame);


	}


}



/*

string x_base_name = "logic [15:0] obj_x_pos_";
string y_base_name = "logic [15:0] obj_y_pos_";
string width_base_name = "logic [15:0] obj_width_";
string height_base_name = "logic [15:0] obj_height_";
string vlog_dec_format_base_name = " = 16'h";

ofstream myfile;
myfile.open("init_sysvlg.txt");
myfile.clear();
for (int i = 0; i < num_obstacles; i++)
{
unsigned int num_to_op;
string tmp_op;

tmp_op = x_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].x - obj_list[i].width / 2;
myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; ";

tmp_op = y_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].y - obj_list[i].height / 2;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; " << endl;

tmp_op = width_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].width;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; ";

tmp_op = height_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].height;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; " << endl;
}
myfile.close();

*/








//code for slidey camera
/*
//THESE GO OUTSIDE FOR LOOP
//use for sliding window??
int camera_right_border = 640 / 2 + 640 / 4;
int camera_left_border = 640 / 4;
int camera_up_border = 200 - player.height - 10;
int camera_down_border = (480 - player.height - 10);
int player_right_push = 640/2;
int player_left_push = 640 / 2;

int player_down_push = (480 - player.height - 10);
int player_up_push = (480 - player.height - 10);

//END OF OUSTIDE FOR LOOP



//update the cam offset in the for loop
if (player.x > player_right_push)
{
player_right_push = player.x;
player_left_push = player.x - 640 / 2;
}
else if (player.x < player_left_push)
{
player_left_push = player.x;
player_right_push = player.x + 640 / 2;
}

if (player.y > player_down_push)
{
player_down_push = player.y;
player_up_push = player.y - 280;
}
else if (player.y < player_up_push)
{
player_up_push = player.y;
player_down_push = player.y + 280;
}

if (player_right_push > camera_right_border)
{
camera_x_offset = camera_right_border - player_right_push;
}
else if (player_left_push < camera_left_border)
{
camera_x_offset = camera_left_border - player_left_push;
}

if (player_down_push > camera_down_border)
{
camera_y_offset = camera_down_border - player_down_push;
}
else if (player_up_push < camera_up_border)
{
camera_y_offset = camera_up_border - player_up_push;
}











string x_base_name = "logic [15:0] obj_x_pos_";
string y_base_name = "logic [15:0] obj_y_pos_";
string width_base_name = "logic [15:0] obj_width_";
string height_base_name = "logic [15:0] obj_height_";
string vlog_dec_format_base_name = " = 16'h";

ofstream myfile;
myfile.open("init_sysvlg.txt");
myfile.clear();
for (int i = 0; i < num_obstacles; i++)
{
unsigned int num_to_op;
string tmp_op;

tmp_op = x_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].x - obj_list[i].width / 2;
myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; ";

tmp_op = y_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].y - obj_list[i].height / 2;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; " << endl;

tmp_op = width_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].width;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; ";

tmp_op = height_base_name;
tmp_op.append(to_string(i));
tmp_op.append(vlog_dec_format_base_name);
num_to_op = obj_list[i].height;

myfile << tmp_op;
//printf("%x", num_to_op);
myfile << hex << num_to_op;
myfile << "; " << endl;
}
myfile.close();


*/