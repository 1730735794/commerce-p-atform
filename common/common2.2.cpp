#include <iostream>
using namespace std;
class matrix
{
private:
    int lines;
    int rows;
    int **temp;

public:
    matrix(int line_num, int row_num)
    {
        lines = line_num;
        rows = row_num;
        temp = new int *[lines];
        for (int i = 0; i < lines; i++)
        {
            temp[i] = new int[rows];
            for (int j = 0; j < rows; j++)
                temp[i][j] = 0;
        }
    }
    matrix()
    {
        lines = 0;
        rows = 0;
        temp = NULL;
    }
    matrix(matrix &p)
    {
        for (int i = 0; i < lines; i++)
            delete[] this->temp[i];
        delete[] this->temp;
        lines = p.lines;
        rows = p.rows;
        temp = new int *[lines];
        for (int i = 0; i < lines; i++)
        {
            temp[i] = new int[rows];
            for (int j = 0; j < rows; j++)
                temp[i][j] = p.temp[i][j];
        }
    }
    void input()
    {
        cout << "请逐个输入矩阵数据" << endl;
        for (int i = 0; i < lines; i++)
        {
            for (int j = 0; j < rows; j++)
                cin >> temp[i][j];
        }
    }
    void output()
    {
        for (int i = 0; i < lines; i++)
        {
            for (int j = 0; j < rows; j++)
                cout << temp[i][j] << " ";
            cout << endl;
        }
    }
    ~matrix()
    {
        for (int i = 0; i < lines; i++)
            delete[] this->temp[i];
        delete[] this->temp;
    }
    void operator=(const matrix &a)
    {
        for (int i = 0; i < lines; i++)
            delete[] this->temp[i];
        delete[] this->temp;
        this->lines = a.lines;
        this->rows = a.rows;
        this->temp = new int *[lines];
        for (int i = 0; i < lines; i++)
        {
            this->temp[i] = new int[rows];
            for (int j = 0; j < rows; j++)
                this->temp[i][j] = a.temp[i][j];
        }
    }
    matrix operator+(const matrix &a)
    {
        matrix result(*this);
        if (this->lines != a.lines || this->rows != a.rows)
            exit(0);
        else
        {
            for (int i = 0; i < lines; i++)
            {
                for (int j = 0; j < rows; j++)
                    result.temp[i][j] += a.temp[i][j];
            }
        }
        return result;
    }
    matrix operator-(const matrix &a)
    {
        matrix result(*this);
        if (this->lines != a.lines || this->rows != a.rows)
            exit(0);
        else
        {
            for (int i = 0; i < lines; i++)
            {
                for (int j = 0; j < rows; j++)
                    result.temp[i][j] -= a.temp[i][j];
            }
        }
        return result;
    }
};
int main()
{
    int line, row;
    cout << "请输入矩阵行数" << endl;
    cin >> line;
    cout << "请输入矩阵列数" << endl;
    cin >> row;
    matrix A1(line, row);
    A1.input();
    matrix A2(line, row);
    A2.input();
    matrix A3;
    A3 = A1 + A2;
    A3.output();
    A3 = A1 - A2;
    A3.output();
    A1.~matrix();
    A2.~matrix();
    A3.~matrix();
    matrix *pA1 = new matrix(line, row);
    matrix *pA2 = new matrix(line, row);
    matrix *pA3 = new matrix;
    pA1->input();
    pA2->input();
    *pA3 = *pA1 + *pA2;
    pA3->output();
    *pA3 = *pA1 - *pA2;
    pA3->output();
    pA1->~matrix();
    pA2->~matrix();
    pA3->~matrix();
    delete[] pA1;
    delete[] pA2;
    delete[] pA3;
    return 0;
}