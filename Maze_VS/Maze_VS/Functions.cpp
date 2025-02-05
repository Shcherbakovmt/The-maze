#include <iostream>
#include <time.h>
#include <locale.h>
#include <iso646.h> // Чтобы можно было писать and и or
#include "structs.h"
using namespace std;



extern bool command_changed = 0;
extern bool treasure_picked = 0;

extern bool show_mase = 0;
extern int turns_1 = 0;
extern int turns_2 = 0;
extern int time_in_swamp_1 = 0;
extern int time_in_swamp_2 = 0;
extern int k_along_bord = 1;
extern int k_r_around = 2;
extern int k_touch_walls = -4;
extern int quadr_dist_between_pits = 7;
extern int add_swamp_max = 4;
extern int add_swamp_min = 1;
extern unsigned seed = time(0);
extern int number_of_river = 22;
extern bool sucsess_river = 0;
extern bool sucsess_bum = 0;
extern bool sucsess_add_swamp = 0;
extern int trash = 0;
extern int count_of_land = 0;
extern string command = "";
extern string action = "";


extern point* complete_river = new point[30];
extern point center_of_swamp;
extern point medbat;
extern point arsenal;
extern point u_mouth;
extern point pit1, pit2, pit3, bum1, bum2, bum3;
extern player player_1;
extern player player_2;
extern point treasure;


extern point* arr_of_land_points = new point[64];
extern char** lines;
extern river_path* paths = new river_path[2000000];
extern point* river = new point[number_of_river + 1];
extern int count_of_path = 0;
extern long tries = 0;
extern long min_path_index = 0;


