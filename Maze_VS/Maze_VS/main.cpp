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
    int id = 0;
    point coord;
    bool with_treasure = 0;
    int hp = 2;
    int bullets = 0;
    int granade = 0;
};
struct crocodile
{
    bool alive;
    int time_to_res;
    point coord;
};



bool operator == (point a, point b) { return (a.i == b.i and a.j == b.j); }
bool operator != (point a, point b) { return not(a.i == b.i and a.j == b.j); }





class elements
{
public:
    point center_of_swamp;
    point treasure;
    point medbat;
    point arsenal;
    point u_mouth;
    point pit1, pit2, pit3, bum1, bum2, bum3; // по умолчанию два бума
    point* complete_river = new point[64]; // финальная река
};
class settings
{
public:
    bool respawn_new_place;
    int k_along_bord;
    int k_r_around;
    int k_touch_walls;
    int quadr_dist_between_pits;
    int add_swamp_max;
    int add_swamp_min;
    int number_of_river;
    settings()
    {
        respawn_new_place = 0; // если == 1, то крокодил перерождается каждый раз в рандомном месте
        k_along_bord = 1; // штраф за прилегание реки к стенам лабиринта
        k_r_around = 2; // штраф за то что река слишком много контактирует сама с собой
        k_touch_walls = -4; // поощрение за касание рекой одной из 4 стен лабиринта
        quadr_dist_between_pits = 7; // квадрат минимального расстояния между ямами
        add_swamp_max = 4; // диапозон добавочного болота
        add_swamp_min = 1;
        number_of_river = 22; // количество рек
    }
    
};
class technical
{
public:
    time_t seed = time(0);
    int time_in_swamp_1 = 0;
    int time_in_swamp_2 = 0;
    bool show_mase = 0;
    bool command_changed = 0;
    bool treasure_picked = 0;
    int turns_1 = 0;
    int turns_2 = 0;
    bool sucsess_river = 0;
    bool sucsess_bum = 0;
    bool sucsess_add_swamp = 0;
    int count_of_land = 0;
    string command;
    string action;
};


static point* arr_of_land_points = new point[64];
static char** lines;
static river_path* paths = new river_path[2000000];
static point* river = new point[64];
static int count_of_path = 0;
static long tries = 0;
static long min_path_index = 0;

int rand_from_a_to_b(int a, int b, technical* tech)
{
    tech->seed = (tech->seed * 73129 + 95121) % 10000;
    srand(tech->seed);
    return rand() % (b - a + 1) + a;
}



player init_player(int id)
{
    player pl;
    string nm;
    cout << "Введите имя игрока" << endl;
    cin >> nm;
    pl.name = nm;
    pl.id = id;
    pl.with_treasure = 0;
    pl.hp = 2;
    pl.bullets = 0;
    pl.granade = 0;
    return pl;
}

crocodile init_croc(char** lines, technical* tech)
{
    int count_of_swamp = 0;
    crocodile croc;
    point* swamp_points = new point[64];
    for (int i = 1; i <= 8; i++) // делаем список всех точек болота, чтобы потом рандомно выбрать одну из них как место где обитает крокодил
    {
        for (int j = 1; j <= 8; j++)
        {
            if (lines[i][j] == 's' or lines[i][j] == 'u')
            {
                swamp_points[count_of_swamp].i = i;
                swamp_points[count_of_swamp].j = j;
                count_of_swamp += 1;
            }
        }
    }
    croc.coord = swamp_points[rand_from_a_to_b(0, count_of_swamp - 1, tech)];
    croc.alive = 1;
    croc.time_to_res = 0;

    delete[] swamp_points;
    return croc;
}
bool player_in_exit(player* pl)
{
    if ((*pl).coord.i == 9 or (*pl).coord.i == 0 or (*pl).coord.j == 9 or (*pl).coord.j == 0)
        return 1;
    else
        return 0;
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
bool is_there_path(char** lines, elements* elem)
{
    bool trig = 0;

    if (elem->u_mouth.j + 1 <= 8)
    {
        if (lines[elem->u_mouth.i][elem->u_mouth.j + 1] == '.') trig = 1;
    }
    if (elem->u_mouth.j - 1 >= 1)
    {
        if (lines[elem->u_mouth.i][elem->u_mouth.j - 1] == '.') trig = 1;
    }
    if (elem->u_mouth.i + 1 <= 8)
    {
        if (lines[elem->u_mouth.i + 1][elem->u_mouth.j] == '.') trig = 1;
    }
    if (elem->u_mouth.i - 1 >= 1)
    {
        if (lines[elem->u_mouth.i - 1][elem->u_mouth.j] == '.') trig = 1;
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

int number_of_land(char** lines, point* arr_of_p, int count_of_t, technical* tech)
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
        if (not is_point_in_arr(arr_of_land_points, possible_points[next_point_index], 64))
        {
            tech->count_of_land += 1;
            arr_of_land_points[tech->count_of_land - 1] = possible_points[next_point_index];
        }
        arr_of_p[count_of_t] = possible_points[next_point_index];
        trash = number_of_land(lines, arr_of_p, count_of_t + 1, tech);
        lines[possible_points[next_point_index].i][possible_points[next_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}
int number_of_land_complitely(char** lines, technical* tech)
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
                tech->count_of_land = 0;
                for (int k = 0; k < 64; k++)
                {
                    arr_of_land_points[k].i = -1;
                    arr_of_land_points[k].j = -1;
                }
                trash = number_of_land(lines, arr_of_p, count_of_t, tech);
                delete[] arr_of_p;
                if (tech->count_of_land > maxx) maxx = tech->count_of_land;
            }
        }
    }
    return maxx;
}

