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

class tech
{
public:
    static bool command_changed;
    static bool treasure_picked;
    static point treasure;
    static int turns_1;
    static int turns_2;
    static int time_in_swamp_1;
    static int time_in_swamp_2;
    static int k_along_bord;
    static int k_r_around;
    static int k_touch_walls;
    static int quadr_dist_between_pits;
    static unsigned seed;
    static bool sucsess_river;
    static bool sucsess_bum;
    static bool sucsess_add_swamp;
    static int trash;
    static string command;
    static string action;
    static int count_of_path;
    static long tries;
    static long min_path_index;
    static river_path* paths;

    tech();
    ~tech();
};
tech::tech()
{
    static bool command_changed = 0;
    static bool treasure_picked = 0;
    static point treasure;
    static int turns_1 = 0;
    static int turns_2 = 0;
    static int time_in_swamp_1 = 0;
    static int time_in_swamp_2 = 0;
    static int k_along_bord = 1;
    static int k_r_around = 2;
    static int k_touch_walls = -4;
    static int quadr_dist_between_pits = 7;
    static unsigned seed = time(0);
    static bool sucsess_river = 0;
    static bool sucsess_bum = 0;
    static bool sucsess_add_swamp = 0;
    static int trash = 0;
    static string command;
    static string action;
    static river_path* paths = new river_path[2000000];
    static int count_of_path = 0;
    static long tries = 0;
    static long min_path_index = 0;
}
tech::~tech()
{
    delete[] paths;
}

static class settings
{
    static bool show_mase; // показывать лабиринт или нет
    static int add_swamp_max;
    static int add_swamp_min; // количество допольнительного болота
    static int number_of_river;
    static int count_of_land;
    settings();
};
settings::settings()
{
    static bool show_mase = 0;
    static int add_swamp_max = 4;
    static int add_swamp_min = 1;
    static int number_of_river = 22;
    static int count_of_land = 0;
}

class elements
{
    static point* complete_river;
    static point center_of_swamp;
    static point medbat;
    static point arsenal;
    static point u_mouth;
    static point pit1, pit2, pit3, bum1, bum2, bum3;
    static player player_1;
    static player player_2;
    static point* arr_of_land_points;
    static char** lines;
    static point* river;
    elements();
    ~elements();
};
elements::elements()
{
    static point* complete_river = new point[30];
    static point center_of_swamp;
    static point medbat;
    static point arsenal;
    static point u_mouth;
    static point pit1, pit2, pit3, bum1, bum2, bum3;
    static player player_1;
    static player player_2;

    static point* arr_of_land_points = new point[64];
    static char** lines;
    static point* river = new point[sttng.number_of_river + 1];

}
elements::~elements()
{
    delete[] river;
    delete[] arr_of_land_points;
    for (int i = 0; i < 10; i++)
    {
        delete[] lines[i];
    }
    delete[] lines;
    delete[] complete_river;
}

class maze
{
public:


private:
    
};


/*
static bool command_changed = 0;
static bool treasure_picked = 0;
static point treasure;
static bool show_mase = 0;
static int turns_1 = 0;
static int turns_2 = 0;
static int time_in_swamp_1 = 0;
static int time_in_swamp_2 = 0;
static int k_along_bord = 1;
static int k_r_around = 2;
static int k_touch_walls = -4;
static int quadr_dist_between_pits = 7;
static int add_swamp_max = 4;
static int add_swamp_min = 1;
static unsigned seed = time(0);
static int number_of_river = 22;
static bool sucsess_river = 0;
static bool sucsess_bum = 0;
static bool sucsess_add_swamp = 0;
static int trash = 0;
static int count_of_land = 0;
static string command;
static string action;

static point* complete_river = new point[30];
static point center_of_swamp;
static point medbat;
static point arsenal;
static point u_mouth;
static point pit1, pit2, pit3, bum1, bum2, bum3;
static player player_1;
static player player_2;


static point* arr_of_land_points = new point[64];
static char** lines;
static river_path* paths = new river_path[2000000];
static point* river = new point[number_of_river + 1];
static int count_of_path = 0;
static long tries = 0;
static long min_path_index = 0;
*/

bool player_in_exit(player* pl)
{
    if ((*pl).coord.i == 9 or (*pl).coord.i == 0 or (*pl).coord.j == 9 or (*pl).coord.j == 0)
        return 1;
    else
        return 0;
}
int rand_from_a_to_b(int a, int b)
{
    techn.seed = (techn.seed * 73129 + 95121) % 10000;
    srand(techn.seed);
    return rand() % (b - a + 1) + a;
}

void visual_lab(char** elem.lines)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << elem.lines[i][j] << " ";
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
bool is_there_path(char** elem.lines, point elem.u_mouth)
{
    bool trig = 0;

    if (elem.u_mouth.j + 1 <= 8)
    {
        if (elem.lines[elem.u_mouth.i][elem.u_mouth.j + 1] == '.') trig = 1;
    }
    if (elem.u_mouth.j - 1 >= 1)
    {
        if (elem.lines[elem.u_mouth.i][elem.u_mouth.j - 1] == '.') trig = 1;
    }
    if (elem.u_mouth.i + 1 <= 8)
    {
        if (elem.lines[elem.u_mouth.i + 1][elem.u_mouth.j] == '.') trig = 1;
    }
    if (elem.u_mouth.i - 1 >= 1)
    {
        if (elem.lines[elem.u_mouth.i - 1][elem.u_mouth.j] == '.') trig = 1;
    }
    return trig;
}

