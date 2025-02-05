#include <iostream>
using namespace std;
class First
{
public:
	int arg_1;
	int arg_2;
	First();
};
First::First()
{
	arg_1 = 1;
	arg_2 = 2;
}
int main()
{
	First t;
	cout << t.arg_1;
	cout << t.arg_2;
}
