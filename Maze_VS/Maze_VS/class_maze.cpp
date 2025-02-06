#include <iostream>
#include <time.h>
#include <locale.h>
#include <iso646.h> // Чтобы можно было писать and и or
using namespace std;

struct point
{
    int i;
    int j;
};
struct river_path
{
    point* river;
    int count_of_r_around_summ = 0;
    int count_river_along_bord = 0;
    int* toched_walls = 0;
    int numb_toched_walls = 0;
};
struct player
{
    string name;
    int id;
    point coord;
    bool with_treasure = 0;
    int hp = 2;
    int bullets = 0;
    int granade = 0;
};
struct crocodile
{
    bool alive = 1;
    point coord;
};


int rand_from_a_to_b(int a, int b, unsigned seed)
{
    seed = (seed * 73129 + 95121) % 10000;
    srand(seed);
    return rand() % (b - a + 1) + a;
}
bool player_in_exit(player* pl)
{
    if ((*pl).coord.i == 9 or (*pl).coord.i == 0 or (*pl).coord.j == 9 or (*pl).coord.j == 0)
        return 1;
    else
        return 0;
} //самостоятельная функция
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

int number_of_land(char** lines, point* arr_of_p, int count_of_t, technical_peremens* tech)
{
    int trash = 0;
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
        if (not is_point_in_arr((*tech).arr_of_land_points, possible_points[next_point_index], 64))
        {
            (*tech).count_of_land += 1;
            (*tech).arr_of_land_points[(*tech).count_of_land - 1] = possible_points[next_point_index];
        }
        arr_of_p[count_of_t] = possible_points[next_point_index];
        trash = number_of_land(lines, arr_of_p, count_of_t + 1, tech);
        lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}
int number_of_land_complitely(char** lines, technical_peremens* tech)
{
    int trash = 0;
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
                (*tech).count_of_land = 0;
                for (int k = 0; k < 64; k++)
                {
                    (*tech).arr_of_land_points[k].i = -1;
                    (*tech).arr_of_land_points[k].j = -1;
                }
                trash = number_of_land(lines, arr_of_p, count_of_t, tech);
                delete[] arr_of_p;
                if ((*tech).count_of_land > maxx) maxx = (*tech).count_of_land;
            }
        }
    }
    return maxx;
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



struct settings
{

};
struct technical_peremens
{
    unsigned seed = time(0);
    point* arr_of_land_points = new point[64];
    int count_of_land = 0;
};
struct elements
{

};
class maze
{

};