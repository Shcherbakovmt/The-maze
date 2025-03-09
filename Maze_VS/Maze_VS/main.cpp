#include <iostream>
#include <time.h>
#include <locale.h>
#include <iso646.h> // Чтобы можно было писать and и or
#include <string>
#include <SFML/Graphics.hpp>
#include <windows.h>


using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::to_string;


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




void WindowClearDrawDisplay(sf::RenderWindow& window, sf::Text& text)
{
    string s = text.getString();
    string s_new = "";
    int count_lines = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '\n')
        {
            count_lines += 1;
        }
    }
    if (count_lines > 15)
    {
        int line_number = 0;
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '\n')
            {
                line_number += 1;
            }
            if (line_number >= count_lines - 15)
            {
                s_new += s[i];
            }

        }
        text.setString(s_new);
    }

    window.clear();
    window.draw(text);
    window.display();
}

void WindowClearDrawDisplay(sf::RenderWindow& window, sf::Text& text, sf::Text& EnteredText)
{
    string s = text.getString();
    string s_new = "";
    int count_lines = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '\n')
        {
            count_lines += 1;
        }
    }
    if (count_lines > 15)
    {
        int line_number = 0;
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '\n')
            {
                line_number += 1;
            }
            if (line_number >= count_lines - 15)
            {
                s_new += s[i];
            }

        }
        text.setString(s_new);
    }

    window.clear();
    window.draw(EnteredText);
    window.draw(text);
    window.display();
}

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
    char** lines;
    crocodile croc;
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
        cout << "The default settings are set" << endl;
    }

};
class technical
{
public:

    time_t seed = time(0);
    int time_in_swamp_1 = 0;
    int time_in_swamp_2 = 0;
    bool show_mase = 1; // пока что по умолчанию лабиринт виден
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
    int count_of_path = 0;
    long tries = 0;
    long min_path_index = 0;
    int repets = 0;
    point* arr_of_land_points = new point[64];
    river_path* paths = new river_path[2000000];
    point* river = new point[64];
};

class Maze : public elements, public settings, public technical
{
public:

    int rand_from_a_to_b(int a, int b)
    {
        seed = (seed * 73129 + 95121) % 10000;
        srand(seed);
        return rand() % (b - a + 1) + a;
    }
    player init_player(int id, sf::RenderWindow& window, sf::Text& text)
    {
        player pl;
        text.setString(text.getString() + "Enter the player's name\n");
        WindowClearDrawDisplay(window, text);
        
        
        pl.name = getCommandString(window, text);
        pl.id = id;
        pl.with_treasure = 0;
        pl.hp = 2;
        pl.bullets = 0;
        pl.granade = 0;
        return pl;
    }
    crocodile init_croc()
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
        croc.coord = swamp_points[rand_from_a_to_b(0, count_of_swamp - 1)];
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
    char** init_arr()
    {
        char** arr = new char* [10];

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
    bool is_there_path(char** lines)
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
        int trash = 0;

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
    bool create_river_complitely(char** lines)
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
        while ((u_mouth.i == center_of_swamp.i and u_mouth.j == center_of_swamp.j) or not (is_there_path(lines)))
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
    point create_treasure(char** lines)
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
        int index = rand_from_a_to_b(0, count_of_possible - 1);
        treas = possible[index];
        delete[] possible;
        return treas;
    }