bool player_in_exit(player* pl)
{
    if ((*pl).coord.i == 9 or (*pl).coord.i == 0 or (*pl).coord.j == 9 or (*pl).coord.j == 0)
        return 1;
    else
        return 0;
}
int rand_from_a_to_b(int a, int b)
{
    seed = (seed * 73129 + 95121) % 10000;
    srand(seed);
    return rand() % (b - a + 1) + a;
}
void visual_lab(char** lines)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << lines[i][j] << " ";
        }
        cout << endl;
    }
}
char** init_arr(char** arr)
{
    arr = new char* [10];

    for (int i = 0; i < 10; i++)
    {
        arr[i] = new char[10];
    }

    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            arr[i][j] = '.';
        }
    }
    for (int n = 0; n < 10; n++)
    {
        arr[0][n] = 'w';
        arr[n][0] = 'w';
        arr[9][n] = 'w';
        arr[n][9] = 'w';
    }
    return arr;
}
bool is_point_in_arr(point* arr, point p, int sise)
{
    for (int k = 0; k < sise; k++)
    {
        if (arr[k].i == p.i and arr[k].j == p.j)
        {
            return 1;
        }
    }
    return 0;
}
bool is_there_path(char** lines, point u_mouth)
{
    bool trig = 0;

    if (u_mouth.j + 1 <= 8)
    {
        if (lines[u_mouth.i][u_mouth.j + 1] == '.') trig = 1;
    }
    if (u_mouth.j - 1 >= 1)
    {
        if (lines[u_mouth.i][u_mouth.j - 1] == '.') trig = 1;
    }
    if (u_mouth.i + 1 <= 8)
    {
        if (lines[u_mouth.i + 1][u_mouth.j] == '.') trig = 1;
    }
    if (u_mouth.i - 1 >= 1)
    {
        if (lines[u_mouth.i - 1][u_mouth.j] == '.') trig = 1;
    }
    return trig;
}
int is_along_border(char** lines, point p)
{
    if ((p.j + 1 > 8 and p.i - 1 < 1) or (p.j + 1 > 8 and p.i + 1 > 8) or (p.j - 1 < 1 and p.i - 1 < 1) or (p.j - 1 < 1 and p.i + 1 > 8)) return 10; // углы поля

    if (p.j + 1 > 8) return 1; // справа стена
    if (p.i - 1 < 1) return 2; // сверху
    if (p.j - 1 < 1) return 3; // слева
    if (p.i + 1 > 8) return 4; // снизу
    return 0;
}
int count_of_swamp_around(char** lines, point p)
{

    int count_of_s = 0;
    if (p.j + 1 <= 8)
    {
        if (lines[p.i][p.j + 1] == 's') count_of_s += 1; // справа
    }

    if (p.i + 1 <= 8)
    {
        if (lines[p.i + 1][p.j] == 's') count_of_s += 1; // снизу
    }

    if (p.i - 1 >= 1)
    {
        if (lines[p.i - 1][p.j] == 's') count_of_s += 1; // сверху
    }

    if (p.j - 1 >= 1)
    {
        if (lines[p.i][p.j - 1] == 's') count_of_s += 1; // слева
    }
    return count_of_s;
}
int count_of_r_around(char** lines, point p)
{

    int count_of_r = 0;
    if (p.j + 1 <= 8)
    {
        if (lines[p.i][p.j + 1] == 'r') count_of_r += 1; // справа
    }

    if (p.i + 1 <= 8)
    {
        if (lines[p.i + 1][p.j] == 'r') count_of_r += 1; // снизу
    }

    if (p.i - 1 >= 1)
    {
        if (lines[p.i - 1][p.j] == 'r') count_of_r += 1; // сверху
    }

    if (p.j - 1 >= 1)
    {
        if (lines[p.i][p.j - 1] == 'r') count_of_r += 1; // слева
    }
    return count_of_r;
}
int dist_quadr(point a, point b)
{
    return (b.i - a.i) * (b.i - a.i) + (b.j - a.j) * (b.j - a.j);
}
int number_of_land(char** lines, point* arr_of_p, int count_of_t)
{
    int count_of_possible;
    //int next_point_index;

    point* possible_points = new point[4]; // определение возможных клеток, куда дальше может течь река
    count_of_possible = 0;

    if (arr_of_p[count_of_t - 1].j + 1 <= 8)
    {
        if (lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j + 1] == '.' or
            lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j + 1] == 'c')
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i;
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j + 1; // шаг вправо
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].i + 1 <= 8)
    {
        if (lines[arr_of_p[count_of_t - 1].i + 1][arr_of_p[count_of_t - 1].j] == '.' or
            lines[arr_of_p[count_of_t - 1].i + 1][arr_of_p[count_of_t - 1].j] == 'c')
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i + 1; // шаг вниз
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j;
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].j - 1 >= 1)
    {
        if (lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j - 1] == '.' or
            lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j - 1] == 'c') // шаг влево
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i;
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j - 1;
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].i - 1 >= 1)
    {
        if (lines[arr_of_p[count_of_t - 1].i - 1][arr_of_p[count_of_t - 1].j] == '.' or
            lines[arr_of_p[count_of_t - 1].i - 1][arr_of_p[count_of_t - 1].j] == 'c')
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i - 1;
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j; // шаг вверх
            count_of_possible += 1;
        }
    }
    if (count_of_possible == 0)
    {
        delete[] possible_points;
        return 0;
    }
    for (int next_point_index = 0; next_point_index < count_of_possible; next_point_index++)
    {
        lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = 't';
        // visual_lab(lines); чтобы видеть как проверяется суша
        if (not is_point_in_arr(arr_of_land_points, possible_points[next_point_index], 64))
        {
            count_of_land += 1;
            arr_of_land_points[count_of_land - 1] = possible_points[next_point_index];
        }
        arr_of_p[count_of_t] = possible_points[next_point_index];
        trash = number_of_land(lines, arr_of_p, count_of_t + 1);
        lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}