bool create_river(char** lines, point* river, int count_of_river, settings* sett) // создание массива со всеми возможными путями и их характеристиками
{
    int trash = 0;

    if (count_of_river == sett->number_of_river) // условие того, что река завершена и завершена на границе лабиринта (опционально)
    {
        // if(number_of_land_complitely(lines) < 20) - как выяснилось тогда тратиться овердохера времени и памяти (7Гб и минут 5 ожидания)
        if (is_along_border(lines, river[sett->number_of_river - 1]) != 0)
        {
            count_of_path += 1;
            paths[count_of_path - 1].river = new point[sett->number_of_river];
            //visual_lab(lines);
            paths[count_of_path - 1].count_of_r_around_summ = 0;
            paths[count_of_path - 1].count_river_along_bord = 0;
            paths[count_of_path - 1].numb_toched_walls = 0;
            paths[count_of_path - 1].toched_walls = new int[20];
            for (int i = 0; i < 5; i++) paths[count_of_path - 1].toched_walls[i] = 0;

            for (int k = 0; k < sett->number_of_river; k++)
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
        trash = create_river(lines, river, count_of_river + 1, sett);
        lines[possible_points[next_river_point_index].i][possible_points[next_river_point_index].j] = '.';
    }
    delete[] possible_points;
    return 0;

}

bool create_bum(char** lines, technical* tech, elements* elem)
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
    //point bum3;
    int repeat = 0;
    do
    {
        if (repeat != 0)
        {
            lines[elem->bum1.i][elem->bum1.j] = '.';
            lines[elem->bum2.i][elem->bum2.j] = '.';
            //lines[elem->bum3.i][elem->bum3.j] = '.';
        }
        elem->bum1 = possible[rand_from_a_to_b(0, count_poss - 1, tech)];
        elem->bum2 = possible[rand_from_a_to_b(0, count_poss - 1, tech)];
        //elem->bum3 = possible[rand_from_a_to_b(0,count_poss-1)];
        while (elem->bum2.i == elem->bum1.i and elem->bum2.j == elem->bum1.j) elem->bum2 = possible[rand_from_a_to_b(0, count_poss - 1, tech)]; // выбираю точку так, чтобы оба бума не попали в одну
        //while((elem->bum3.i == elem->bum1.i and elem->bum3.j == elem->bum1.j) or (elem->bum3.i == elem->bum2.i and elem->bum3.j == elem->bum2.j)) elem->bum3 = possible[rand_from_a_to_b(0,count_poss-1)]; // аналогично
        lines[elem->bum1.i][elem->bum1.j] = 'b';
        lines[elem->bum2.i][elem->bum2.j] = 'b';
        //lines[elem->bum3.i][elem->bum3.j] = 'b';
        repeat += 1;
    } while (false);

    delete[] possible;
    return 1;
}
void create_pits(char** lines, settings* sett, technical* tech, elements* elem)
{
    do
    {
        elem->pit1.i = rand_from_a_to_b(1, 8, tech);
        elem->pit1.j = rand_from_a_to_b(1, 8, tech);

        elem->pit2.i = rand_from_a_to_b(1, 8, tech);
        elem->pit2.j = rand_from_a_to_b(1, 8, tech);

        elem->pit3.i = rand_from_a_to_b(1, 8, tech);
        elem->pit3.j = rand_from_a_to_b(1, 8, tech);
        //
    } while (not (lines[elem->pit1.i][elem->pit1.j] == '.' and lines[elem->pit2.i][elem->pit2.j] == '.' and lines[elem->pit3.i][elem->pit3.j] == '.' and
        dist_quadr(elem->pit1, elem->pit2) >= sett->quadr_dist_between_pits and dist_quadr(elem->pit1, elem->pit3) >= sett->quadr_dist_between_pits and dist_quadr(elem->pit2, elem->pit3) >= sett->quadr_dist_between_pits and
        dist_quadr(elem->pit1, elem->u_mouth) > 1 and dist_quadr(elem->u_mouth, elem->pit3) > 1 and dist_quadr(elem->pit2, elem->u_mouth) > 1 and
        dist_quadr(elem->pit1, elem->medbat) > 1 and dist_quadr(elem->medbat, elem->pit3) > 1 and dist_quadr(elem->pit2, elem->medbat) > 1));// and
    //number_of_land_complitely(lines) < 100
    lines[elem->pit1.i][elem->pit1.j] = '1';
    lines[elem->pit2.i][elem->pit2.j] = '2';
    lines[elem->pit3.i][elem->pit3.j] = '3';

}

