#pragma once
#include <iostream>
#include <string>
#include "structs.h"
using namespace std;

extern bool command_changed;
extern bool treasure_picked;
extern point treasure;
extern bool show_mase;
extern int turns_1;
extern int turns_2;
extern int time_in_swamp_1;
extern int time_in_swamp_2;
extern int k_along_bord;
extern int k_r_around;
extern int k_touch_walls;
extern int quadr_dist_between_pits;
extern int add_swamp_max;
extern int add_swamp_min;
extern unsigned seed;
extern int number_of_river;
extern bool sucsess_river;
extern bool sucsess_bum;
extern bool sucsess_add_swamp;
extern int trash;
extern int count_of_land;
extern string command;
extern string action;

extern point* complete_river;
extern point center_of_swamp;
extern point medbat;
extern point arsenal;
extern point u_mouth;
extern point pit1, pit2, pit3, bum1, bum2, bum3;
extern player player_1;
extern player player_2;


extern point* arr_of_land_points;
extern char** lines;
extern river_path* paths;
extern point* river;
extern int count_of_path;
extern long tries;
extern long min_path_index;





bool player_in_exit(player* pl);
int rand_from_a_to_b(int a, int b);
void visual_lab(char** lines);
char** init_arr(char** arr);
bool is_point_in_arr(point* arr, point p, int sise);
bool is_there_path(char** lines, point u_mouth);
int is_along_border(char** lines, point p);
int count_of_swamp_around(char** lines, point p);
int count_of_r_around(char** lines, point p);
int dist_quadr(point a, point b);
int number_of_land(char** lines, point* arr_of_p, int count_of_t);
int number_of_land_complitely(char** lines);
bool create_river(char** lines, point* river, int count_of_river);
bool create_bum(char** lines);
void create_pits(char** lines);
bool create_river_complitely(char** lines, point u_mouth);
void create_ars_and_med(char** lines);
void create_u_mouth_and_swamp(char** lines);
bool create_add_swamp(char** lines);
void create_land(char** lines);
void create_treasure(char** lines);
void death(char** lines, player* player);
void create_exit(char** lines);
char** create_labirint();
bool shoot(char** lines, player* victim, player* hunter, string cmd);
bool explode(char** lines, player* player, string cmd);
bool step(player* player, int turns, int* time_in_swamp, int id);