int is_along_border(char** elem.lines, point p)
{
    if ((p.j + 1 > 8 and p.i - 1 < 1) or (p.j + 1 > 8 and p.i + 1 > 8) or (p.j - 1 < 1 and p.i - 1 < 1) or (p.j - 1 < 1 and p.i + 1 > 8)) return 10; // углы поля

    if (p.j + 1 > 8) return 1; // справа стена
    if (p.i - 1 < 1) return 2; // сверху
    if (p.j - 1 < 1) return 3; // слева
    if (p.i + 1 > 8) return 4; // снизу
    return 0;
}

int count_of_swamp_around(char** elem.lines, point p)
{

    int count_of_s = 0;
    if (p.j + 1 <= 8)
    {
        if (elem.lines[p.i][p.j + 1] == 's') count_of_s += 1; // справа
    }

    if (p.i + 1 <= 8)
    {
        if (elem.lines[p.i + 1][p.j] == 's') count_of_s += 1; // снизу
    }

    if (p.i - 1 >= 1)
    {
        if (elem.lines[p.i - 1][p.j] == 's') count_of_s += 1; // сверху
    }

    if (p.j - 1 >= 1)
    {
        if (elem.lines[p.i][p.j - 1] == 's') count_of_s += 1; // слева
    }
    return count_of_s;
}
int count_of_r_around(char** elem.lines, point p)
{

    int count_of_r = 0;
    if (p.j + 1 <= 8)
    {
        if (elem.lines[p.i][p.j + 1] == 'r') count_of_r += 1; // справа
    }

    if (p.i + 1 <= 8)
    {
        if (elem.lines[p.i + 1][p.j] == 'r') count_of_r += 1; // снизу
    }

    if (p.i - 1 >= 1)
    {
        if (elem.lines[p.i - 1][p.j] == 'r') count_of_r += 1; // сверху
    }

    if (p.j - 1 >= 1)
    {
        if (elem.lines[p.i][p.j - 1] == 'r') count_of_r += 1; // слева
    }
    return count_of_r;
}

int dist_quadr(point a, point b)
{
    return (b.i - a.i) * (b.i - a.i) + (b.j - a.j) * (b.j - a.j);
}

int number_of_land(char** elem.lines, point* arr_of_p, int count_of_t)
{
    int count_of_possible;
    //int next_point_index;

    point* possible_points = new point[4]; // определение возможных клеток, куда дальше может течь река
    count_of_possible = 0;

    if (arr_of_p[count_of_t - 1].j + 1 <= 8)
    {
        if (elem.lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j + 1] == '.' or
            elem.lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j + 1] == 'c')
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i;
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j + 1; // шаг вправо
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].i + 1 <= 8)
    {
        if (elem.lines[arr_of_p[count_of_t - 1].i + 1][arr_of_p[count_of_t - 1].j] == '.' or
            elem.lines[arr_of_p[count_of_t - 1].i + 1][arr_of_p[count_of_t - 1].j] == 'c')
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i + 1; // шаг вниз
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j;
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].j - 1 >= 1)
    {
        if (elem.lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j - 1] == '.' or
            elem.lines[arr_of_p[count_of_t - 1].i][arr_of_p[count_of_t - 1].j - 1] == 'c') // шаг влево
        {
            possible_points[count_of_possible].i = arr_of_p[count_of_t - 1].i;
            possible_points[count_of_possible].j = arr_of_p[count_of_t - 1].j - 1;
            count_of_possible += 1;
        }
    }
    if (arr_of_p[count_of_t - 1].i - 1 >= 1)
    {
        if (elem.lines[arr_of_p[count_of_t - 1].i - 1][arr_of_p[count_of_t - 1].j] == '.' or
            elem.lines[arr_of_p[count_of_t - 1].i - 1][arr_of_p[count_of_t - 1].j] == 'c')
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
        elem.lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = 't';
        // visual_lab(elem.lines); чтобы видеть как проверяется суша
        if (not is_point_in_arr(elem.arr_of_land_points, possible_points[next_point_index], 64))
        {
            sttng.count_of_land += 1;
            elem.arr_of_land_points[sttng.count_of_land - 1] = possible_points[next_point_index];
        }
        arr_of_p[count_of_t] = possible_points[next_point_index];
        techn.trash = number_of_land(elem.lines, arr_of_p, count_of_t + 1);
        elem.lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}
