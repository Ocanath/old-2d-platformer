#include "display_game_functions.h"
#include <iostream>
#include <fstream>


#include <bitset>


int frame_library[250][37044];			//change all 37044 to c-style dyanmic memory allocation. This will be:    player.width*player.height * 3
int pic_width = 98;			//96 originally
int pic_height = 126;		//126 originally


void print_processed_frame_to_text(int frame_addr, int print_addr)
{
	ofstream myfile;
	myfile.open("single_test_frame.txt");
	myfile.clear();
	
	myfile << endl << endl << "-- frame " << print_addr << endl << endl;

	for (int i = 0; i < 37044; i+=3)
	{
		bitset<8> tmp(201);
		
		int frame_lib_val = frame_library[frame_addr][i];
		if (frame_lib_val == 201)
			frame_lib_val = 200;

		bitset<8> op(frame_lib_val);
		if (frame_lib_val != -1)
		{
			myfile << hex << i / 3 + print_addr * 12348 << " : " << op << ";" << endl;
		}
		else
			myfile << hex << i / 3 + print_addr * 12348 << " : " << tmp << ";" << endl;

	}
	myfile.close();
}

void crop_image_helper(string filename_in, string filename_out, int left_bord, int right_bord, int top_bord, int bott_bord)
{
	Mat fil = imread(filename_in, 1);

	Mat outfil = Mat::zeros(Size( (fil.cols-right_bord) - left_bord + 1, (fil.rows-bott_bord) - top_bord + 1), CV_8UC3);

	for (int y = top_bord; y < fil.rows - bott_bord; y++)
	{
		for (int x = left_bord; x < fil.cols - right_bord; x++)
		{
			outfil.data[outfil.step[0] * (y - top_bord) + outfil.step[1] * (x - left_bord) + 0] = fil.data[fil.step[0] * (y)+fil.step[1] * (x)+0];		//
			outfil.data[outfil.step[0] * (y - top_bord) + outfil.step[1] * (x - left_bord) + 1] = fil.data[fil.step[0] * (y)+fil.step[1] * (x)+1];		//
			outfil.data[outfil.step[0] * (y - top_bord) + outfil.step[1] * (x - left_bord) + 2] = fil.data[fil.step[0] * (y)+fil.step[1] * (x)+2];		//
			
		}
	}
	imwrite(filename_out, outfil);
	
}


void load_file_player_frame_lib(string filename, int obj_type, int bin_thresh, int address)
{
	// "Image_lib / LITTL_DUDE.jpg"


	Mat f1 = imread(filename, 1);
	resize(f1, f1, Size(pic_width, pic_height), 0, 0, 1);



	for (int y = 0; y < pic_height; y++)		//player height = 24
	{
		for (int x = 0; x < pic_width; x++)	//player width = 12
		{
			
			if ((f1.data[f1.step[0] * (y)+f1.step[1] * (x)+0] < 50 && f1.data[f1.step[0] * (y)+f1.step[1] * (x)+1] > 170 && f1.data[f1.step[0] * (y)+f1.step[1] * (x)+2] < 50))
			{
				frame_library[address][y*pic_width * 3 + x * 3 + 0] = -1;
				frame_library[address][y*pic_width * 3 + x * 3 + 1] = -1;
				frame_library[address][y*pic_width * 3 + x * 3 + 2] = -1;
			}

			else
			{
				if (obj_type == 0)
				{
					int avg_grey = f1.data[f1.step[0] * (y)+f1.step[1] * (x)+0] + f1.data[f1.step[0] * (y)+f1.step[1] * (x)+1] + f1.data[f1.step[0] * (y)+f1.step[1] * (x)+2];
					avg_grey = avg_grey / 3;
					if (avg_grey < 200)
						avg_grey -= 70;
					if (avg_grey < 0)
						avg_grey = 0;

					frame_library[address][y*pic_width * 3 + x * 3 + 0] = avg_grey;
					frame_library[address][y*pic_width * 3 + x * 3 + 1] = avg_grey;
					frame_library[address][y*pic_width * 3 + x * 3 + 2] = avg_grey;
				}

				else if (obj_type == 15)
				{
					frame_library[address][y*pic_width * 3 + x * 3 + 0] = f1.data[f1.step[0] * (y)+f1.step[1] * (x)+0];
					frame_library[address][y*pic_width * 3 + x * 3 + 1] = f1.data[f1.step[0] * (y)+f1.step[1] * (x)+1];
					frame_library[address][y*pic_width * 3 + x * 3 + 2] = f1.data[f1.step[0] * (y)+f1.step[1] * (x)+2];
				}
			}
					

		}
	}
}








