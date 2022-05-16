#include <iostream>
#include <string>

using namespace std;

int m = 0;

void PrintRevNums(int sum)
{
    int num;
    cin >> num;

    sum += num;
    
    if(num > m)
        m = num;

    if(num == 0) //do nothing
        return;
    else
    {
        PrintRevNums(sum);
        cout << num << " Total: " << sum << endl;
    }
    
    if(sum == num)
       cout << "The maximum value is: " << m << endl;

}