int number_of_land_complitely(char** elem.lines)
{
    int maxx = 0;
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (elem.lines[i][j] == '.' or elem.lines[i][j] == 'c')
            {
                point* arr_of_p = new point[64];
                int count_of_t = 1;
                point p;
                p.i = i;
                p.j = j;
                arr_of_p[0] = p;
                sttng.count_of_land = 0;
                for (int k = 0; k < 64; k++)
                {
                    elem.arr_of_land_points[k].i = -1;
                    elem.arr_of_land_points[k].j = -1;
                }
                techn.trash = number_of_land(elem.lines, arr_of_p, count_of_t);
                delete[] arr_of_p;
                if (sttng.count_of_land > maxx) maxx = sttng.count_of_land;
            }
        }
    }
    return maxx;
}

bool create_elem.river(char** elem.lines, point* elem.river, int count_of_elem.river) // создание массива со всеми возможными путями и их характеристиками
{

    if (count_of_elem.river == sttng.number_of_elem.river) // условие того, что река завершена и завершена на границе лабиринта (опционально)
    {
        // if(number_of_land_complitely(elem.lines) < 20) - как выяснилось тогда тратиться овердохера времени и памяти (7Гб и минут 5 ожидания)
        if (is_along_border(elem.lines, elem.river[sttng.number_of_elem.river - 1]) != 0)
        {
            techn.count_of_path += 1;
            techn.paths[techn.count_of_path - 1].elem.river = new point[sttng.number_of_elem.river];
            //visual_lab(elem.lines);
            techn.paths[techn.count_of_path - 1].count_of_r_around_summ = 0;
            techn.paths[techn.count_of_path - 1].count_elem.river_along_bord = 0;
            techn.paths[techn.count_of_path - 1].numb_toched_walls = 0;
            techn.paths[techn.count_of_path - 1].toched_walls = new int[20];
            for (int i = 0; i < 5; i++) techn.paths[techn.count_of_path - 1].toched_walls[i] = 0;

            for (int k = 0; k < sttng.number_of_elem.river; k++)
            {
                techn.paths[techn.count_of_path - 1].elem.river[k] = elem.river[k];
                techn.paths[techn.count_of_path - 1].count_of_r_around_summ += count_of_r_around(elem.lines, techn.paths[techn.count_of_path - 1].elem.river[k]);
                if (is_along_border(elem.lines, elem.river[k]) != 0)
                {
                    techn.paths[techn.count_of_path - 1].count_elem.river_along_bord += 1;
                    techn.paths[techn.count_of_path - 1].toched_walls[is_along_border(elem.lines, elem.river[k])] = 1;
                }
            }
            for (int i = 0; i < 5; i++)
            {
                techn.paths[techn.count_of_path - 1].numb_toched_walls += techn.paths[techn.count_of_path - 1].toched_walls[i];
            }


            techn.tries += 1;
            delete[] techn.paths[techn.count_of_path - 1].toched_walls;

        }

        return 0;

    }
    int count_of_possible;
    int next_elem.river_point_index;

    point* possible_points = new point[3]; // определение возможных клеток, куда дальше может течь река
    count_of_possible = 0;
    if (elem.river[count_of_elem.river - 1].j + 1 <= 8)
    {
        if (elem.lines[elem.river[count_of_elem.river - 1].i][elem.river[count_of_elem.river - 1].j + 1] == '.')
        {
            possible_points[count_of_possible].i = elem.river[count_of_elem.river - 1].i;
            possible_points[count_of_possible].j = elem.river[count_of_elem.river - 1].j + 1; // шаг вправо
            count_of_possible += 1;
        }
    }

    if (elem.river[count_of_elem.river - 1].i + 1 <= 8)
    {
        if (elem.lines[elem.river[count_of_elem.river - 1].i + 1][elem.river[count_of_elem.river - 1].j] == '.')
        {
            possible_points[count_of_possible].i = elem.river[count_of_elem.river - 1].i + 1; // шаг вниз
            possible_points[count_of_possible].j = elem.river[count_of_elem.river - 1].j;
            count_of_possible += 1;
        }
    }
    if (elem.river[count_of_elem.river - 1].j - 1 >= 1)
    {
        if (elem.lines[elem.river[count_of_elem.river - 1].i][elem.river[count_of_elem.river - 1].j - 1] == '.') // шаг влево
        {
            possible_points[count_of_possible].i = elem.river[count_of_elem.river - 1].i;
            possible_points[count_of_possible].j = elem.river[count_of_elem.river - 1].j - 1;
            count_of_possible += 1;
        }
    }
    if (elem.river[count_of_elem.river - 1].i - 1 >= 1)
    {
        if (elem.lines[elem.river[count_of_elem.river - 1].i - 1][elem.river[count_of_elem.river - 1].j] == '.')
        {
            possible_points[count_of_possible].i = elem.river[count_of_elem.river - 1].i - 1;
            possible_points[count_of_possible].j = elem.river[count_of_elem.river - 1].j; // шаг вверх
            count_of_possible += 1;
        }
    }
    if (count_of_possible == 0)
    {
        delete[] possible_points;
        return 0;
    }
    for (int next_elem.river_point_index = 0; next_elem.river_point_index < count_of_possible; next_elem.river_point_index++)
    {
        elem.lines[possible_points[next_elem.river_point_index].i][possible_points[next_elem.river_point_index].j] = 'r';
        // visual_lab(elem.lines); чтобы видеть как строится река
        elem.river[count_of_elem.river] = possible_points[next_elem.river_point_index];
        techn.trash = create_elem.river(elem.lines, elem.river, count_of_elem.river + 1);
        elem.lines[possible_points[next_elem.river_point_index].i][possible_points[next_elem.river_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}

bool create_bum(char** elem.lines)
{
    point* possible = new point[64];
    int count_poss = 0;
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (elem.lines[i][j] == '.')
            {
                possible[count_poss].i = i;
                possible[count_poss].j = j;
                count_poss += 1;
            }
        }
    }
    point elem.bum1;
    point elem.bum2;
    //point elem.bum3;
    int repeat = 0;
    do
    {
        if (repeat != 0)
        {
            elem.lines[elem.bum1.i][elem.bum1.j] = '.';
            elem.lines[elem.bum2.i][elem.bum2.j] = '.';
            //elem.lines[elem.bum3.i][elem.bum3.j] = '.';
        }
        elem.bum1 = possible[rand_from_a_to_b(0, count_poss - 1)];
        elem.bum2 = possible[rand_from_a_to_b(0, count_poss - 1)];
        //elem.bum3 = possible[rand_from_a_to_b(0,count_poss-1)];
        while (elem.bum2.i == elem.bum1.i and elem.bum2.j == elem.bum1.j) elem.bum2 = possible[rand_from_a_to_b(0, count_poss - 1)]; // выбираю точку так, чтобы оба бума не попали в одну
        //while((elem.bum3.i == elem.bum1.i and elem.bum3.j == elem.bum1.j) or (elem.bum3.i == elem.bum2.i and elem.bum3.j == elem.bum2.j)) elem.bum3 = possible[rand_from_a_to_b(0,count_poss-1)]; // аналогично
        elem.lines[elem.bum1.i][elem.bum1.j] = 'b';
        elem.lines[elem.bum2.i][elem.bum2.j] = 'b';
        //elem.lines[elem.bum3.i][elem.bum3.j] = 'b';
        repeat += 1;
    } while (false);

    delete[] possible;
    return 1;
}
void create_pits(char** elem.lines)
{
    do
    {
        elem.pit1.i = rand_from_a_to_b(1, 8);
        elem.pit1.j = rand_from_a_to_b(1, 8);

        elem.pit2.i = rand_from_a_to_b(1, 8);
        elem.pit2.j = rand_from_a_to_b(1, 8);

        elem.pit3.i = rand_from_a_to_b(1, 8);
        elem.pit3.j = rand_from_a_to_b(1, 8);
        //
    } while (not (elem.lines[elem.pit1.i][elem.pit1.j] == '.' and elem.lines[elem.pit2.i][elem.pit2.j] == '.' and elem.lines[elem.pit3.i][elem.pit3.j] == '.' and
        dist_quadr(elem.pit1, elem.pit2) >= techn.quadr_dist_between_pits and dist_quadr(elem.pit1, elem.pit3) >= techn.quadr_dist_between_pits and dist_quadr(elem.pit2, elem.pit3) >= techn.quadr_dist_between_pits and
        dist_quadr(elem.pit1, elem.u_mouth) > 1 and dist_quadr(elem.u_mouth, elem.pit3) > 1 and dist_quadr(elem.pit2, elem.u_mouth) > 1 and
        dist_quadr(elem.pit1, elem.medbat) > 1 and dist_quadr(elem.medbat, elem.pit3) > 1 and dist_quadr(elem.pit2, elem.medbat) > 1));// and
    //number_of_land_complitely(elem.lines) < 100
    elem.lines[elem.pit1.i][elem.pit1.j] = '1';
    elem.lines[elem.pit2.i][elem.pit2.j] = '2';
    elem.lines[elem.pit3.i][elem.pit3.j] = '3';

}

bool create_elem.river_complitely(char** elem.lines, point elem.u_mouth)
{
    elem.river[0] = elem.u_mouth;
    techn.count_of_path = 0;
    techn.sucsess_elem.river = create_elem.river(elem.lines, elem.river, 1);
    if (techn.count_of_path == 0) return 0;
    long minn = 10000000;
    techn.min_path_index = 0;

    for (int k = 0; k < techn.count_of_path; k++)
    {
        if (techn.k_r_around * techn.paths[k].count_of_r_around_summ + techn.k_along_bord * techn.paths[k].count_elem.river_along_bord + techn.k_touch_walls * techn.paths[k].numb_toched_walls < minn)
        {
            minn = techn.k_r_around * techn.paths[k].count_of_r_around_summ + techn.k_along_bord * techn.paths[k].count_elem.river_along_bord + techn.k_touch_walls * techn.paths[k].numb_toched_walls;
            techn.min_path_index = k;
        }
    }

    for (int k = 0; k < sttng.number_of_elem.river; k++)
    {
        elem.lines[techn.paths[techn.min_path_index].elem.river[k].i][techn.paths[techn.min_path_index].elem.river[k].j] = 'r';
        elem.complete_river[k].i = techn.paths[techn.min_path_index].elem.river[k].i;
        elem.complete_river[k].j = techn.paths[techn.min_path_index].elem.river[k].j;
    }
    elem.lines[elem.u_mouth.i][elem.u_mouth.j] = 'u';

    return 1;
}
void create_ars_and_med(char** elem.lines)
{
    do
    {
        elem.arsenal.i = rand_from_a_to_b(1, 8);
        elem.arsenal.j = rand_from_a_to_b(1, 8);

        elem.medbat.i = rand_from_a_to_b(2, 7);
        elem.medbat.j = rand_from_a_to_b(2, 7);

    } while (not (elem.lines[elem.arsenal.i][elem.arsenal.j] == '.' and elem.lines[elem.medbat.i][elem.medbat.j] == '.' and
        dist_quadr(elem.medbat, elem.arsenal) >= 9 and dist_quadr(elem.medbat, elem.u_mouth) >= 9 and dist_quadr(elem.u_mouth, elem.arsenal) >= 9));

    elem.lines[elem.arsenal.i][elem.arsenal.j] = 'A';
    elem.lines[elem.medbat.i][elem.medbat.j] = 'M';
}
void create_elem.u_mouth_and_swamp(char** elem.lines)
{
    elem.center_of_swamp.i = rand_from_a_to_b(3, 6);
    elem.center_of_swamp.j = rand_from_a_to_b(3, 6); // рандомно определили центр болота
    for (int n = -1; n <= 1; n++)
    {
        for (int m = -1; m <= 1; m++)
        {
            elem.lines[elem.center_of_swamp.i + n][elem.center_of_swamp.j + m] = 's';
        }
    }
    elem.u_mouth.i = rand_from_a_to_b(elem.center_of_swamp.i - 1, elem.center_of_swamp.i + 1);
    elem.u_mouth.j = rand_from_a_to_b(elem.center_of_swamp.j - 1, elem.center_of_swamp.j + 1);
    while ((elem.u_mouth.i == elem.center_of_swamp.i and elem.u_mouth.j == elem.center_of_swamp.j) or not (is_there_path(elem.lines, elem.u_mouth)))
    {
        elem.u_mouth.i = rand_from_a_to_b(elem.center_of_swamp.i - 1, elem.center_of_swamp.i + 1);
        elem.u_mouth.j = rand_from_a_to_b(elem.center_of_swamp.j - 1, elem.center_of_swamp.j + 1);
    }
    elem.lines[elem.u_mouth.i][elem.u_mouth.j] = 'u';
}
bool create_add_swamp(char** elem.lines)
{
    int number_of_add_swamp = rand_from_a_to_b(sttng.add_swamp_min, sttng.add_swamp_max);
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
            if (elem.lines[i][j] == '.' and count_of_swamp_around(elem.lines, p) >= 1)
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
            elem.lines[s.i][s.j] = 's'; //
        }  //
    }

    delete[] added_swamp;
    delete[] possible;
    return 1;
}
void create_land(char** elem.lines)
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (elem.lines[i][j] == '.') elem.lines[i][j] = 'c';
        }
    }
}