//this is gonna be in hardware
void init_player_frame_library()
{

	cout << "initializing game animation" << endl;
	
	/*
	standing still
	*/
	int bin_thresh = 200;
	//"Image_lib / LITTL_DUDE.jpg"

	load_file_player_frame_lib("Image_lib/Still_R.bmp", 0, bin_thresh, 0);
	
	
	load_file_player_frame_lib("Image_lib/Still_L.bmp", 0, bin_thresh, 1);
	

	int repeated_frames = 7;


	
	string fname_base = "Image_lib/RunR_";
	int base_address = 2;	//starts at this address, goes until the end val of the for loop + baseaddress - 1
	for (int i = 1; i <= 5; i++)
	{
		string fnam = fname_base;
		fnam.append(to_string(i));
		fnam.append(".bmp");
		for (int address = i * repeated_frames - repeated_frames + base_address; address < i * repeated_frames + base_address; address++)
		{
			cout << fnam << ", addr: "<< address << endl;
			load_file_player_frame_lib(fnam, 0, bin_thresh, address);
		}

	}
	

	fname_base = "Image_lib/RunL_";
	base_address = 5*repeated_frames + 2;	//starts at this address, goes until the end val of the for loop + baseaddress - 1
	for (int i = 1; i <= 5; i++)
	{
		string fnam = fname_base;
		fnam.append(to_string(i));
		fnam.append(".bmp");
		for (int address = i * repeated_frames - repeated_frames + base_address; address < i * repeated_frames + base_address; address++)
		{
			cout << fnam << ", addr: " << address << endl;
			load_file_player_frame_lib(fnam, 0, bin_thresh, address);
		}
	}






	//	load_file_player_frame_lib("Image_lib/RUNNIN_RIGHT_1.jpg", bin_thresh, player, 1);
	//	load_file_player_frame_lib("Image_lib/RUNNIN_RIGHT_2.jpg", bin_thresh, player, 2);
	//	load_file_player_frame_lib("Image_lib/RUNNIN_LEFT_1.jpg", bin_thresh, player, 3);
	//	load_file_player_frame_lib("Image_lib/RUNNIN_LEFT_2.jpg", bin_thresh, player, 4);


	load_file_player_frame_lib("Image_lib/Jump_R.bmp", 0, bin_thresh, 151);
	load_file_player_frame_lib("Image_lib/Jump_L.bmp", 0, bin_thresh, 152);
	load_file_player_frame_lib("Image_lib/Fall_R.bmp", 0, bin_thresh, 153);
	load_file_player_frame_lib("Image_lib/Fall_L.bmp", 0, bin_thresh, 154);
	load_file_player_frame_lib("Image_lib/JumpUP_R.bmp", 0, bin_thresh, 155);
	load_file_player_frame_lib("Image_lib/JumpUP_L.bmp", 0, bin_thresh, 156);

	load_file_player_frame_lib("Image_lib/Jump_R_TH.bmp", 0, bin_thresh, 157);
	load_file_player_frame_lib("Image_lib/Jump_L_TH.bmp", 0, bin_thresh, 158);
	load_file_player_frame_lib("Image_lib/JumpUP_R_TH.bmp", 0, bin_thresh, 159);
	load_file_player_frame_lib("Image_lib/JumpUP_L_TH.bmp", 0, bin_thresh, 160);
	


	load_file_player_frame_lib("Image_lib/droneball.bmp", 15, bin_thresh, 200);




}


void load_player_animation_frames(int * frame, GameObject player)
{
	for (int i = 0; i < 37044; i++)
		frame[i] = frame_library[player.anim_step][i];
}

