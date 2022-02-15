#include "menu.h"
#include "user.h"
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
int main()
{
    //初始化套接字
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sd = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(20);
    if (connect(sd, (sockaddr *)&server_addr, sizeof(sockaddr_in)) != 0)
    {
        cout << "Connect failed!";
        exit(0);
    }
    menu function_menu;
    menu_state menu_state = unlog_menu;
    user *active_user_info = NULL;
    all_item goods_database(sd);
    vector<pair<goods *, int>> debug_cart;
    buyer debug(sd, debug_cart);
    int operate_code = 1;
    while (operate_code)
    {
        cout << "----------------------------------------------" << endl;
        goods_database.get_all_item(sd); //每次操作更新本地所有物品信息
        goods_database.print_all(goods_database.base_goods_info);
        function_menu.print_menu(menu_state);
        operate_code = active_user_info->check_num(0, 1000 + goods_database.base_goods_info.size()); //menu_text有一行提示语
        if (operate_code > 1000)         //输入的是商品id,表示对商品进行操作
        {
            if (menu_state != unlog_menu)
            {
                int num_id = operate_code - 1001;
                active_user_info->add_shop_cart(goods_database.base_goods_info[num_id], goods_database, sd);
            }
            else
                cout << "Please log in first!" << endl;
        }
        else
        {
            function_menu.check_operateCode(operate_code, menu_state);
            switch (operate_code)
            {
            case 1: //登录
                active_user_info = active_user_info->log_in(sd, goods_database);
                if (active_user_info == &debug)
                    break;
                if (active_user_info->getUserType() == merchant)
                    menu_state = merchant_menu;
                else if (active_user_info->getUserType() == customer)
                    menu_state = customer_menu;
                else if (active_user_info->getUserType() == administrator)
                    menu_state = admin_menu;
                break;
            case 2: //注册
                active_user_info->sign_in(sd);
                break;
            case 3: //查找
                goods_database.search();
                break;
            case 4: //获取物品信息(非管理)/修改物品折扣(管理员)
                if (active_user_info->getUserType() != administrator)
                    cout << "Your balance are: " << active_user_info->get_balance(sd) << endl;
                else
                    active_user_info->change_discount_rate(sd, goods_database);
                break;
            case 5: //充值
                active_user_info->recharge(sd);
                break;
            case 6: //修改密码
                active_user_info->change_password(sd);
                break;
            case 7: //管理购物车
                active_user_info->manage_shop_cart(sd, goods_database);
                break;
            case 8: //管理商店
                active_user_info->manage_shop_item(sd, goods_database);
                break;
            default:
                break;
            }
        }
        cout << "-----------------------------------------" << endl;
        cout << endl;
    }
    closesocket(sd);
    WSACleanup();
}
