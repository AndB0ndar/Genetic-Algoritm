#include <iostream>

using namespace std;

int main0()
{
	int a = 7;
	int b = 7;
	int c = (a + b + abs(a-b))/2;
	cout << c;

	return 0;
}