void draw_rope(Mat & frame, GameObject player, Rope rope, int x_offset, int y_offset)
{
	double theta = atan2(long double(player.y - rope.y), long double(player.x - rope.x));		//first thing to try is switching this order if something doesn't work (r - p vs p - r)

	theta = theta + 3.14159265359;

	int x = player.x; int y = player.y;
	double mag = 0.0;
	
//	while ( !(x > rope.x - 2 && x < rope.x + 2 && y > rope.y - 2 && y < rope.y + 2) )
	double xprev = x;
	double yprev = y;
	for (int i = 0; i < 150; i++)
	{
		

		mag = mag + .1;
		x += round(mag*cos(theta));
		y += round(mag*sin(theta));


		

		if (y + y_offset < frame.rows && y + y_offset > 0 && x + x_offset < frame.cols && x + x_offset > 0)
		{
			frame.data[frame.step[0] * (y + y_offset) + frame.step[1] * (x + x_offset) + 0] = 0;
			frame.data[frame.step[0] * (y + y_offset) + frame.step[1] * (x + x_offset) + 1] = 0;
			frame.data[frame.step[0] * (y + y_offset) + frame.step[1] * (x + x_offset) + 2] = 0;
		}

	}
	
	
}

void draw_obj(Mat & frame, GameObject obj, int x_offset, int y_offset)
{

	int x_pos = obj.x;
	int y_pos = obj.y;

	int tmp_obj_w = obj.width;
	int tmp_obj_h = obj.height;


	int r = 0; int g = 0; int b = 0;
	if (obj.obj_num == 0)
	{
		r = 255; g = 255; b = 255;
	}
	else if (obj.obj_num == 1)
	{
	}
	else if (obj.obj_num == 2)
	{
		r = 0; g = 0; b = 0;
	}
	else if (obj.obj_num == 3)
	{
		r = 0; g = 0; b = 255;
	}
	else if (obj.obj_num == 4)
	{
		r = 128; g = 0; b = 0;
	}
	else if (obj.obj_num == 7)
	{
		r = 255; g = 153; b = 0;
	}
	else if (obj.obj_num == 8)
	{
		r = 255; g = 0; b = 0;
	}



	else
	{
		r = 0; g = 255; b = 0;
	}


	if (obj.obj_num == 0 || obj.obj_num == 15)
	{

		int player_anim_frame[37044];			//12*15*3, or playerwidth*playerheight*3

		load_player_animation_frames(player_anim_frame, obj);

		for (int dy = -1 * pic_height / 2; dy < pic_height / 2; dy++)
		{
			for (int dx = -1 * pic_width / 2; dx < pic_width / 2; dx++)
			{
				if (y_pos + dy + y_offset > 0 && y_pos + dy + y_offset < frame.rows && x_pos + dx + x_offset > 0 && x_pos + dx + x_offset < frame.cols)
				{

					int b = player_anim_frame[(dy + pic_height / 2)*pic_width * 3 + (dx + pic_width / 2) * 3 + 0];	//+0 = b;
					int g = player_anim_frame[(dy + pic_height / 2)*pic_width * 3 + (dx + pic_width / 2) * 3 + 1];	//+1 = g;
					int r = player_anim_frame[(dy + pic_height / 2)*pic_width * 3 + (dx + pic_width / 2) * 3 + 2];	//+2 = r;
					if (b != -1 && g != -1 && r != -1)
					{
						frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 0] = b;
						frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 1] = g;
						frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 2] = r;
					}
					else
					{
					}

				}
			}
		}
	}

	else //if (obj.obj_num == 2 || obj.obj_num == 3)
	{
		for (int dy = -1 * tmp_obj_h / 2; dy < tmp_obj_h / 2; dy++)
		{
			for (int dx = -1 * tmp_obj_w / 2; dx < tmp_obj_w / 2; dx++)
			{
				if (y_pos + dy + y_offset > 0 && y_pos + dy + y_offset < frame.rows && x_pos + dx + x_offset > 0 && x_pos + dx + x_offset < frame.cols)
				{
					frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 0] = b;
					frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 1] = g;
					frame.data[frame.step[0] * (y_pos + dy + y_offset) + frame.step[1] * (x_pos + dx + x_offset) + 2] = r;
				}
			}
		}
	}
	
}
