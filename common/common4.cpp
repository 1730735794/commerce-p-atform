#include<iostream>
using namespace std;
int main()
{
    int result = rand() % 1000 + 1;
    cout << "������1~1000���ڵ�����" << endl;
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
            cout << "ֻ����������!" << endl;
        }
        else if(x > 1000 || x < 1)
            cout << "������1~1000���ڵ����֣�" << endl;
        else if(x > result) cout << "̫����" << endl;
        else if(x < result)
            cout << "̫С��" << endl;
        cout << "�ٲ�һ�ΰɣ�" << endl;
        cin >> x;
    }
    cout << "�������";
    return 0;
}