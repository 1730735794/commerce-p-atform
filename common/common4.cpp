#include<iostream>
using namespace std;
int main()
{
    int result = rand() % 1000 + 1;
    cout << "请输入1~1000以内的数字" << endl;
    int x;
    cin >> x;
    while(x != result)
    {
        int judge;
        if (cin.fail())
        {
            cin.clear();
            cin.clear();
            cin.ignore(100, '\n');
            cout << "只能输入数字!" << endl;
        }
        else if(x > 1000 || x < 1)
            cout << "请输入1~1000以内的数字！" << endl;
        else if(x > result) cout << "太大了" << endl;
        else if(x < result)
            cout << "太小了" << endl;
        cout << "再猜一次吧！" << endl;
        cin >> x;
    }
    cout << "答对啦！";
    return 0;
}