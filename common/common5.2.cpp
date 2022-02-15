#include<iostream>
#include<utility>
using namespace std;
class point {
    public:
        int x;
        int y;
        point(int a,int b)
        {
            x = a;
            y = b;
        }
        point &operator++()
        {
            x++;
            y++;
            return *this;
        }
        point operator++(int)
        {
            point tmp = *this;
            ++x;
            ++y;
            return tmp;
        }
        point &operator--()
        {
            x--;
            y--;
            return *this;
        }
        point operator--(int)
        {
            point tmp = *this;
            --x;
            --y;
            return tmp;
        }
};
int main()
{
    point a(1,1);
    a++;
    cout << a.x << " " << a.y << endl;
    ++a;
    cout << a.x << " " << a.y << endl;
    a--;
    cout << a.x << " " << a.y << endl;
    --a;
    cout << a.x << " " << a.y << endl;
    return 0;
}