int number_of_land_complitely(char** lines)
{
    int maxx = 0;
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (lines[i][j] == '.' or lines[i][j] == 'c')
            {
                point* arr_of_p = new point[64];
                int count_of_t = 1;
                point p;
                p.i = i;
                p.j = j;
                arr_of_p[0] = p;
                count_of_land = 0;
                for (int k = 0; k < 64; k++)
                {
                    arr_of_land_points[k].i = -1;
                    arr_of_land_points[k].j = -1;
                }
                trash = number_of_land(lines, arr_of_p, count_of_t);
                delete[] arr_of_p;
                if (count_of_land > maxx) maxx = count_of_land;
            }
        }
    }
    return maxx;
}
bool create_river(char** lines, point* river, int count_of_river) // создание массива со всеми возможными путями и их характеристиками
{

    if (count_of_river == number_of_river) // условие того, что река завершена и завершена на границе лабиринта (опционально)
    {
        // if(number_of_land_complitely(lines) < 20) - как выяснилось тогда тратиться овердохера времени и памяти (7Гб и минут 5 ожидания)
        if (is_along_border(lines, river[number_of_river - 1]) != 0)
        {
            count_of_path += 1;
            paths[count_of_path - 1].river = new point[number_of_river];
            //visual_lab(lines);
            paths[count_of_path - 1].count_of_r_around_summ = 0;
            paths[count_of_path - 1].count_river_along_bord = 0;
            paths[count_of_path - 1].numb_toched_walls = 0;
            paths[count_of_path - 1].toched_walls = new int[20];
            for (int i = 0; i < 5; i++) paths[count_of_path - 1].toched_walls[i] = 0;

            for (int k = 0; k < number_of_river; k++)
            {
                paths[count_of_path - 1].river[k] = river[k];
                paths[count_of_path - 1].count_of_r_around_summ += count_of_r_around(lines, paths[count_of_path - 1].river[k]);
                if (is_along_border(lines, river[k]) != 0)
                {
                    paths[count_of_path - 1].count_river_along_bord += 1;
                    paths[count_of_path - 1].toched_walls[is_along_border(lines, river[k])] = 1;
                }
            }
            for (int i = 0; i < 5; i++)
            {
                paths[count_of_path - 1].numb_toched_walls += paths[count_of_path - 1].toched_walls[i];
            }


            tries += 1;
            delete[] paths[count_of_path - 1].toched_walls;

        }

        return 0;

    }
    int count_of_possible;
    int next_river_point_index;

    point* possible_points = new point[3]; // определение возможных клеток, куда дальше может течь река
    count_of_possible = 0;
    if (river[count_of_river - 1].j + 1 <= 8)
    {
        if (lines[river[count_of_river - 1].i][river[count_of_river - 1].j + 1] == '.')
        {
            possible_points[count_of_possible].i = river[count_of_river - 1].i;
            possible_points[count_of_possible].j = river[count_of_river - 1].j + 1; // шаг вправо
            count_of_possible += 1;
        }
    }

    if (river[count_of_river - 1].i + 1 <= 8)
    {
        if (lines[river[count_of_river - 1].i + 1][river[count_of_river - 1].j] == '.')
        {
            possible_points[count_of_possible].i = river[count_of_river - 1].i + 1; // шаг вниз
            possible_points[count_of_possible].j = river[count_of_river - 1].j;
            count_of_possible += 1;
        }
    }
    if (river[count_of_river - 1].j - 1 >= 1)
    {
        if (lines[river[count_of_river - 1].i][river[count_of_river - 1].j - 1] == '.') // шаг влево
        {
            possible_points[count_of_possible].i = river[count_of_river - 1].i;
            possible_points[count_of_possible].j = river[count_of_river - 1].j - 1;
            count_of_possible += 1;
        }
    }
    if (river[count_of_river - 1].i - 1 >= 1)
    {
        if (lines[river[count_of_river - 1].i - 1][river[count_of_river - 1].j] == '.')
        {
            possible_points[count_of_possible].i = river[count_of_river - 1].i - 1;
            possible_points[count_of_possible].j = river[count_of_river - 1].j; // шаг вверх
            count_of_possible += 1;
        }
    }
    if (count_of_possible == 0)
    {
        delete[] possible_points;
        return 0;
    }
    for (int next_river_point_index = 0; next_river_point_index < count_of_possible; next_river_point_index++)
    {
        lines[possible_points[next_river_point_index].i][possible_points[next_river_point_index].j] = 'r';
        // visual_lab(lines); чтобы видеть как строится река
        river[count_of_river] = possible_points[next_river_point_index];
        trash = create_river(lines, river, count_of_river + 1);
        lines[possible_points[next_river_point_index].i][possible_points[next_river_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}
bool create_bum(char** lines)
{
    point* possible = new point[64];
    int count_poss = 0;
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (lines[i][j] == '.')
            {
                possible[count_poss].i = i;
                possible[count_poss].j = j;
                count_poss += 1;
            }
        }
    }
    point bum1;
    point bum2;
    //point bum3;
    int repeat = 0;
    do
    {
        if (repeat != 0)
        {
            lines[bum1.i][bum1.j] = '.';
            lines[bum2.i][bum2.j] = '.';
            //lines[bum3.i][bum3.j] = '.';
        }
        bum1 = possible[rand_from_a_to_b(0, count_poss - 1)];
        bum2 = possible[rand_from_a_to_b(0, count_poss - 1)];
        //bum3 = possible[rand_from_a_to_b(0,count_poss-1)];
        while (bum2.i == bum1.i and bum2.j == bum1.j) bum2 = possible[rand_from_a_to_b(0, count_poss - 1)]; // выбираю точку так, чтобы оба бума не попали в одну
        //while((bum3.i == bum1.i and bum3.j == bum1.j) or (bum3.i == bum2.i and bum3.j == bum2.j)) bum3 = possible[rand_from_a_to_b(0,count_poss-1)]; // аналогично
        lines[bum1.i][bum1.j] = 'b';
        lines[bum2.i][bum2.j] = 'b';
        //lines[bum3.i][bum3.j] = 'b';
        repeat += 1;
    } while (false);

    delete[] possible;
    return 1;
}
void create_pits(char** lines)
{
    do
    {
        pit1.i = rand_from_a_to_b(1, 8);
        pit1.j = rand_from_a_to_b(1, 8);

        pit2.i = rand_from_a_to_b(1, 8);
        pit2.j = rand_from_a_to_b(1, 8);

        pit3.i = rand_from_a_to_b(1, 8);
        pit3.j = rand_from_a_to_b(1, 8);
        //
    } while (not (lines[pit1.i][pit1.j] == '.' and lines[pit2.i][pit2.j] == '.' and lines[pit3.i][pit3.j] == '.' and
        dist_quadr(pit1, pit2) >= quadr_dist_between_pits and dist_quadr(pit1, pit3) >= quadr_dist_between_pits and dist_quadr(pit2, pit3) >= quadr_dist_between_pits and
        dist_quadr(pit1, u_mouth) > 1 and dist_quadr(u_mouth, pit3) > 1 and dist_quadr(pit2, u_mouth) > 1 and
        dist_quadr(pit1, medbat) > 1 and dist_quadr(medbat, pit3) > 1 and dist_quadr(pit2, medbat) > 1));// and
    //number_of_land_complitely(lines) < 100
    lines[pit1.i][pit1.j] = '1';
    lines[pit2.i][pit2.j] = '2';
    lines[pit3.i][pit3.j] = '3';

}
bool create_river_complitely(char** lines, point u_mouth)
{
    river[0] = u_mouth;
    count_of_path = 0;
    sucsess_river = create_river(lines, river, 1);
    if (count_of_path == 0) return 0;
    long minn = 10000000;
    min_path_index = 0;

    for (int k = 0; k < count_of_path; k++)
    {
        if (k_r_around * paths[k].count_of_r_around_summ + k_along_bord * paths[k].count_river_along_bord + k_touch_walls * paths[k].numb_toched_walls < minn)
        {
            minn = k_r_around * paths[k].count_of_r_around_summ + k_along_bord * paths[k].count_river_along_bord + k_touch_walls * paths[k].numb_toched_walls;
            min_path_index = k;
        }
    }

    for (int k = 0; k < number_of_river; k++)
    {
        lines[paths[min_path_index].river[k].i][paths[min_path_index].river[k].j] = 'r';
        complete_river[k].i = paths[min_path_index].river[k].i;
        complete_river[k].j = paths[min_path_index].river[k].j;
    }
    lines[u_mouth.i][u_mouth.j] = 'u';

    return 1;
}
void create_ars_and_med(char** lines)
{
    do
    {
        arsenal.i = rand_from_a_to_b(1, 8);
        arsenal.j = rand_from_a_to_b(1, 8);

        medbat.i = rand_from_a_to_b(2, 7);
        medbat.j = rand_from_a_to_b(2, 7);

    } while (not (lines[arsenal.i][arsenal.j] == '.' and lines[medbat.i][medbat.j] == '.' and
        dist_quadr(medbat, arsenal) >= 9 and dist_quadr(medbat, u_mouth) >= 9 and dist_quadr(u_mouth, arsenal) >= 9));

    lines[arsenal.i][arsenal.j] = 'A';
    lines[medbat.i][medbat.j] = 'M';
}
void create_u_mouth_and_swamp(char** lines)
{
    center_of_swamp.i = rand_from_a_to_b(3, 6);
    center_of_swamp.j = rand_from_a_to_b(3, 6); // рандомно определили центр болота
    for (int n = -1; n <= 1; n++)
    {
        for (int m = -1; m <= 1; m++)
        {
            lines[center_of_swamp.i + n][center_of_swamp.j + m] = 's';
        }
    }
    u_mouth.i = rand_from_a_to_b(center_of_swamp.i - 1, center_of_swamp.i + 1);
    u_mouth.j = rand_from_a_to_b(center_of_swamp.j - 1, center_of_swamp.j + 1);
    while ((u_mouth.i == center_of_swamp.i and u_mouth.j == center_of_swamp.j) or not (is_there_path(lines, u_mouth)))
    {
        u_mouth.i = rand_from_a_to_b(center_of_swamp.i - 1, center_of_swamp.i + 1);
        u_mouth.j = rand_from_a_to_b(center_of_swamp.j - 1, center_of_swamp.j + 1);
    }
    lines[u_mouth.i][u_mouth.j] = 'u';
}
bool create_add_swamp(char** lines)
{
    int number_of_add_swamp = rand_from_a_to_b(add_swamp_min, add_swamp_max);
    point* possible = new point[20];
    point* added_swamp = new point[20];
    int count_of_added = 0;
    int count_of_possible_swamp = 0;
    point p;
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            p.i = i;
            p.j = j;
            if (lines[i][j] == '.' and count_of_swamp_around(lines, p) >= 1)
            {
                count_of_possible_swamp += 1;
                possible[count_of_possible_swamp - 1].i = p.i;
                possible[count_of_possible_swamp - 1].j = p.j;
            }
        }
    }
    if (count_of_possible_swamp == 0) return 1;
    if (number_of_add_swamp + 1 >= count_of_possible_swamp) number_of_add_swamp = count_of_possible_swamp - 1;
    for (int k = 0; k < number_of_add_swamp; k++)
    {
        point s = possible[rand_from_a_to_b(0, count_of_possible_swamp - 1)];
        if (not is_point_in_arr(added_swamp, s, count_of_added)) // утечки нет
        { //
            count_of_added += 1; //
            added_swamp[count_of_added - 1].i = s.i; //
            added_swamp[count_of_added - 1].j = s.j; //
            lines[s.i][s.j] = 's'; //
        }  //
    }

    delete[] added_swamp;
    delete[] possible;
    return 1;
}
void create_land(char** lines)
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (lines[i][j] == '.') lines[i][j] = 'c';
        }
    }
}
void create_treasure(char** lines)
{
    int count_of_possible = 0;
    point* possible = new point[64];
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (lines[i][j] == 'c')
            {
                count_of_possible += 1;
                possible[count_of_possible - 1].i = i;
                possible[count_of_possible - 1].j = j;
            }

        }
    }
    int index = rand_from_a_to_b(0, count_of_possible - 1);
    treasure = possible[index];
    delete[] possible;
}
void death(char** lines, player* player)
{
    (*player).hp = 2;

    (*player).granade = 0;
    (*player).bullets = 0;
    if ((*player).with_treasure == 1)
    {
        treasure_picked = 0;
        cout << (*player).name << " выронил клад" << endl;
        (*player).with_treasure = 0;
        if (lines[(*player).coord.i][(*player).coord.j] == 's' or lines[(*player).coord.i][(*player).coord.j] == 'u')
        {
            create_treasure(lines);
            cout << "Крокодил съел клад месте с " << (*player).name << " и высрал его в другом месте" << endl;
        }
    }
    (*player).coord = medbat;
    cout << (*player).name << " умер и появился в медсанбате" << endl;
}
void create_exit(char** lines)
{
    int y_left, y_right, x_up, x_down;
    y_left = rand_from_a_to_b(1, 8);
    y_right = rand_from_a_to_b(1, 8);
    x_up = rand_from_a_to_b(1, 8);
    x_down = rand_from_a_to_b(1, 8);
    lines[y_left][0] = 'E';
    lines[y_right][9] = 'E';
    lines[0][x_up] = 'E';
    lines[9][x_down] = 'E';

}
char** create_labirint()
{
    tries = 0;
    do
    {
        if (tries != 0)
        {
            for (int i = 0; i < 10; i++)
            {
                delete[] lines[i];
            }
            delete[] lines;
            for (int k = 0; k < count_of_path; k++)
            {
                delete[] paths[k].river;
            }
        }
        lines = init_arr(lines);
        create_u_mouth_and_swamp(lines); // сделали болото и рандомно определили в нем устье

        create_ars_and_med(lines); // теперь нужно добавить арсенал и медсанбат на расстоянии^2 не менее 17
        create_pits(lines); //  Теперь нужно сделать ямы по такому же принципу
        sucsess_river = create_river_complitely(lines, u_mouth); // Теперь сделаем реку - порядка 22 последовательных клеток
        sucsess_bum = create_bum(lines);
        sucsess_add_swamp = create_add_swamp(lines);

    } while (not(sucsess_river == 1 and number_of_land_complitely(lines) < 10));

    cout << "number_of_land = " << number_of_land_complitely(lines) << endl;
    cout << "min_path_index = " << min_path_index << endl;

    cout << "min_number = " << k_r_around * paths[min_path_index].count_of_r_around_summ << " + " <<
        k_along_bord * paths[min_path_index].count_river_along_bord << " + " <<
        k_touch_walls * paths[min_path_index].numb_toched_walls << endl;

    cout << "paths[min_path_index].river[number_of_river] i and j = " << paths[min_path_index].river[number_of_river - 1].i << " " << paths[min_path_index].river[number_of_river - 1].j << endl;
    cout << "count_of_paths = " << count_of_path << endl;


    create_land(lines);
    create_exit(lines);
    return lines;
}
bool shoot(char** lines, player* victim, player* hunter, string cmd)
{
    if ((*hunter).bullets == 0)
    {
        cout << "Ты спускаешь курок и ничего не происходит. Ты потратил ход" << endl;
        return 1;
    }
    (*hunter).bullets -= 1;
    cout << "У тебя осталось " << (*hunter).bullets << " патрона" << endl;
    if (cmd == "Down")
    {
        for (int k = (*hunter).coord.i; k <= 9; k++)
        {
            if (k == (*victim).coord.i and (*hunter).coord.j == (*victim).coord.j)
            {
                (*victim).hp -= 1;
                if ((*victim).hp == 0) death(lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
        }
    }
    if (cmd == "Up")
    {
        for (int k = (*hunter).coord.i; k >= 0; k--)
        {
            if (k == (*victim).coord.i and (*hunter).coord.j == (*victim).coord.j)
            {
                (*victim).hp -= 1;
                if ((*victim).hp == 0) death(lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
        }
    }
    if (cmd == "Right")
    {
        for (int k = (*hunter).coord.j; k <= 9; k++)
        {
            if (k == (*victim).coord.j and (*hunter).coord.i == (*victim).coord.i)
            {
                (*victim).hp -= 1;
                if ((*victim).hp == 0) death(lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
        }
    }
    if (cmd == "Left")
    {
        for (int k = (*hunter).coord.j; k >= 0; k--)
        {
            if (k == (*victim).coord.j and (*hunter).coord.i == (*victim).coord.i)
            {
                (*victim).hp -= 1;
                if ((*victim).hp == 0) death(lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
        }
    }
    cout << "Мимо" << endl;
    return 0;
}
bool explode(char** lines, player* player, string cmd)
{
    int j = (*player).coord.j;
    int i = (*player).coord.i;
    if ((*player).granade == 0)
    {
        cout << "Ты засовываешь руку в карман, достаешь гранатаобразный камень и кидаешь. Ничего не произошло. Ты потратил ход" << endl;
        return 1;
    }
    (*player).granade -= 1;
    if (cmd == "Up")
    {
        if (i - 1 < 1)
        {
            cout << "Стена не взорвалась" << endl;
            cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
            return 1;
        }
        else
        {
            if (lines[i - 1][j] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                lines[i - 1][j] = 'c';
                return 1;
            }
        }
    }
    if (cmd == "Down")
    {
        if (i + 1 > 8)
        {
            cout << "Стена не взорвалась" << endl;
            cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
            return 1;
        }
        else
        {
            if (lines[i + 1][j] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                lines[i + 1][j] = 'c';
                return 1;
            }
        }
    }
    if (cmd == "Left")
    {
        if (j - 1 < 1)
        {
            cout << "Стена не взорвалась" << endl;
            cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
            return 1;
        }
        else
        {
            if (lines[i][j - 1] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                lines[i][j - 1] = 'c';
                return 1;
            }
        }
    }
    if (cmd == "Right")
    {
        if (j + 1 > 8)
        {
            cout << "Стена не взорвалась" << endl;
            cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
            return 1;
        }
        else
        {
            if (lines[i][j + 1] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                lines[i][j + 1] = 'c';
                return 1;
            }
        }
    }
    cout << "Там ни бума ни стены не было, чё ты вообще сделал? Гранаты не взрывают крокодила и игроков" << endl;
    cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
    return 1;
}

bool step(player* player, int turns, int* time_in_swamp, int id)
{
    command_changed = 0;
    if (player_in_exit(player) and turns != 0) // если человек находится "в выходе"
    {
        cout << "Игрок " << (*player).name << " заходит обратно в лабиринт" << endl;
        if ((*player).coord.i == 9) command = "Up";
        if ((*player).coord.i == 0) command = "Down";
        if ((*player).coord.j == 9) command = "Left"; // определяем куда надо шагнуть обратно
        if ((*player).coord.j == 0) command = "Right";
        command_changed = 1;
    }
    if ((*player).coord.i == treasure.i and (*player).coord.j == treasure.j // на случай если сокровище заспавнилось прямо под игроком
        and (*player).with_treasure == 0 and (*player).hp == 2)
    {
        cout << (*player).name << " обнаружил под собой клад. Берешь его?" << endl;
        cin >> action;
        if (action == "Yes" or action == "y" or action == "Y") (*player).with_treasure = 1;
    }
    if ((*player).coord.i == treasure.i and (*player).coord.j == treasure.j
        and (*player).with_treasure == 0 and (*player).hp != 2)
    {
        cout << (*player).name << " обнаружил под собой клад но не может его взять т.к. не полное здоровье" << endl;
    }
    char temp_1;
    char temp_2;
    char temp_3;
    if (turns == 0) // высадка
    {
        cout << "Введите координаты высадки (цифры от 1 до 8)" << endl;
        char x;
        char y;
        cin >> x >> y;

        do
        {
            while (not(isdigit(x) and isdigit(y)) or
                not(1 <= x - '0' and x - '0' <= 8 and 1 <= y - '0' and y - '0' <= 8))
            {
                cout << "P.s. координата - это число, а не буквы. Причем надо ввести от 1 до 8. Сначала координата по Х, потом по Y, причем ось Y направлена вниз" << endl;
                cin >> x >> y;
            }
            (*player).coord.j = x - '0';
            (*player).coord.i = y - '0';

        } while (not(1 <= (*player).coord.i and (*player).coord.i <= 8 and
            1 <= (*player).coord.j and (*player).coord.j <= 8));
        while (lines[(*player).coord.i][(*player).coord.j] == 'b')
        {
            cout << "Ты высадился в бум. Пожалуйста выбери другую точку" << endl;
            cout << "Введите координаты высадки (цифры от 1 до 8)" << endl;
            cin >> x >> y;
            do
            {
                while (not(isdigit(x) and isdigit(y)) or
                    not(1 <= x - '0' and x - '0' <= 8 and 1 <= y - '0' and y - '0' <= 8))
                {
                    cout << "P.s. координата - это число, а не буквы. Причем надо ввести от 1 до 8. Сначала координата по Х, потом по Y, причем ось Y направлена вниз" << endl;
                    cin >> x >> y;
                }
                (*player).coord.j = x - '0';
                (*player).coord.i = y - '0';

            } while (not(1 <= (*player).coord.i and (*player).coord.i <= 8 and
                1 <= (*player).coord.j and (*player).coord.j <= 8));
        }



        (*player).coord.i += 1; // чтобы в нулевой ход игрок пришел на клетку, куда захотел высадиться
        command = "Up";
        command_changed = 1;
    }
    if (not(command_changed)) cin >> command; // P.s. command это extern string обьявленный в самом начале
    if (command == "Show_maze")
    {
        show_mase = 1;

        temp_1 = lines[player_1.coord.i][player_1.coord.j];
        temp_2 = lines[player_2.coord.i][player_2.coord.j];
        temp_3 = lines[treasure.i][treasure.j];
        lines[player_1.coord.i][player_1.coord.j] = 'O'; // Нанести игрока на карту лабиринта временно (чтобы было видно где он)
        lines[treasure.i][treasure.j] = 't';
        lines[player_2.coord.i][player_2.coord.j] = 'T';

        visual_lab(lines);
        cout << endl;
        // if(show_mase) visual_lab(lines);                  // Вывести лабиринт
        lines[player_1.coord.i][player_1.coord.j] = temp_1; // Вернуть в точку в лабиринте то что было
        lines[player_2.coord.i][player_2.coord.j] = temp_2;
        lines[treasure.i][treasure.j] = temp_3;
        cin >> command;
    }
    if (command == "Hide_maze")
    {
        show_mase = 0;
        cin >> command;
    }
    if (command == "Stop" or command == "stop") return 1;


    bool trig = 0;
    while (not(command == "Up" or command == "Down" or command == "Left" or command == "Right"
        or command == "Show_maze" or command == "Shoot" or command == "Explode"))
    {
        cout << "Херню какую то ввел, давай нормальную команду 819 строка";
        cin >> command;
    }
    if (command == "Up") (*player).coord.i -= 1;

    if (command == "Down") (*player).coord.i += 1;

    if (command == "Right") (*player).coord.j += 1;

    if (command == "Left") (*player).coord.j -= 1;

    if ((*player).coord.i == treasure.i and (*player).coord.j == treasure.j and treasure_picked != 1 and (*player).hp == 2) // если нашел клад
    {
        cout << (*player).name << " нашел клад. Берешь его?" << endl;
        cin >> action;
        if (action == "Yes" or action == "y" or action == "Y")
        {
            (*player).with_treasure = 1;
            treasure_picked = 1;
        }
    }

    if ((*player).coord.i == treasure.i and (*player).coord.j == treasure.j and treasure_picked != 1 and (*player).hp < 2)
    {
        cout << (*player).name << " нашел клад но не может его взять" << endl;
    }

    if ((*player).with_treasure)
    {
        treasure.i = (*player).coord.i;
        treasure.j = (*player).coord.j;
    }
    if (command == "Shoot")
    {
        cout << "Куда стреляешь?" << endl;
        cin >> command;
        if (id == 10) // Тогда охотник - первый игрок (И сейчас ход 1 игрока)
        {
            trash = shoot(lines, &player_2, player, command);
            return 1;
        }
        if (id == 20) // Тогда охотник - второй игрок (И сейчас ход 2 игрока)
        {
            trash = shoot(lines, &player_1, player, command);
            return 1;
        }
    }
    if (command == "Explode")
    {
        cout << "Куда бросаешь гранату?" << endl;
        cin >> action;
        explode(lines, player, action);
        return 1;
    }
    /*if(trig == 0) // кажется можно удалить т.к. trig = 0 когда челик никуда не ушел (раньше код был такой)
    {
        cout << "Бум"<< endl;
        if(lines[(*player).coord.i][(*player).coord.j] == 's')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                death(lines, player );
                (*time_in_swamp) = 0;
                return 1;
            }
            return 1;
        }
        return 1;
    } */

    if (lines[(*player).coord.i][(*player).coord.j] == 'r')
    {
        (*time_in_swamp) = 0;
        for (int k = 0; k < number_of_river; k++)
        {
            if ((*player).coord.i == complete_river[k].i and (*player).coord.j == complete_river[k].j)
            {

                if (k - 3 > 0)
                {
                    (*player).coord = complete_river[k - 3];
                    cout << "Река понесла" << endl;
                    return 1;
                    break;
                }
                else
                {
                    (*player).coord = u_mouth;
                    cout << "Река в устье принесла" << endl;
                    (*time_in_swamp) += 1;
                    cout << "Болото рычит " << (*time_in_swamp) << " раз";
                    return 1;
                    break;

                }
            }
        }
    }

    if (lines[(*player).coord.i][(*player).coord.j] == 'c')
    {
        (*time_in_swamp) = 0;
        cout << "Суша" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 'u')
    {
        cout << "Пришел в устье" << endl;
        (*time_in_swamp) += 1;
        cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
        if ((*time_in_swamp) == 3)
        {
            cout << "Покусан крокодилом, отправился в медсанбат" << endl;
            death(lines, player);
            (*time_in_swamp) = 0;
            return 1;
        }
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 'M')
    {
        (*player).hp = 2;
        (*time_in_swamp) = 0;
        cout << "Медсанбат" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 'A')
    {
        (*player).bullets = 3;
        (*player).granade = 2;
        (*time_in_swamp) = 0;
        cout << "Арсенал" << endl;
        cout << "Ты получил 3 патрона и 2 гранаты" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == '1')
    {
        (*time_in_swamp) = 0;
        (*player).coord = pit2;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == '2')
    {
        (*time_in_swamp) = 0;
        (*player).coord = pit3;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == '3')
    {
        (*time_in_swamp) = 0;
        (*player).coord = pit1;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 's')
    {
        (*time_in_swamp) += 1;
        cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
        if ((*time_in_swamp) == 3)
        {
            cout << "Покусан крокодилом, отправился в медсанбат" << endl;
            death(lines, player);
            (*time_in_swamp) = 0;
            return 1;
        }
        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 'b' or
        lines[(*player).coord.i][(*player).coord.j] == 'w')
    {
        cout << "Бум" << endl;

        if (command == "Up") (*player).coord.i += 1;
        if (command == "Down") (*player).coord.i -= 1;
        if (command == "Right") (*player).coord.j -= 1;
        if (command == "Left") (*player).coord.j += 1;

        if ((*player).with_treasure)
        {
            treasure.i = (*player).coord.i;
            treasure.j = (*player).coord.j;
        }

        if (lines[(*player).coord.i][(*player).coord.j] == 's' or lines[(*player).coord.i][(*player).coord.j] == 'u')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                (*time_in_swamp) = 0;
                death(lines, player);
                return 1;
            }

        }

        return 1;
    }
    if (lines[(*player).coord.i][(*player).coord.j] == 'E')
    {
        if ((*player).with_treasure)
        {
            (*time_in_swamp) = 0;
            cout << "Игрок" << (*player).id << "вышел из лабиринта с кладом и победил!" << endl;
        }
        else
        {
            (*time_in_swamp) = 0;
            cout << "Ты нашел выход! Следующий ход тратишь на то чтобы выйти (сделай шаг обратно)" << endl;
        }
        return 1;
    }



    return 0;

}