bool create_river_complitely(char** lines, elements* elem, settings* sett, technical* tech)
{
    river[0] = elem->u_mouth;
    count_of_path = 0;
    tech->sucsess_river = create_river(lines, river, 1, sett);
    if (count_of_path == 0) return 0;
    long minn = 10000000;
    min_path_index = 0;

    for (int k = 0; k < count_of_path; k++)
    {
        if (sett->k_r_around * paths[k].count_of_r_around_summ + sett->k_along_bord * paths[k].count_river_along_bord + sett->k_touch_walls * paths[k].numb_toched_walls < minn)
        {
            minn = sett->k_r_around * paths[k].count_of_r_around_summ + sett->k_along_bord * paths[k].count_river_along_bord + sett->k_touch_walls * paths[k].numb_toched_walls;
            min_path_index = k;
        }
    }

    for (int k = 0; k < sett->number_of_river; k++)
    {
        lines[paths[min_path_index].river[k].i][paths[min_path_index].river[k].j] = 'r';
        elem->complete_river[k].i = paths[min_path_index].river[k].i;
        elem->complete_river[k].j = paths[min_path_index].river[k].j;
    }
    lines[elem->u_mouth.i][elem->u_mouth.j] = 'u';

    return 1;
}
void create_ars_and_med(char** lines, technical* tech, elements* elem)
{
    do
    {
        elem->arsenal.i = rand_from_a_to_b(1, 8, tech);
        elem->arsenal.j = rand_from_a_to_b(1, 8, tech);

        elem->medbat.i = rand_from_a_to_b(2, 7, tech);
        elem->medbat.j = rand_from_a_to_b(2, 7, tech);

    } while (not (lines[elem->arsenal.i][elem->arsenal.j] == '.' and lines[elem->medbat.i][elem->medbat.j] == '.' and
        dist_quadr(elem->medbat, elem->arsenal) >= 9 and dist_quadr(elem->medbat, elem->u_mouth) >= 9 and dist_quadr(elem->u_mouth, elem->arsenal) >= 9));

    lines[elem->arsenal.i][elem->arsenal.j] = 'A';
    lines[elem->medbat.i][elem->medbat.j] = 'M';
}
void create_u_mouth_and_swamp(char** lines, elements* elem, technical* tech)
{
    (*elem).center_of_swamp.i = rand_from_a_to_b(3, 6, tech);
    (*elem).center_of_swamp.j = rand_from_a_to_b(3, 6, tech); // рандомно определили центр болота
    for (int n = -1; n <= 1; n++)
    {
        for (int m = -1; m <= 1; m++)
        {
            lines[(*elem).center_of_swamp.i + n][(*elem).center_of_swamp.j + m] = 's';
        }
    }
    elem->u_mouth.i = rand_from_a_to_b((*elem).center_of_swamp.i - 1, (*elem).center_of_swamp.i + 1, tech);
    elem->u_mouth.j = rand_from_a_to_b((*elem).center_of_swamp.j - 1, (*elem).center_of_swamp.j + 1, tech);
    while ((elem->u_mouth.i == (*elem).center_of_swamp.i and elem->u_mouth.j == (*elem).center_of_swamp.j) or not (is_there_path(lines, elem)))
    {
        elem->u_mouth.i = rand_from_a_to_b((*elem).center_of_swamp.i - 1, (*elem).center_of_swamp.i + 1, tech);
        elem->u_mouth.j = rand_from_a_to_b((*elem).center_of_swamp.j - 1, (*elem).center_of_swamp.j + 1, tech);
    }
    lines[elem->u_mouth.i][elem->u_mouth.j] = 'u';
}
bool create_add_swamp(char** lines, settings* sett, technical* tech)
{
    int number_of_add_swamp = rand_from_a_to_b(sett->add_swamp_min, sett->add_swamp_max, tech);
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
        point s = possible[rand_from_a_to_b(0, count_of_possible_swamp - 1, tech)];
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


point create_treasure(char** lines, technical* tech)
{
    point treas;
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
    int index = rand_from_a_to_b(0, count_of_possible - 1, tech);
    treas = possible[index];
    delete[] possible;
    return treas;
}
void death(char** lines, player* player, technical* tech, elements* elem)
{
    (*player).hp = 2;

    (*player).granade = 0;
    (*player).bullets = 0;
    if ((*player).with_treasure == 1)
    {
        (*tech).treasure_picked = 0;
        cout << (*player).name << " выронил клад" << endl;
        (*player).with_treasure = 0;
        if (lines[(*player).coord.i][(*player).coord.j] == 's' or lines[(*player).coord.i][(*player).coord.j] == 'u')
        {
            create_treasure(lines, tech);
            cout << "Крокодил съел клад месте с " << (*player).name << " и высрал его в другом месте" << endl;
        }
    }
    (*player).coord = elem->medbat;
    cout << (*player).name << " умер и появился в медсанбате" << endl;
}

void create_exit(char** lines, technical* tech)
{
    int y_left, y_right, x_up, x_down;
    y_left = rand_from_a_to_b(1, 8, tech);
    y_right = rand_from_a_to_b(1, 8, tech);
    x_up = rand_from_a_to_b(1, 8, tech);
    x_down = rand_from_a_to_b(1, 8, tech);
    lines[y_left][0] = 'E';
    lines[y_right][9] = 'E';
    lines[0][x_up] = 'E';
    lines[9][x_down] = 'E';

}
char** create_labirint(elements* elem, settings* sett, technical* tech)
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
        create_u_mouth_and_swamp(lines, elem, tech); // сделали болото и рандомно определили в нем устье

        create_ars_and_med(lines, tech, elem); // теперь нужно добавить арсенал и медсанбат на расстоянии^2 не менее 17
        create_pits(lines, sett, tech, elem); //  Теперь нужно сделать ямы по такому же принципу
        tech->sucsess_river = create_river_complitely(lines, elem, sett, tech); // Теперь сделаем реку - порядка 22 последовательных клеток
        tech->sucsess_bum = create_bum(lines, tech, elem);
        tech->sucsess_add_swamp = create_add_swamp(lines, sett, tech);

    } while (not(tech->sucsess_river == 1 and number_of_land_complitely(lines, tech) < 10));

    cout << "number_of_land = " << number_of_land_complitely(lines, tech) << endl;
    cout << "min_path_index = " << min_path_index << endl;

    cout << "min_number = " << sett->k_r_around * paths[min_path_index].count_of_r_around_summ << " + " <<
        sett->k_along_bord * paths[min_path_index].count_river_along_bord << " + " <<
        sett->k_touch_walls * paths[min_path_index].numb_toched_walls << endl;

    cout << "paths[min_path_index].river[sett->number_of_river] i and j = " << paths[min_path_index].river[sett->number_of_river - 1].i << " " << paths[min_path_index].river[sett->number_of_river - 1].j << endl;
    cout << "count_of_paths = " << count_of_path << endl;


    create_land(lines);
    create_exit(lines, tech);
    return lines;
}

