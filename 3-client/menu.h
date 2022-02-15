#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;
enum menu_state
{
    unlog_menu = 0,
    common_menu = 1,
    merchant_menu = 2,
    customer_menu = 3,
    admin_menu = 4
};
class menu
{
    public:
    vector<vector<string> > menu_text;
    menu();
    const void print_menu(menu_state cur_state);
    const bool check_operateCode(int operateCode, menu_state cur_state);
};

#endif//MENU_H