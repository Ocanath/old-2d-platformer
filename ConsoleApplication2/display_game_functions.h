#ifndef DISPLAY_GAME_FUNCTIONS_H 
#define DISPLAY_GAME_FUNCTIONS_H


#include <iostream>
#include <string>

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2\opencv.hpp>

#include "GameState.h"

using namespace std;
using namespace cv;



void print_processed_frame_to_text(int frame_addr, int print_addr);

void crop_image_helper(string filename_in, string filename_out, int left_bord, int right_bord, int top_bord, int bott_bord);
void load_file_player_frame_lib(string filename, int bin_thresh, GameObject player, int address);
void init_player_frame_library();
void load_player_animation_frames(int * frame, GameObject player);
void draw_obj(Mat & frame, GameObject obj, int x_offset, int y_offset);
void draw_rope(Mat & frame, GameObject player, Rope rope, int x_offset, int y_offset);

int is_inside(GameObject moving_obj, GameObject  env_obj);

#endif