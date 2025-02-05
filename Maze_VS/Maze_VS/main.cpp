#include <iostream>
#include <time.h>
#include <locale.h>
#include <iso646.h> // ����� ����� ���� ������ and � or
#include "Functions.h"
using namespace std;

int main()
{
    char temp_1;
    char temp_2;
    char temp_3;
    action = "";

    player_1.hp = 2;
    player_2.hp = 2;
    player_1.with_treasure = 0;
    player_2.with_treasure = 0;
    player_1.bullets = 0;
    player_2.bullets = 0;
    player_1.granade = 0;
    player_2.granade = 0;
    player_1.id = 10;
    player_2.id = 20;
    player_1.name = "������ �����";
    player_2.name = "������ �����";

    char** lines;
    setlocale(0, "Russian");
    string key = "";
    /*cin >> key;

     while(key != "0") // ����� ���������� ����� ��������� ��������
    {
        lines = create_labirint();
        visual_lab(lines);
        for(int i = 0; i < 8; i++)
        {
            delete[] lines[i];
        }
        delete[] lines;
        for(int k = 0; k < count_of_path; k++)
        {
            delete[] paths[k].river;
        }
        cin >> key;
    } */

    // ���������� ���� �������
    cout << "���������� ��������? ������ Yes ���� ��, ���� ���, ������ ��� ������" << endl;
    cin >> command;
    if (command == "Yes" or command == "yes" or command == "y" or command == "Y") show_mase = 1;
    cout << "������� ���������� ��� � ��������� (�� 24)" << endl;
    cin >> number_of_river;
    do
    {
        lines = create_labirint();
        create_treasure(lines);
        if (show_mase)
        {
            temp_3 = lines[treasure.i][treasure.j];
            lines[treasure.i][treasure.j] = 't';
            visual_lab(lines);
            cout << endl;
            lines[treasure.i][treasure.j] = temp_3;
        }
        cout << "���������� ��������?" << endl;
        cin >> key;

    } while (not(key == "Yes" or key == "yes" or key == "y" or key == "yep"));



    int repets = 0;

    while (command != "Stop" and command != "stop")
    {
        cout << "��� ������� ������:" << endl;
        trash = step(&player_1, turns_1, &time_in_swamp_1, player_1.id); // ��� 1 ������
        while (trash != 1)
        {
            cout << "����� ����� �� ����, ����� ���������� �������" << endl;
            trash = step(&player_1, turns_1, &time_in_swamp_1, player_1.id);
        }
        cout << endl;
        cout << "��� ������� ������:" << endl;
        trash = step(&player_2, turns_2, &time_in_swamp_2, player_2.id); // ��� 2 ������
        while (trash != 1)
        {
            cout << "����� ����� �� ����, ����� ���������� �������" << endl;
            trash = step(&player_2, turns_2, &time_in_swamp_2, player_2.id);
        }
        cout << endl;

        turns_1 += 1;
        turns_2 += 1;
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
    delete[] complete_river;

}