bool shoot(char** lines, player* victim, player* hunter, string cmd, crocodile* croc, technical* tech, elements* elem)
{
    if ((*hunter).bullets == 0)
    {
        cout << "Ты спускаешь курок и ничего не происходит. Ты потратил ход" << endl;
        return 1;
    }
    (*hunter).bullets -= 1;
    if ((*hunter).bullets == 2) cout << "У тебя осталось 2 патрона" << endl;
    if ((*hunter).bullets == 1) cout << "У тебя остался 1 патрон" << endl;
    if ((*hunter).bullets == 0) cout << "У тебя не осталось патронов" << endl;
    if (cmd == "Down") // добавлен крокодил
    {
        for (int k = (*hunter).coord.i; k <= 9; k++)
        {
            if (k == (*victim).coord.i and (*hunter).coord.j == (*victim).coord.j) // если попал в игрока
                                                                        // т.е. если игрок и крокодил на одной клетке, первым получит пулю игрок
            {
                (*victim).hp -= 1;
                if ((*victim).hp == 0) death(lines, victim, tech, elem);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    (*tech).treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
            if (k == (*croc).coord.i and (*hunter).coord.j == (*croc).coord.j and (*croc).alive) // если попал в крокодила
            {
                (*croc).alive = 0;
                (*croc).time_to_res = 3;
                cout << "Убил крокодила" << endl;
                cout << "До появления нового крокодила осталось 3 хода" << endl;
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
                if ((*victim).hp == 0) death(lines, victim, tech, elem);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    (*tech).treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
            if (k == (*croc).coord.i and (*hunter).coord.j == (*croc).coord.j and (*croc).alive) // если попал в крокодила
            {
                (*croc).alive = 0;
                (*croc).time_to_res = 3;
                cout << "Убил крокодила" << endl;
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
                if ((*victim).hp == 0) death(lines, victim, tech, elem);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    (*tech).treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
            if (k == (*croc).coord.j and (*hunter).coord.i == (*croc).coord.i and (*croc).alive) // если попал в крокодила
            {
                (*croc).alive = 0;
                (*croc).time_to_res = 3;
                cout << "Убил крокодила" << endl;
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
                if ((*victim).hp == 0) death(lines, victim, tech, elem);
                cout << "Попал в игрока" << endl;
                if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                {
                    (*victim).with_treasure = 0;
                    (*tech).treasure_picked = 0;
                    cout << "Игрок " << (*victim).name << " выронил клад" << endl;
                }
                return 1;
            }
            if (k == (*croc).coord.j and (*hunter).coord.i == (*croc).coord.i and (*croc).alive) // если попал в крокодила
            {
                (*croc).alive = 0;
                (*croc).time_to_res = 3;
                cout << "Убил крокодила" << endl;
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

bool step(player* pl, int turns, int* time_in_swamp, int id, crocodile* croc, player* pl1, player* pl2, technical* tech, elements* elem, settings* sett)
{
    (*tech).command_changed = 0;


    int trash = 0;
    char temp_1;
    char temp_2;
    char temp_3;
    char temp_4;
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
            (*pl).coord.j = x - '0';
            (*pl).coord.i = y - '0';

        } while (not(1 <= (*pl).coord.i and (*pl).coord.i <= 8 and
            1 <= (*pl).coord.j and (*pl).coord.j <= 8));
        while (lines[(*pl).coord.i][(*pl).coord.j] == 'b')
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
                (*pl).coord.j = x - '0';
                (*pl).coord.i = y - '0';

            } while (not(1 <= (*pl).coord.i and (*pl).coord.i <= 8 and
                1 <= (*pl).coord.j and (*pl).coord.j <= 8));
        }



        (*pl).coord.i += 1; // чтобы в нулевой ход игрок пришел на клетку, куда захотел высадиться
        tech->command = "Up";
        (*tech).command_changed = 1;
    }

    if (player_in_exit(pl) and turns != 0) // если человек находится "в выходе"
    {
        cout << "Игрок " << (*pl).name << " заходит обратно в лабиринт" << endl;
        if ((*pl).coord.i == 9) tech->command = "Up";
        if ((*pl).coord.i == 0) tech->command = "Down";
        if ((*pl).coord.j == 9) tech->command = "Left"; // определяем куда надо шагнуть обратно
        if ((*pl).coord.j == 0) tech->command = "Right";
        (*tech).command_changed = 1;
    }

    if ((*pl).coord.i == (*elem).treasure.i and (*pl).coord.j == (*elem).treasure.j // на случай если сокровище заспавнилось прямо под игроком
        and (*pl).with_treasure == 0 and (*pl).hp == 2)
    {
        cout << (*pl).name << " обнаружил под собой клад. Берешь его?" << endl;
        cin >> tech->action;
        if (tech->action == "Yes" or tech->action == "y" or tech->action == "Y") (*pl).with_treasure = 1;
    }
    if ((*pl).coord.i == (*elem).treasure.i and (*pl).coord.j == (*elem).treasure.j
        and (*pl).with_treasure == 0 and (*pl).hp != 2)
    {
        cout << (*pl).name << " обнаружил под собой клад но не может его взять т.к. не полное здоровье" << endl;
    }


    if (not((*tech).command_changed)) cin >> tech->command; // P.s. tech->command это static string обьявленный в самом начале P.s. так было раньше
    if (tech->command == "Show_maze")
    {
        tech->show_mase = 1;

        temp_1 = lines[pl1->coord.i][(*pl1).coord.j];
        temp_2 = lines[pl2->coord.i][pl2->coord.j];
        temp_3 = lines[(*elem).treasure.i][(*elem).treasure.j];
        temp_4 = lines[(*croc).coord.i][(*croc).coord.j];
        lines[(*pl1).coord.i][(*pl1).coord.j] = 'O'; // Нанести игрока на карту лабиринта временно (чтобы было видно где он)
        lines[(*elem).treasure.i][(*elem).treasure.j] = 't';
        lines[(*pl2).coord.i][(*pl2).coord.j] = 'T';
        lines[(*croc).coord.i][(*croc).coord.j] = 'C';
        

        visual_lab(lines);
        cout << endl;
        // if(tech->show_mase) visual_lab(lines);                  // Вывести лабиринт
        lines[(*pl1).coord.i][(*pl1).coord.j] = temp_1; // Вернуть в точку в лабиринте то что было
        lines[(*pl2).coord.i][(*pl2).coord.j] = temp_2;
        lines[(*elem).treasure.i][(*elem).treasure.j] = temp_3;
        lines[(*croc).coord.i][(*croc).coord.j] = temp_4;
        cin >> tech->command;
    }
    if (tech->command == "Hide_maze")
    {
        tech->show_mase = 0;
        cin >> tech->command;
    }
    if (tech->command == "Stop" or tech->command == "stop") return 1;

    if (lines[(*pl).coord.i][(*pl).coord.j] == 'M') // чел стоит в медсанбате - тогда он все время хилится
    {
        (*pl).hp = 2;
        (*time_in_swamp) = 0;
        cout << "Стоишь в медсанбате - ты полностью здоров" << endl; // хер пойми как, но эта штука вызывает нарушение каких то прав
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'A') // чел стоит в Арсенале - тогда у него все время полный боезапас
    {
        (*pl).bullets = 3;
        (*pl).granade = 2;
        (*time_in_swamp) = 0;
        cout << "Стоишь в арсенале - у тебя 3 патрона и 2 гранаты" << endl;
    }

    bool trig = 0;
    while (not(tech->command == "Up" or tech->command == "Down" or tech->command == "Left" or tech->command == "Right"
        or tech->command == "Show_maze" or tech->command == "Shoot" or tech->command == "Explode"))
    {
        cout << "Херню какую то ввел, давай нормальную команду 819 строка";
        cin >> tech->command;
    }
    if (tech->command == "Up") (*pl).coord.i -= 1;

    if (tech->command == "Down") (*pl).coord.i += 1;

    if (tech->command == "Right") (*pl).coord.j += 1;

    if (tech->command == "Left") (*pl).coord.j -= 1;

    if ((*pl).coord.i == (*elem).treasure.i and (*pl).coord.j == (*elem).treasure.j and (*tech).treasure_picked != 1 and (*pl).hp == 2) // если нашел клад
    {
        cout << (*pl).name << " нашел клад. Берешь его?" << endl;
        cin >> tech->action;
        if (tech->action == "Yes" or tech->action == "y" or tech->action == "Y")
        {
            (*pl).with_treasure = 1;
            (*tech).treasure_picked = 1;
        }
    }

    if ((*pl).coord.i == (*elem).treasure.i and (*pl).coord.j == (*elem).treasure.j and (*tech).treasure_picked != 1 and (*pl).hp < 2)
    {
        cout << (*pl).name << " нашел клад но не может его взять" << endl;
    }

    if ((*pl).with_treasure)
    {
        (*elem).treasure.i = (*pl).coord.i;
        (*elem).treasure.j = (*pl).coord.j;
    }
    if (tech->command == "Shoot")
    {
        cout << "Куда стреляешь?" << endl;
        cin >> tech->command;
        if (id == 10) // Тогда охотник - первый игрок (И сейчас ход 1 игрока)
        {
            trash = shoot(lines, &(*pl2), pl, tech->command, croc, tech, elem);
            if ( (lines[(*pl).coord.i][(*pl).coord.j] == 's' or 
                  lines[(*pl).coord.i][(*pl).coord.j] == 'u')
                  and (*croc).alive)
            {
                (*time_in_swamp) += 1;
                cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
                if ((*time_in_swamp) == 3)
                {
                    cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                    (*time_in_swamp) = 0;
                    death(lines, pl, tech, elem);
                    return 1;
                }
            }
            else (*time_in_swamp) = 0;
            return 1;
        }
        if (id == 20) // Тогда охотник - второй игрок (И сейчас ход 2 игрока)
        {
            trash = shoot(lines, &(*pl1), pl, tech->command, croc, tech, elem);
            if ((lines[(*pl).coord.i][(*pl).coord.j] == 's' or
                lines[(*pl).coord.i][(*pl).coord.j] == 'u')
                and (*croc).alive)
            {
                (*time_in_swamp) += 1;
                cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
                if ((*time_in_swamp) == 3)
                {
                    cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                    (*time_in_swamp) = 0;
                    death(lines, pl, tech, elem);
                    return 1;
                }
            }
            else (*time_in_swamp) = 0;
            return 1;
        }
    }
    if (tech->command == "Explode")
    {
        cout << "Куда бросаешь гранату?" << endl;
        cin >> tech->action;
        explode(lines, pl, tech->action);
        if ((lines[(*pl).coord.i][(*pl).coord.j] == 's' or
            lines[(*pl).coord.i][(*pl).coord.j] == 'u')
            and (*croc).alive)
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                (*time_in_swamp) = 0;
                death(lines, pl, tech, elem);
                return 1;
            }
        }
        else (*time_in_swamp) = 0;
        return 1;
    }
    /*if(trig == 0) // кажется можно удалить т.к. trig = 0 когда челик никуда не ушел (раньше код был такой)
    {
        cout << "Бум"<< endl;
        if(lines[(*pl).coord.i][(*pl).coord.j] == 's')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                death(lines, pl );
                (*time_in_swamp) = 0;
                return 1;
            }
            return 1;
        }
        return 1;
    } */

    if (lines[(*pl).coord.i][(*pl).coord.j] == 'r')
    {
        (*time_in_swamp) = 0;
        for (int k = 0; k < sett->number_of_river; k++)
        {
            if ((*pl).coord.i == elem->complete_river[k].i and (*pl).coord.j == elem->complete_river[k].j )
            {

                if (k - 3 > 0)
                {
                    (*pl).coord = elem->complete_river[k - 3];
                    cout << "Река понесла" << endl;
                    return 1;
                    break;
                }
                else
                {
                    (*pl).coord = elem->u_mouth;
                    cout << "Река в устье принесла" << endl;
                    (*time_in_swamp) += 1;
                    cout << "Болото рычит " << (*time_in_swamp) << " раз";
                    return 1;
                    break;

                }
            }
        }
    }

    if (lines[(*pl).coord.i][(*pl).coord.j] == 'c')
    {
        (*time_in_swamp) = 0;
        cout << "Суша" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'u')
    {
        cout << "Пришел в устье" << endl;
        if ((*croc).alive)
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                death(lines, pl, tech, elem);
                (*time_in_swamp) = 0;
                return 1;
            }
        }
        else cout << "Крокодила нет, в болоте рычать некому" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'M')
    {
        (*pl).hp = 2;
        (*time_in_swamp) = 0;
        cout << "Медсанбат" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'A')
    {
        (*pl).bullets = 3;
        (*pl).granade = 2;
        (*time_in_swamp) = 0;
        cout << "Арсенал" << endl;
        cout << "Ты получил 3 патрона и 2 гранаты" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == '1')
    {
        (*time_in_swamp) = 0;
        (*pl).coord = elem->pit2;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == '2')
    {
        (*time_in_swamp) = 0;
        (*pl).coord = elem->pit3;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == '3')
    {
        (*time_in_swamp) = 0;
        (*pl).coord = elem->pit1;
        cout << "Попал в яму" << endl;
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 's')
    {
        if ((*croc).alive)
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                death(lines, pl, tech, elem);
                (*time_in_swamp) = 0;
                return 1;
            }
        }
        else
        {
            cout << "Крокодила нет, в болоте рычать некому" << endl;

        }
        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'b' or
        lines[(*pl).coord.i][(*pl).coord.j] == 'w')
    {
        cout << "Бум" << endl;

        if (tech->command == "Up") (*pl).coord.i += 1;
        if (tech->command == "Down") (*pl).coord.i -= 1;
        if (tech->command == "Right") (*pl).coord.j -= 1;
        if (tech->command == "Left") (*pl).coord.j += 1;

        if ((*pl).with_treasure)
        {
            (*elem).treasure.i = (*pl).coord.i;
            (*elem).treasure.j = (*pl).coord.j;
        }

        if (lines[(*pl).coord.i][(*pl).coord.j] == 's' or lines[(*pl).coord.i][(*pl).coord.j] == 'u')
        {
            (*time_in_swamp) += 1;
            cout << "Болото рычит " << (*time_in_swamp) << " раз" << endl;
            if ((*time_in_swamp) == 3)
            {
                cout << "Покусан крокодилом, отправился в медсанбат" << endl;
                (*time_in_swamp) = 0;
                death(lines, pl, tech, elem);
                return 1;
            }

        }

        return 1;
    }
    if (lines[(*pl).coord.i][(*pl).coord.j] == 'E')
    {
        if ((*pl).with_treasure)
        {
            (*time_in_swamp) = 0;
            cout << "Игрок" << (*pl).id << "вышел из лабиринта с кладом и победил!" << endl;
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
    settings sett;
    elements elem;
    
    technical tech;
    char temp_1;
    char temp_2;
    char temp_3;
    int trash = 0;
    
    tech.action = "";
    setlocale(0, "Russian");
    cout << "Крокодил возрождается на прежнем месте?" << endl;
    cin >> tech.action;
    if (tech.action == "Yes" or tech.action == "y" or tech.action == "yes") sett.respawn_new_place = 0; // в будущем часть функции init_settings
    else sett.respawn_new_place = 1;

    player player_1 = init_player(10); // 10 это id
    player player_2 = init_player(20); // 20 это id
    

    char** lines;
    
    string key = "";

    // Начинаются ходы игроков
    cout << "Показывать лабиринт? Напиши Yes если да, если нет, напиши что угодно" << endl;
    cin >> tech.command;
    if (tech.command == "Yes" or tech.command == "yes" or tech.command == "y" or tech.command == "Y") tech.show_mase = 1;
    cout << "Введите количество рек в лабиринте (от 20 до 24)" << endl;
    cin >> sett.number_of_river;
    do
    {
        lines = create_labirint(&elem, &sett, &tech);
        elem.treasure = create_treasure(lines, &tech);
        
        if (tech.show_mase)
        {
            temp_3 = lines[elem.treasure.i][elem.treasure.j];
            lines[elem.treasure.i][elem.treasure.j] = 't';
            visual_lab(lines);
            cout << endl;
            lines[elem.treasure.i][elem.treasure.j] = temp_3;
        }
        cout << "Устраивает лабиринт?" << endl;
        cin >> key;

    } while (not(key == "Yes" or key == "yes" or key == "y" or key == "yep"));

    crocodile croc = init_croc(lines, &tech);
    

    int repets = 0;

    while ( not(tech.command == "Stop" or tech.command == "stop"))
    {
        if (not(croc.alive))
        {
            croc.time_to_res -= 1;
            if (croc.time_to_res == 2) cout << "До появления нового крокодила в болоте осталось 2 хода" << endl;
            if (croc.time_to_res == 1) cout << "До появления нового крокодила в болоте остался 1 ход" << endl;
            if (croc.time_to_res == 0)
            {
                if (sett.respawn_new_place) croc = init_croc(lines, &tech); // новый крокодил т.е. случайное место
                if ( not(sett.respawn_new_place) ) croc.alive = 1; // крокодил остался на прежнем месте
                
                cout << "В болоте завелся новый крокодил" << endl;
            }
        }
        cout << "Ход первого игрока:" << endl;
        trash = step(&player_1, tech.turns_1, &tech.time_in_swamp_1, player_1.id, &croc, &player_1, &player_2, &tech, &elem, &sett); // Ход 1 игрока
        while (trash != 1)
        {
            cout << "Херню какую то ввел, давай нормальную команду" << endl;
            trash = step(&player_1, tech.turns_1, &tech.time_in_swamp_1, player_1.id, &croc, &player_1, &player_2, &tech, &elem, &sett);
        }
        cout << endl;
        cout << "Ход второго игрока:" << endl;
        trash = step(&player_2, tech.turns_2, &tech.time_in_swamp_2, player_2.id, &croc, &player_1, &player_2, &tech, &elem, &sett); // Ход 2 игрока
        while (trash != 1)
        {
            cout << "Херню какую то ввел, давай нормальную команду" << endl;
            trash = step(&player_2, tech.turns_2, &tech.time_in_swamp_2, player_2.id, &croc, &player_1, &player_2, &tech, &elem, &sett);
        }
        cout << endl;

        tech.turns_1 += 1;
        tech.turns_2 += 1;
    }

    for (int i = 0; i < 10; i++)
    {
        delete[] lines[i];
    }
    delete[] lines;
    for (int k = 0; k < count_of_path; k++)
    {
        delete[] paths[k].river;
    }

    delete[] river;
    delete[] arr_of_land_points;
    delete[] paths;
    delete[] elem.complete_river;

}