void create_techn.treasure(char** elem.lines)
{
    int count_of_possible = 0;
    point* possible = new point[64];
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            if (elem.lines[i][j] == 'c')
            {
                count_of_possible += 1;
                possible[count_of_possible - 1].i = i;
                possible[count_of_possible - 1].j = j;
            }

        }
    }
    int index = rand_from_a_to_b(0, count_of_possible - 1);
    techn.treasure = possible[index];
    delete[] possible;
}
void death(char** elem.lines, player* player)
{
    (*player).hp = 2;

    (*player).granade = 0;
    (*player).bullets = 0;
    if ((*player).with_techn.treasure == 1)
    {
        techn.treasure_picked = 0;
        cout << (*player).name << " выронил клад" << endl;
        (*player).with_techn.treasure = 0;
        if (elem.lines[(*player).coord.i][(*player).coord.j] == 's' or elem.lines[(*player).coord.i][(*player).coord.j] == 'u')
        {
            create_techn.treasure(elem.lines);
            cout << "Крокодил съел клад месте с " << (*player).name << " и высрал его в другом месте" << endl;
        }
    }
    (*player).coord = elem.medbat;
    cout << (*player).name << " умер и появился в медсанбате" << endl;
}

void create_exit(char** elem.lines)
{
    int y_left, y_right, x_up, x_down;
    y_left = rand_from_a_to_b(1, 8);
    y_right = rand_from_a_to_b(1, 8);
    x_up = rand_from_a_to_b(1, 8);
    x_down = rand_from_a_to_b(1, 8);
    elem.lines[y_left][0] = 'E';
    elem.lines[y_right][9] = 'E';
    elem.lines[0][x_up] = 'E';
    elem.lines[9][x_down] = 'E';

}
char** create_labirint()
{
    techn.tries = 0;
    do
    {
        if (techn.tries != 0)
        {
            for (int i = 0; i < 10; i++)
            {
                delete[] elem.lines[i];
            }
            delete[] elem.lines;
            for (int k = 0; k < techn.count_of_path; k++)
            {
                delete[] techn.paths[k].elem.river;
            }
        }
        elem.lines = init_arr(elem.lines);
        create_elem.u_mouth_and_swamp(elem.lines); // сделали болото и рандомно определили в нем устье

        create_ars_and_med(elem.lines); // теперь нужно добавить арсенал и медсанбат на расстоянии^2 не менее 17
        create_pits(elem.lines); //  Теперь нужно сделать ямы по такому же принципу
        techn.sucsess_elem.river = create_elem.river_complitely(elem.lines, elem.u_mouth); // Теперь сделаем реку - порядка 22 последовательных клеток
        techn.sucsess_bum = create_bum(elem.lines);
        techn.sucsess_add_swamp = create_add_swamp(elem.lines);

    } while (not(techn.sucsess_elem.river == 1 and number_of_land_complitely(elem.lines) < 10));

    cout << "number_of_land = " << number_of_land_complitely(elem.lines) << endl;
    cout << "techn.min_path_index = " << techn.min_path_index << endl;

    cout << "min_number = " << techn.k_r_around * techn.paths[techn.min_path_index].count_of_r_around_summ << " + " <<
        techn.k_along_bord * techn.paths[techn.min_path_index].count_elem.river_along_bord << " + " <<
        techn.k_touch_walls * techn.paths[techn.min_path_index].numb_toched_walls << endl;

    cout << "techn.paths[techn.min_path_index].elem.river[sttng.number_of_elem.river] i and j = " << techn.paths[techn.min_path_index].elem.river[sttng.number_of_elem.river - 1].i << " " << techn.paths[techn.min_path_index].elem.river[sttng.number_of_elem.river - 1].j << endl;
    cout << "count_of_techn.paths = " << techn.count_of_path << endl;


    create_land(elem.lines);
    create_exit(elem.lines);
    return elem.lines;
}