    string death(player* player)
    {
        string ans = "";
        (*player).hp = 2;

        (*player).granade = 0;
        (*player).bullets = 0;
        if ((*player).with_treasure == 1)
        {
            treasure_picked = 0;
            ans += (*player).name + " dropped the treasure\n";
            (*player).with_treasure = 0;
            /*if (lines[(*player).coord.i][(*player).coord.j] == 's' or lines[(*player).coord.i][(*player).coord.j] == 'u')
            {
                create_treasure(lines);
                cout << "Крокодил съел клад месте с " << (*player).name << " и высрал его в другом месте" << endl;
            } */ // если нужно чтобы сокровище респавнилось при попадании в болото
        }
        (*player).coord = medbat;
        ans += (*player).name + " died and appeared in the medical unit.\n";
        return ans;
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
        char** lines = init_arr();
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
            lines = init_arr();
            create_u_mouth_and_swamp(lines); // сделали болото и рандомно определили в нем устье

            create_ars_and_med(lines); // теперь нужно добавить арсенал и медсанбат на расстоянии^2 не менее 17
            create_pits(lines); //  Теперь нужно сделать ямы по такому же принципу
            sucsess_river = create_river_complitely(lines); // Теперь сделаем реку - порядка 22 последовательных клеток
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

    string shoot(player* victim, player* hunter, string cmd, crocodile* croc)
    {
        string ans = "";
        if ((*hunter).bullets == 0)
        {
            ans += "You pull the trigger and nothing happens. You've wasted a turn\n";
            return ans;
        }
        (*hunter).bullets -= 1;
        if ((*hunter).bullets == 2) ans += "You have 2 bullets left.\n";
        if ((*hunter).bullets == 1) ans += "You have 1 bullet left.\n";
        if ((*hunter).bullets == 0) ans += "You don't have any bullets left.\n";
        if (cmd == "Down") // добавлен крокодил
        {
            for (int k = (*hunter).coord.i; k <= 9; k++)
            {
                if (k == (*victim).coord.i and (*hunter).coord.j == (*victim).coord.j) // если попал в игрока
                    // т.е. если игрок и крокодил на одной клетке, первым получит пулю игрок
                {
                    (*victim).hp -= 1;
                    ans += "Hit the player\n";
                    if ((*victim).hp == 0) ans += death(victim);

                    if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                    {
                        (*victim).with_treasure = 0;
                        treasure_picked = 0;
                        ans += (*victim).name + " dropped the treasure\n";
                    }
                    return ans;
                }
                if (k == (*croc).coord.i and (*hunter).coord.j == (*croc).coord.j and (*croc).alive) // если попал в крокодила
                {
                    (*croc).alive = 0;
                    (*croc).time_to_res = 3;
                    ans += "Killed a crocodile\n";
                    ans += "There are 3 moves left before the new crocodile appears.\n";
                    return ans;
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
                    ans += "Hit the player\n";
                    if ((*victim).hp == 0) ans += death(victim);
                    if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                    {
                        (*victim).with_treasure = 0;
                        treasure_picked = 0;
                        ans += (*victim).name + " dropped the treasure\n";
                    }
                    return ans;
                }
                if (k == (*croc).coord.i and (*hunter).coord.j == (*croc).coord.j and (*croc).alive) // если попал в крокодила
                {
                    (*croc).alive = 0;
                    (*croc).time_to_res = 3;
                    ans += "Killed a crocodile\n";
                    ans += "There are 3 moves left before the new crocodile appears.\n";
                    return ans;
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
                    ans += "Hit the player\n";
                    if ((*victim).hp == 0) ans += death(victim);
                    if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                    {
                        (*victim).with_treasure = 0;
                        treasure_picked = 0;
                        ans += (*victim).name + " dropped the treasure\n";
                    }
                    return ans;
                }
                if (k == (*croc).coord.j and (*hunter).coord.i == (*croc).coord.i and (*croc).alive) // если попал в крокодила
                {
                    (*croc).alive = 0;
                    (*croc).time_to_res = 3;
                    ans += "Killed a crocodile\n";
                    ans += "There are 3 moves left before the new crocodile appears.\n";
                    return ans;
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
                    ans += "Hit the player\n";
                    if ((*victim).hp == 0) ans += death(victim);

                    if ((*victim).hp == 1 and (*victim).with_treasure == 1)
                    {
                        (*victim).with_treasure = 0;
                        treasure_picked = 0;
                        ans += (*victim).name + " dropped the treasure\n";
                    }
                    return ans;
                }
                if (k == (*croc).coord.j and (*hunter).coord.i == (*croc).coord.i and (*croc).alive) // если попал в крокодила
                {
                    (*croc).alive = 0;
                    (*croc).time_to_res = 3;
                    ans += "Killed a crocodile\n";
                    ans += "There are 3 moves left before the new crocodile appears.\n";
                    return ans;
                }
            }
        }
        ans += "You missed\n";
        return ans;
    }

    string explode(player* player, string cmd)
    {
        string ans = "";
        int j = (*player).coord.j;
        int i = (*player).coord.i;
        if ((*player).granade == 0)
        {
            ans += "You put your hand in your pocket, take out a grenade-shaped stone and throw it. Nothing happened. You've wasted a turn\n";
            return ans;
        }
        (*player).granade -= 1;
        if (cmd == "Up")
        {
            if (i - 1 < 1)
            {
                ans += "The wall didn't explode\n";
                ans += "You have " + to_string((*player).granade) + "grenades left\n";
                return ans;
            }
            else
            {
                if (lines[i - 1][j] == 'b')
                {
                    ans += "The wall exploded\n";
                    ans += "You have " + to_string((*player).granade) + "grenades left\n";
                    lines[i - 1][j] = 'c';
                    return ans;
                }
            }
        }
        if (cmd == "Down")
        {
            if (i + 1 > 8)
            {
                ans += "The wall didn't explode\n";
                ans += "You have " + to_string((*player).granade) + "grenades left\n";
                return ans;
            }
            else
            {
                if (lines[i + 1][j] == 'b')
                {
                    ans += "The wall exploded\n";
                    ans += "You have " + to_string((*player).granade) + "grenades left\n";
                    lines[i + 1][j] = 'c';
                    return ans;
                }
            }
        }
        if (cmd == "Left")
        {
            if (j - 1 < 1)
            {
                ans += "The wall didn't explode\n";
                ans += "You have " + to_string((*player).granade) + "grenades left\n";
                return ans;
            }
            else
            {
                if (lines[i][j - 1] == 'b')
                {
                    ans += "The wall exploded\n";
                    ans += "You have " + to_string((*player).granade) + "grenades left\n";
                    lines[i][j - 1] = 'c';
                    return ans;
                }
            }
        }
        if (cmd == "Right")
        {
            if (j + 1 > 8)
            {
                ans += "The wall didn't explode\n";
                ans += "You have " + to_string((*player).granade) + "grenades left\n";
                return ans;
            }
            else
            {
                if (lines[i][j + 1] == 'b')
                {
                    ans += "The wall exploded\n";
                    ans += "You have " + to_string((*player).granade) + "grenades left\n";
                    lines[i][j + 1] = 'c';
                    return ans;
                }
            }
        }
        ans += "There was no boom or wall, what did you do? Grenades don't blow up the crocodile or the players.\n";
        ans += "You have " + to_string((*player).granade) + "grenades left\n";
        return ans;
    }

    void getCommandToDo(sf::RenderWindow& window /*, sf::Text& text */ )
    {
        while (command_changed == 0)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
                {
                    command = "Right";
                    command_changed = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
                {
                    command = "Left";
                    command_changed = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
                {
                    command = "Down";
                    command_changed = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
                {
                    command = "Up";
                    command_changed = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::F))
                {
                    command = "Shoot";
                    command_changed = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::E))
                {
                    command = "Explode";
                    command_changed = 1;
                }
            }
        }
        //text.setString(text.getString() + command + '\n');
        //WindowClearDrawDisplay(window, text); // касперский жалуется

    }
    string getCommandString(sf::RenderWindow& window, sf::Text& text)
    {
        bool trig = 1;
        string ans = "";
        char add = 'f';
        while (trig == 1)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (textEntered->unicode < 128 and textEntered->unicode != 13 and textEntered->unicode != 8) // Проверка что был введен не Enter и не Backspace
                    {
                        add = static_cast<char>(textEntered->unicode);
                        ans += add;
                        text.setString(text.getString() + add);
                        WindowClearDrawDisplay(window, text);
                    }
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scan::Enter)
                    {
                        add = '\n';
                        trig = 0;
                        text.setString(text.getString() + add);
                        WindowClearDrawDisplay(window, text);
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scan::Backspace)
                    {
                        if (ans != "")
                        {
                            ans.pop_back();

                            string temp = "";
                            temp += text.getString();
                            temp.pop_back();
                            text.setString(temp);

                            WindowClearDrawDisplay(window, text);
                        }
                    }
                }
            }
        }
        cout << "Entered some text\n";
        return ans;
    }

    bool step(player* pl, int turns, int* time_in_swamp, int id, crocodile* croc, player* pl1, player* pl2, sf::Text& text, sf::RenderWindow& window)
    {
        Sleep(300);
        command_changed = 0;
        

        int trash = 0;
        char temp_1;
        char temp_2;
        char temp_3;
        char temp_4;
        if (turns == 0) // высадка
        {
            text.setString(text.getString() + "Enter the landing coordinates (numbers from 1 to 8)\n");
            WindowClearDrawDisplay(window, text);
            string s = getCommandString(window, text);
            int x = s[0] - '0';
            int y = s[2] - '0';
            (*pl).coord.j = x;
            (*pl).coord.i = y;

            while (lines[(*pl).coord.i][(*pl).coord.j] == 'b')
            {
                text.setString(text.getString() + "You landed in boom. Please select another point\n Enter the coordinates of the landing (numbers from 1 to 8)\n");
                WindowClearDrawDisplay(window, text);
                string s = getCommandString(window, text);
                int x = s[0] - '0';
                int y = s[2] - '0';
                (*pl).coord.j = x;
                (*pl).coord.i = y;
            }



            (*pl).coord.i += 1; // чтобы в нулевой ход игрок пришел на клетку, куда захотел высадиться
            command = "Up";
            command_changed = 1;
        }

        if (player_in_exit(pl) and turns != 0) // если человек находится "в выходе"
        {
            text.setString(text.getString() + (*pl).name + " goes back into the maze\n");
            WindowClearDrawDisplay(window, text);

            if ((*pl).coord.i == 9) command = "Up";
            if ((*pl).coord.i == 0) command = "Down";
            if ((*pl).coord.j == 9) command = "Left"; // определяем куда надо шагнуть обратно
            if ((*pl).coord.j == 0) command = "Right";
            command_changed = 1;
        }

        if ((*pl).coord.i == treasure.i and (*pl).coord.j == treasure.j // на случай если сокровище заспавнилось прямо под игроком
            and (*pl).with_treasure == 0 and (*pl).hp == 2)
        {

            text.setString(text.getString() + (*pl).name + " found a treasure under him. Are you taking it?\n");
            WindowClearDrawDisplay(window, text);

            action = getCommandString(window, text);
            if (action == "Yes" or action == "y" or action == "Y") (*pl).with_treasure = 1;
        }
        if ((*pl).coord.i == treasure.i and (*pl).coord.j == treasure.j
            and (*pl).with_treasure == 0 and (*pl).hp != 2)
        {
            text.setString(text.getString() + (*pl).name + " found a treasure under himself, but he can't take it because he's not in full health\n");
            WindowClearDrawDisplay(window, text);
        }


        getCommandToDo(window);
        /*if (command == "Show_maze")
        {
            show_mase = 1;

            temp_1 = lines[pl1->coord.i][(*pl1).coord.j];
            temp_2 = lines[pl2->coord.i][pl2->coord.j];
            temp_3 = lines[treasure.i][treasure.j];
            temp_4 = lines[(*croc).coord.i][(*croc).coord.j];
            lines[(*pl1).coord.i][(*pl1).coord.j] = 'O'; // Нанести игрока на карту лабиринта временно (чтобы было видно где он)
            lines[treasure.i][treasure.j] = 't';
            lines[(*pl2).coord.i][(*pl2).coord.j] = 'T';
            lines[(*croc).coord.i][(*croc).coord.j] = 'C';


            visual_lab(lines);
            cout << endl;
            // if(show_mase) visual_lab(lines);                  // Вывести лабиринт
            lines[(*pl1).coord.i][(*pl1).coord.j] = temp_1; // Вернуть в точку в лабиринте то что было
            lines[(*pl2).coord.i][(*pl2).coord.j] = temp_2;
            lines[treasure.i][treasure.j] = temp_3;
            lines[(*croc).coord.i][(*croc).coord.j] = temp_4;
        }*/

        if (command == "Stop" or command == "stop") return 1;

        if (lines[(*pl).coord.i][(*pl).coord.j] == 'M') // чел стоит в медсанбате - тогда он все время хилится
        {
            (*pl).hp = 2;
            (*time_in_swamp) = 0;
            text.setString(text.getString() + "You're standing in the medical ward, and you're completely healthy\n");
            WindowClearDrawDisplay(window, text);
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == 'A') // чел стоит в Арсенале - тогда у него все время полный боезапас
        {
            (*pl).bullets = 3;
            (*pl).granade = 2;
            (*time_in_swamp) = 0;
            text.setString(text.getString() + "Standing in the arsenal, you have 3 bullets and 2 grenades\n");
            WindowClearDrawDisplay(window, text);
        }

        bool trig = 0;
        if(command_changed == 0) getCommandToDo(window);
        

        if (command == "Up") (*pl).coord.i -= 1;

        if (command == "Down") (*pl).coord.i += 1;

        if (command == "Right") (*pl).coord.j += 1;

        if (command == "Left") (*pl).coord.j -= 1;

        if ((*pl).coord.i == treasure.i and (*pl).coord.j == treasure.j and treasure_picked != 1 and (*pl).hp == 2) // если нашел клад
        {
            text.setString(text.getString() + (*pl).name + " found a treasure. Are you taking it?\n");
            WindowClearDrawDisplay(window, text);
            action = getCommandString(window, text);
            if (action == "Yes" or action == "y" or action == "Y")
            {
                (*pl).with_treasure = 1;
                treasure_picked = 1;
            }
        }

        if ((*pl).coord.i == treasure.i and (*pl).coord.j == treasure.j and treasure_picked != 1 and (*pl).hp < 2)
        {
            text.setString(text.getString() + (*pl).name + " found the treasure, but he can't take it\n");
            WindowClearDrawDisplay(window, text);

        }

        if ((*pl).with_treasure)
        {
            treasure.i = (*pl).coord.i;
            treasure.j = (*pl).coord.j;
        }
        if (command == "Shoot")
        {
            text.setString(text.getString() + "Where are you shooting at?\n");
            WindowClearDrawDisplay(window, text);
            
            getCommandToDo(window);
            if (id == 10) // Тогда охотник - первый игрок (И сейчас ход 1 игрока)
            {
                text.setString(text.getString() + shoot(&(*pl2), pl, command, croc));

                if ((lines[(*pl).coord.i][(*pl).coord.j] == 's' or
                    lines[(*pl).coord.i][(*pl).coord.j] == 'u')
                    and (*croc).alive)
                {
                    (*time_in_swamp) += 1;
                    text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                    WindowClearDrawDisplay(window, text);

                    if ((*time_in_swamp) == 3)
                    {
                        text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n");
                        WindowClearDrawDisplay(window, text);
                        (*time_in_swamp) = 0;
                        death(pl);
                        return 1;
                    }
                }
                else (*time_in_swamp) = 0;
                return 1;
            }
            if (id == 20) // Тогда охотник - второй игрок (И сейчас ход 2 игрока)
            {
                text.setString(text.getString() + shoot(&(*pl1), pl, command, croc));
                if ((lines[(*pl).coord.i][(*pl).coord.j] == 's' or
                    lines[(*pl).coord.i][(*pl).coord.j] == 'u')
                    and (*croc).alive)
                {
                    (*time_in_swamp) += 1;
                    text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                    WindowClearDrawDisplay(window, text);
                    if ((*time_in_swamp) == 3)
                    {
                        text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n");
                        WindowClearDrawDisplay(window, text);
                        (*time_in_swamp) = 0;
                        death(pl);
                        return 1;
                    }
                }
                else (*time_in_swamp) = 0;
                return 1;
            }
        }

        if (command == "Explode")
        {
            text.setString(text.getString() + "Where are you throwing the grenade? \n");
            WindowClearDrawDisplay(window, text);
            action = getCommandString(window, text);

            
            text.setString(text.getString() + explode(pl, action) + "\n");
            WindowClearDrawDisplay(window, text);

            if ((lines[(*pl).coord.i][(*pl).coord.j] == 's' or
                lines[(*pl).coord.i][(*pl).coord.j] == 'u')
                and (*croc).alive)
            {
                (*time_in_swamp) += 1;
                text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                WindowClearDrawDisplay(window, text);
                if ((*time_in_swamp) == 3)
                {
                    (*time_in_swamp) = 0;
                    text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n" + death(pl));
                    WindowClearDrawDisplay(window, text);
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
            for (int k = 0; k < number_of_river; k++)
            {
                if ((*pl).coord.i == complete_river[k].i and (*pl).coord.j == complete_river[k].j)
                {

                    if (k - 3 > 0)
                    {
                        (*pl).coord = complete_river[k - 3];
                        text.setString(text.getString() + "The river carried you away\n");
                        WindowClearDrawDisplay(window, text);
                        return 1;
                        break;
                    }
                    else
                    {
                        (*pl).coord = u_mouth;
                        (*time_in_swamp) += 1;

                        text.setString(text.getString() + "The river brought you to the mouth\n" + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                        WindowClearDrawDisplay(window, text);

                        return 1;
                        break;

                    }
                }
            }
        }

        if (lines[(*pl).coord.i][(*pl).coord.j] == 'c')
        {
            (*time_in_swamp) = 0;
            text.setString(text.getString() + "Land\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == 'u')
        {
            text.setString(text.getString() + "You came to the mouth\n");
            WindowClearDrawDisplay(window, text);

            if ((*croc).alive)
            {
                (*time_in_swamp) += 1;
                text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                WindowClearDrawDisplay(window, text);
                if ((*time_in_swamp) == 3)
                {
                    text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n" + death(pl));
                    WindowClearDrawDisplay(window, text);
                    (*time_in_swamp) = 0;
                    return 1;
                }
            }
            else
            {
                text.setString(text.getString() + "There is no crocodile, there is no one to growl in the swamp\n");
                WindowClearDrawDisplay(window, text);
            }

            return 1;
        }

        if (lines[(*pl).coord.i][(*pl).coord.j] == 'M')
        {
            (*pl).hp = 2;
            (*time_in_swamp) = 0;
            text.setString(text.getString() + "Medical unit\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == 'A')
        {
            (*pl).bullets = 3;
            (*pl).granade = 2;
            (*time_in_swamp) = 0;
            text.setString(text.getString() + "Arsenal\n You got 3 bullets and 2 grenades\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == '1')
        {
            (*time_in_swamp) = 0;
            (*pl).coord = pit2;
            text.setString(text.getString() + "Got into a hole\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == '2')
        {
            (*time_in_swamp) = 0;
            (*pl).coord = pit3;
            text.setString(text.getString() + "Got into a hole\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == '3')
        {
            (*time_in_swamp) = 0;
            (*pl).coord = pit1;
            text.setString(text.getString() + "Got into a hole\n");
            WindowClearDrawDisplay(window, text);
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == 's')
        {
            if ((*croc).alive)
            {
                (*time_in_swamp) += 1;
                text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                WindowClearDrawDisplay(window, text);
                if ((*time_in_swamp) == 3)
                {
                    text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n" + death(pl));
                    WindowClearDrawDisplay(window, text);
                    (*time_in_swamp) = 0;
                    return 1;
                }
            }
            else
            {
                text.setString(text.getString() + "There is no crocodile, there is no one to growl in the swamp\n");
                WindowClearDrawDisplay(window, text);
            }
            return 1;
        }
        if (lines[(*pl).coord.i][(*pl).coord.j] == 'b' or
            lines[(*pl).coord.i][(*pl).coord.j] == 'w')
        {
            text.setString(text.getString() + "Boom\n");
            WindowClearDrawDisplay(window, text);

            if (command == "Up") (*pl).coord.i += 1;
            if (command == "Down") (*pl).coord.i -= 1;
            if (command == "Right") (*pl).coord.j -= 1;
            if (command == "Left") (*pl).coord.j += 1;

            if ((*pl).with_treasure)
            {
                treasure.i = (*pl).coord.i;
                treasure.j = (*pl).coord.j;
            }

            if (lines[(*pl).coord.i][(*pl).coord.j] == 's' or lines[(*pl).coord.i][(*pl).coord.j] == 'u')
            {
                (*time_in_swamp) += 1;
                text.setString(text.getString() + "The swamp roars " + to_string((*time_in_swamp)) + " times\n");
                WindowClearDrawDisplay(window, text);
                if ((*time_in_swamp) == 3)
                {
                    text.setString(text.getString() + pl->name + " bitten by a crocodile, went to the medical unit\n" + death(pl));
                    WindowClearDrawDisplay(window, text);
                    (*time_in_swamp) = 0;
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
                text.setString(text.getString() + pl->name + " came out of the maze with a treasure and won!\n");
                WindowClearDrawDisplay(window, text);
            }
            else
            {
                (*time_in_swamp) = 0;
                text.setString(text.getString() + pl->name + "'ve found a way out! You spend the next turn trying to get back in\n");
                WindowClearDrawDisplay(window, text);

            }
            return 1;
        }



        return 0;

    }


    char temp_3;
    int trash = 0;
    crocodile croc;
    player player_1;
    player player_2;
public:
    Maze(sf::RenderWindow& window, sf::Text& text, sf::Text& EnteredText)
    {

        cout << "Created Maze" << endl;

        setlocale(0, "Russian");

        
        player_1 = init_player(10, window, text); // 10 это id
        player_2 = init_player(20, window, text); // 20 это id



        string key = "";
        // Начинаются ходы игроков

        text.setString(text.getString() + "Enter the number of rivers in the maze (from 20 to 24)\n");

        WindowClearDrawDisplay(window, text);
        number_of_river = stoi(getCommandString(window, text));

        do
        {
            lines = create_labirint();
            treasure = create_treasure(lines);

            if (show_mase)
            {
                temp_3 = lines[treasure.i][treasure.j];
                lines[treasure.i][treasure.j] = 't';
                visual_lab(lines);
                cout << endl;
                lines[treasure.i][treasure.j] = temp_3;
            }
            text.setString(text.getString() + "Are you satisfied with the maze?\n");
            WindowClearDrawDisplay(window, text);
            key = getCommandString(window, text);

        } while (not(key == "Yes" or key == "yes" or key == "y" or key == "yep"));

        croc = init_croc();
    }
    ~Maze()
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

        delete[] river;
        delete[] arr_of_land_points;
        delete[] paths;
        delete[] complete_river;
    }

    void turn(sf::Text& text, sf::RenderWindow& window)
    {
        command = "";

            if (not(croc.alive))
            {
                croc.time_to_res -= 1;
                if (croc.time_to_res == 2) text.setString(text.getString() + "There are 2 moves left in the swamp before the new crocodile appears");
                if (croc.time_to_res == 1) text.setString(text.getString() + "There is 1 turn left in the swamp before the new crocodile appears");
                if (croc.time_to_res == 0)
                {
                    if (respawn_new_place) croc = init_croc(); // новый крокодил т.е. случайное место
                    if (not(respawn_new_place)) croc.alive = 1; // крокодил остался на прежнем месте

                    text.setString(text.getString() + "There's a new crocodile in the swamp");
                }
            }
            text.setString(text.getString() + player_1.name + "'s move:\n");
            WindowClearDrawDisplay(window, text);
            trash = step(&player_1, turns_1, &time_in_swamp_1, player_1.id, &croc, &player_1, &player_2, text, window); // Ход 1 игрока
            text.setString(text.getString() + "\n");
            WindowClearDrawDisplay(window, text);

            cout << endl;
            text.setString(text.getString() + player_2.name +  "'s move:\n");
            WindowClearDrawDisplay(window, text);
            trash = step(&player_2, turns_2, &time_in_swamp_2, player_2.id, &croc, &player_1, &player_2, text, window); // Ход 2 игрока
            text.setString(text.getString() + "\n");
            WindowClearDrawDisplay(window, text);

            cout << endl;

            turns_1 += 1;
            turns_2 += 1;
        }
};







int main()
{
    

    sf::RenderWindow window(sf::VideoMode({ 1000, 1000 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Font font;
    font.openFromFile("Fonts/Helvetica.ttf");

    sf::Text text(font);
    sf::Text EnteredText(font);
    EnteredText.setPosition({ 0,600 });

    Maze labirint(window, text, EnteredText);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        labirint.turn(text, window);
        window.clear();
        window.draw(text);
        window.display();
    }
    
    
}
