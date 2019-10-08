#include <iostream>
#include "calculator.h"
#include <string>

using namespace std;

int main()
{
    string str;
    cin >> str;
    Node cal;
    cout << cal.create(str) << endl;
    cout << cal.calculate() << endl;
    return 0;
}