bool shoot(char** elem.lines, player* victim, player* hunter, string cmd)
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
                if ((*victim).hp == 0) death(elem.lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_techn.treasure == 1)
                {
                    (*victim).with_techn.treasure = 0;
                    techn.treasure_picked = 0;
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
                if ((*victim).hp == 0) death(elem.lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_techn.treasure == 1)
                {
                    (*victim).with_techn.treasure = 0;
                    techn.treasure_picked = 0;
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
                if ((*victim).hp == 0) death(elem.lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_techn.treasure == 1)
                {
                    (*victim).with_techn.treasure = 0;
                    techn.treasure_picked = 0;
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
                if ((*victim).hp == 0) death(elem.lines, victim);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_techn.treasure == 1)
                {
                    (*victim).with_techn.treasure = 0;
                    techn.treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
        }
    }
    cout << "Мимо" << endl;
    return 0;
}
bool explode(char** elem.lines, player* player, string cmd)
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
            if (elem.lines[i - 1][j] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                elem.lines[i - 1][j] = 'c';
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
            if (elem.lines[i + 1][j] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                elem.lines[i + 1][j] = 'c';
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
            if (elem.lines[i][j - 1] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                elem.lines[i][j - 1] = 'c';
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
            if (elem.lines[i][j + 1] == 'b')
            {
                cout << "Стена взорвалась" << endl;
                cout << "У тебя осталось " << (*player).granade << " гранат" << endl;
                elem.lines[i][j + 1] = 'c';
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
    techn.command_changed = 0;
    if (player_in_exit(player) and turns != 0) // если человек находится "в выходе"
    {
        cout << "Игрок " << (*player).name << " заходит обратно в лабиринт" << endl;
        if ((*player).coord.i == 9) techn.command = "Up";
        if ((*player).coord.i == 0) techn.command = "Down";
        if ((*player).coord.j == 9) techn.command = "Left"; // определяем куда надо шагнуть обратно
        if ((*player).coord.j == 0) techn.command = "Right";
        techn.command_changed = 1;
    }
    if ((*player).coord.i == techn.treasure.i and (*player).coord.j == techn.treasure.j // на случай если сокровище заспавнилось прямо под игроком
        and (*player).with_techn.treasure == 0 and (*player).hp == 2)
    {
        cout << (*player).name << " обнаружил под собой клад. Берешь его?" << endl;
        cin >> techn.action;
        if (techn.action == "Yes" or techn.action == "y" or techn.action == "Y") (*player).with_techn.treasure = 1;
    }
    if ((*player).coord.i == techn.treasure.i and (*player).coord.j == techn.treasure.j
        and (*player).with_techn.treasure == 0 and (*player).hp != 2)
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
        while (elem.lines[(*player).coord.i][(*player).coord.j] == 'b')
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
        techn.command = "Up";
        techn.command_changed = 1;
    }
    if (not(techn.command_changed)) cin >> techn.command; // P.s. techn.command это static string обьявленный в самом начале
    if (techn.command == "Show_maze")
    {
        sttng.show_mase = 1;

        temp_1 = elem.lines[elem.player_1.coord.i][elem.player_1.coord.j];
        temp_2 = elem.lines[elem.player_2.coord.i][elem.player_2.coord.j];
        temp_3 = elem.lines[techn.treasure.i][techn.treasure.j];
        elem.lines[elem.player_1.coord.i][elem.player_1.coord.j] = 'O'; // Нанести игрока на карту лабиринта временно (чтобы было видно где он)
        elem.lines[techn.treasure.i][techn.treasure.j] = 't';
        elem.lines[elem.player_2.coord.i][elem.player_2.coord.j] = 'T';

        visual_lab(elem.lines);
        cout << endl;
        // if(sttng.show_mase) visual_lab(elem.lines);                  // Вывести лабиринт
        elem.lines[elem.player_1.coord.i][elem.player_1.coord.j] = temp_1; // Вернуть в точку в лабиринте то что было
        elem.lines[elem.player_2.coord.i][elem.player_2.coord.j] = temp_2;
        elem.lines[techn.treasure.i][techn.treasure.j] = temp_3;
        cin >> techn.command;
    }
    if (techn.command == "Hide_maze")
    {
        sttng.show_mase = 0;
        cin >> techn.command;
    }
    if (techn.command == "Stop" or techn.command == "stop") return 1;


    bool trig = 0;
    while (not(techn.command == "Up" or techn.command == "Down" or techn.command == "Left" or techn.command == "Right"
        or techn.command == "Show_maze" or techn.command == "Shoot" or techn.command == "Explode"))
    {
        cout << "Херню какую то ввел, давай нормальную команду 819 строка";
        cin >> techn.command;
    }
    if (techn.command == "Up") (*player).coord.i -= 1;

    if (techn.command == "Down") (*player).coord.i += 1;

    if (techn.command == "Right") (*player).coord.j += 1;

    if (techn.command == "Left") (*player).coord.j -= 1;

    if ((*player).coord.i == techn.treasure.i and (*player).coord.j == techn.treasure.j and techn.treasure_picked != 1 and (*player).hp == 2) // если нашел клад
    {
        cout << (*player).name << " нашел клад. Берешь его?" << endl;
        cin >> techn.action;
        if (techn.action == "Yes" or techn.action == "y" or techn.action == "Y")
        {
            (*player).with_techn.treasure = 1;
            techn.treasure_picked = 1;
        }
    }

    if ((*player).coord.i == techn.treasure.i and (*player).coord.j == techn.treasure.j and techn.treasure_picked != 1 and (*player).hp < 2)
    {
        cout << (*player).name << " нашел клад но не может его взять" << endl;
    }

    if ((*player).with_techn.treasure)
    {
        techn.treasure.i = (*player).coord.i;
        techn.treasure.j = (*player).coord.j;
    }
    if (techn.command == "Shoot")
    {
        cout << "Куда стреляешь?" << endl;
        cin >> techn.command;
        if (id == 10) // Тогда охотник - первый игрок (И сейчас ход 1 игрока)
        {
            techn.trash = shoot(elem.lines, &elem.player_2, player, techn.command);
            return 1;
        }
        if (id == 20) // Тогда охотник - второй игрок (И сейчас ход 2 игрока)
        {
            techn.trash = shoot(elem.lines, &elem.player_1, player, techn.command);
            return 1;
        }
    }
    if (techn.command == "Explode")
    {
        cout << "Куда бросаешь гранату?" << endl;
        cin >> techn.action;
        explode(elem.lines, player, techn.action);
        return 1;
    }
    /*if(trig == 0) // кажется можно удалить т.к. trig = 0 когда челик никуда не ушел (раньше код был такой)
    {
        cout << "Бум"<< endl;
        if(elem.lines[(*player).coord.i][(*player).coord.j] == 's')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                death(elem.lines, player );
                (*time_in_swamp) = 0;
                return 1;
            }
            return 1;
        }
        return 1;
    } */

    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'r')
    {
        (*time_in_swamp) = 0;
        for (int k = 0; k < sttng.number_of_elem.river; k++)
        {
            if ((*player).coord.i == elem.complete_river[k].i and (*player).coord.j == elem.complete_river[k].j)
            {

                if (k - 3 > 0)
                {
                    (*player).coord = elem.complete_river[k - 3];
                    cout << "Река понесла" << endl;
                    return 1;
                    break;
                }
                else
                {
                    (*player).coord = elem.u_mouth;
                    cout << "Река в устье принесла" << endl;
                    (*time_in_swamp) += 1;
                    cout << "Болото рычит " << (*time_in_swamp) << " раз";
                    return 1;
                    break;

                }
            }
        }
    }

    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'c')
    {
        (*time_in_swamp) = 0;
        cout << "Суша" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'u')
    {
        cout << "Пришел в устье" << endl;
        (*time_in_swamp) += 1;
        cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
        if ((*time_in_swamp) == 3)
        {
            cout << "Покусан крокодилом, отправился в медсанбат" << endl;
            death(elem.lines, player);
            (*time_in_swamp) = 0;
            return 1;
        }
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'M')
    {
        (*player).hp = 2;
        (*time_in_swamp) = 0;
        cout << "Медсанбат" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'A')
    {
        (*player).bullets = 3;
        (*player).granade = 2;
        (*time_in_swamp) = 0;
        cout << "Арсенал" << endl;
        cout << "Ты получил 3 патрона и 2 гранаты" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == '1')
    {
        (*time_in_swamp) = 0;
        (*player).coord = elem.pit2;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == '2')
    {
        (*time_in_swamp) = 0;
        (*player).coord = elem.pit3;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == '3')
    {
        (*time_in_swamp) = 0;
        (*player).coord = elem.pit1;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 's')
    {
        (*time_in_swamp) += 1;
        cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
        if ((*time_in_swamp) == 3)
        {
            cout << "Покусан крокодилом, отправился в медсанбат" << endl;
            death(elem.lines, player);
            (*time_in_swamp) = 0;
            return 1;
        }
        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'b' or
        elem.lines[(*player).coord.i][(*player).coord.j] == 'w')
    {
        cout << "Бум" << endl;

        if (techn.command == "Up") (*player).coord.i += 1;
        if (techn.command == "Down") (*player).coord.i -= 1;
        if (techn.command == "Right") (*player).coord.j -= 1;
        if (techn.command == "Left") (*player).coord.j += 1;

        if ((*player).with_techn.treasure)
        {
            techn.treasure.i = (*player).coord.i;
            techn.treasure.j = (*player).coord.j;
        }

        if (elem.lines[(*player).coord.i][(*player).coord.j] == 's' or elem.lines[(*player).coord.i][(*player).coord.j] == 'u')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                (*time_in_swamp) = 0;
                death(elem.lines, player);
                return 1;
            }

        }

        return 1;
    }
    if (elem.lines[(*player).coord.i][(*player).coord.j] == 'E')
    {
        if ((*player).with_techn.treasure)
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


int main()
{
    tech techn;
    settings sttng;
    elements elem;
    char temp_1;
    char temp_2;
    char temp_3;
    techn.techn.action = "";

    elem.player_1.hp = 2;
    elem.player_2.hp = 2;
    elem.player_1.with_techn.treasure = 0;
    elem.player_2.with_techn.treasure = 0;
    elem.player_1.bullets = 0;
    elem.player_2.bullets = 0;
    elem.player_1.granade = 0;
    elem.player_2.granade = 0;
    elem.player_1.id = 10;
    elem.player_2.id = 20;
    elem.player_1.name = "Первый игрок";
    elem.player_2.name = "Второй игрок";

    char** elem.lines;
    setlocale(0, "Russian");
    string key = "";
    /*cin >> key;

     while(key != "0") // чтобы посмотреть какие лабиринты строятся
    {
        elem.lines = create_labirint();
        visual_lab(elem.lines);
        for(int i = 0; i < 8; i++)
        {
            delete[] elem.lines[i];
        }
        delete[] elem.lines;
        for(int k = 0; k < techn.count_of_path; k++)
        {
            delete[] techn.paths[k].elem.river;
        }
        cin >> key;
    } */

    // Начинаются ходы игроков
    cout << "Показывать лабиринт? Напиши Yes если да, если нет, напиши что угодно" << endl;
    cin >> techn.command;
    if (techn.command == "Yes" or techn.command == "yes" or techn.command == "y" or techn.command == "Y") sttng.show_mase = 1;
    cout << "Введите количество рек в лабиринте (до 24)" << endl;
    cin >> sttng.number_of_elem.river;
    do
    {
        elem.lines = create_labirint();
        create_techn.treasure(elem.lines);
        if (sttng.show_mase)
        {
            temp_3 = elem.lines[techn.treasure.i][techn.treasure.j];
            elem.lines[techn.treasure.i][techn.treasure.j] = 't';
            visual_lab(elem.lines);
            cout << endl;
            elem.lines[techn.treasure.i][techn.treasure.j] = temp_3;
        }
        cout << "Устраивает лабиринт?" << endl;
        cin >> key;

    } while (not(key == "Yes" or key == "yes" or key == "y" or key == "yep"));



    int repets = 0;

    while (techn.command != "Stop" and techn.command != "stop")
    {
        cout << "Ход первого игрока:" << endl;
        techn.trash = step(&elem.player_1, techn.turns_1, &techn.time_in_swamp_1, elem.player_1.id); // Ход 1 игрока
        while (techn.trash != 1)
        {
            cout << "Херню какую то ввел, давай нормальную команду" << endl;
            techn.trash = step(&elem.player_1, techn.turns_1, &techn.time_in_swamp_1, elem.player_1.id);
        }
        cout << endl;
        cout << "Ход второго игрока:" << endl;
        techn.trash = step(&elem.player_2, techn.turns_2, &techn.time_in_swamp_2, elem.player_2.id); // Ход 2 игрока
        while (techn.trash != 1)
        {
            cout << "Херню какую то ввел, давай нормальную команду" << endl;
            techn.trash = step(&elem.player_2, techn.turns_2, &techn.time_in_swamp_2, elem.player_2.id);
        }
        cout << endl;

        techn.turns_1 += 1;
        techn.turns_2 += 1;
    }


    
    for (int k = 0; k < techn.count_of_path; k++)
    {
        delete[] techn.paths[k].elem.river;
    }


    


}
