#include "menu.h"
menu::menu()
{
    menu_text.resize(5);
    menu_text[unlog_menu].push_back("Please input operate code or item_id to view item_details:");
    menu_text[unlog_menu].push_back("0.exit");
    menu_text[unlog_menu].push_back("1.log in");
    menu_text[unlog_menu].push_back("2.sign in");
    menu_text[unlog_menu].push_back("3.search commodity");
    menu_text[common_menu] = menu_text[unlog_menu];
    menu_text[common_menu].push_back("4.consult balance");
    menu_text[common_menu].push_back("5.recharge");
    menu_text[common_menu].push_back("6.change password");
    menu_text[common_menu].push_back("7.Managing shopping cart");
    menu_text[customer_menu] = menu_text[common_menu];
    menu_text[merchant_menu] = menu_text[common_menu];
    menu_text[merchant_menu].push_back("8.Managing commodity information");
    menu_text[admin_menu] =  menu_text[unlog_menu];
    menu_text[admin_menu].push_back("4.Change discount rate");
};
const void menu::print_menu(menu_state cur_state)
{
    for(int i = 0; i < menu_text[cur_state].size(); i++)
        cout << menu_text[cur_state][i] << endl;
}
const bool menu::check_operateCode(int operateCode, menu_state cur_state)
{
    if(operateCode >= menu_text[cur_state].size() || operateCode < 0)
    {
        cout << "Please input right number" << endl;
        return 1;
    }
    else
        return 0;
}