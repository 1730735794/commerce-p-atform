#include "user.h"
#include "commodity.h"
#include <winsock.h>
user::user(SOCKET cur_sd, vector<pair<goods *, int>> shop_cart, string id, string p, user_type t, double b)
{
    shopping_cart = shop_cart;
    balance = b;
    user_id = id;
    password = p;
    type = t;
}
const user_type admin::getUserType()
{
    return administrator;
}
const user_type seller::getUserType()
{
    return merchant;
}
const user_type buyer::getUserType()
{
    return customer;
}
double user::get_balance(SOCKET sd)
{
    char *request = new char[2]{0};
    request[0] = 0;
    request[1] = 9;
    send(sd,request,2,0);
    delete[] request;
    char * answer = new char[10]{0};
    recv(sd, answer, 10, 0);
    memcpy(&balance, answer + 2, 8);
    delete[] answer;
    return balance;
}
const int user::check_format(string s)
{
    if (s == "0")
        return 1;
    if (s.length() < 2 || s.length() > 8)
    {
        cout << "illegal length.(length must be between 2 and 8)" << endl;
        return 1;
    }
    else
    {
        for (int i = 0; i < s.length(); i++)
        {
            if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))
                ;
            else
            {
                cout << "illegal character!String could only be made of upper and lower case letters and numbers." << endl;
                return 1;
            }
        }
    }
    return 0;
}
void user::change_discount_rate(SOCKET sd, all_item goods_database)
{
    if (!user_id.size())
    {
        cout << "Please log first!" << endl;
        return;
    }
    else if (type != administrator)
    {
        cout << "Not enough permissions!" << endl;
        return;
    }
    char *change_discount = new char[14]{0};
    change_discount[0] = 0;
    change_discount[1] = 14;
    cout << "Please enter the goods' type." << endl;
    cout << "1.book" << endl
         << "2.cloth" << endl
         << "3.e_product" << endl;
    int cur_type = check_num(1, 3);
    memcpy(change_discount + 2, &cur_type, 4);
    cout << "Please enter the new discount." << endl;
    double discount = check_num(0, 1.0);
    memcpy(change_discount + 6, &discount, 8);
    send(sd, change_discount, 14, 0);
    char *recv_check = new char[3]{0};
    recv(sd, recv_check, 3, 0);
    if (recv_check[0] == 1 && recv_check[1] == 14)
    {
        if (recv_check[2] == 0)
            cout << "Success!" << endl;
        else
            cout << "Failed!" << endl;
    }
    delete[] recv_check;
    goods_database.change_discount(cur_type, discount);
}
int user::find_id_in_shopcart(int id)
{
    for (int i = 0; i < shopping_cart.size(); i++)
    {
        if (shopping_cart[i].first->get_item_id() == id)
        {
            return i;
        }
    }
    return -1;
}
const void user::print_shop_cart()
{
    cout << left << setw(8) << "item_id";
    cout << left << setw(5) << "type";
    cout << left << setw(30) << "name";
    cout << left << setw(10) << "choose_num";
    cout << left << setw(10) << "left_num";
    cout << left << setw(10) << "cur_price";
    cout << left << setw(10) << "merchant";
    cout << endl;
    for (int i = 0; i < shopping_cart.size(); i++)
    {
        cout << left << setw(8) << shopping_cart[i].first->get_item_id();
        cout << left << setw(5) << shopping_cart[i].first->get_type();
        cout << left << setw(30) << shopping_cart[i].first->get_name();
        cout << left << setw(10) << shopping_cart[i].second;
        cout << left << setw(10) << shopping_cart[i].first->get_left_num();
        cout << left << setw(10) << shopping_cart[i].first->get_price();
        cout << left << setw(10) << shopping_cart[i].first->get_seller_id();
        cout << endl;
    }
}
const double user::check_num(double min, double max)
{
    double num;
    cout << "Please enter num between " << min << " and " << max << endl;
    cin >> num;
    while (num > max || num < min || cin.fail())
    {
        if (cin.fail())
        {
            cin.clear();
            cin.clear();
            cin.ignore(100, '\n');
            cout << "You can only enter numbers.";
        }
        else if (num > max)
            cout << "Too Big!";
        else
            cout << "Too Small!";
        cout << "Please re-enter num between " << min << "and" << max << endl;
        cin >> num;
    }
    return num;
}
vector<pair<goods *, int>> user::get_all_cart(SOCKET sd, all_item *goods_database)
{
    char *request_for_cart = new char[2];
    request_for_cart[0] = 0;
    request_for_cart[1] = 101;
    send(sd, request_for_cart, 2, 0);
    delete[] request_for_cart;

    char *cart_mess = new char[1000];
    int cart_length = recv(sd, cart_mess, 1000, 0);
    vector<pair<goods *, int>> cur_shop_cart;
    if (cart_mess[0] == 1 && cart_mess[1] == 101)
    {
        int offset = 2;
        while (offset < cart_length)
        {
            int have_num;
            memcpy(&have_num, cart_mess + offset + 64, 4);
            cur_shop_cart.push_back(make_pair(goods_database->get_item(cart_mess, offset), have_num));
            offset += 68;
        }
    }
    delete[] cart_mess;
    return cur_shop_cart;
}
void user::sign_in(SOCKET tmp_sd)
{
    bool success_flag = 0;
    user *cur_user;
    while (!success_flag)
    {
        cout << "Please select user type." << endl
             << "0.merchant" << endl
             << "1.customer" << endl;
        int cur_type = cur_user->check_num(0, 1);
        cout << "Please enter your user id:" << endl;
        string id;
        cin >> id;
        bool format_flag = 0;
        while ((format_flag = cur_user->check_format(id)) == 1)
        {
            if (id == "0")
                return;
            cout << "Please re-enter or input '0' to back to main page." << endl;
            cin >> id;
        }
        cout << "Please enter your user password:" << endl;
        string cur_password;
        cin >> cur_password;
        while (format_flag = cur_user->check_format(cur_password))
        {
            if (cur_password == "0")
                return;
            cout << "Please re-enter or input '0' to back to main page." << endl;
            cin >> cur_password;
        }
        char *buf = new char[30]{0};
        buf[0] = 0;
        buf[1] = 10;
        memcpy(buf + 2, &cur_type, 4);
        memcpy(buf + 6, (id + '\0').c_str(), id.length() + 1);
        memcpy(buf + 15, (cur_password + '\0').c_str(), cur_password.length() + 1); //有结束符0,故id和密码长度最大为9
        send(tmp_sd, buf, 30, 0);
        recv(tmp_sd, buf, 3, 0);
        if (buf[0] == 1 && buf[1] == 10)
        {
            if (buf[2] == 1)
                cout << "This ID already exists!" << endl;
            else
            {
                cout << "Sign in successfully!" << endl;
                break;
            }
            cout << "Please re-enter or input '0' to back to main page." << endl;
        }
        delete[] buf;
    }
}
void user::manage_shop_item(SOCKET sd, all_item goods_database)
{
    if (this == NULL || !this->get_user_id().size())
        cout << "Please log in first!" << endl;
    else if (getUserType() != merchant)
    {
        cout << "You are not merchant." << endl;
    }
    else
    {
        int i = 0;
        char *request_shop_mess = new char[2]{0};
        request_shop_mess[0] = 0;
        request_shop_mess[1] = 102;
        send(sd, request_shop_mess, 2, 0);
        delete[] request_shop_mess;

        vector<goods *> myCommodity;
        char *recv_shop_mess = new char[2000]{0};
        int recv_length = recv(sd, recv_shop_mess, 2000, 0);
        if (recv_shop_mess[0] == 1 && recv_shop_mess[1] == 102)
        {
            int offset = 2;
            while (offset < recv_length)
            {
                myCommodity.push_back(goods_database.get_item(recv_shop_mess, offset));
                offset += 64;
            }
        }
        cout << "Your goods are:" << endl;
        goods_database.print_all(myCommodity);
        delete[] recv_shop_mess;
        cout << "Please select the specific operation: " << endl;
        cout << "1.add new item." << endl;
        cout << "2.change item message." << endl;
        int operation = check_num(1, 3);
        switch (operation)
        {
        case 1:
        {
            char *request_add = new char[70]{0};
            request_add[0] = 0;
            request_add[1] = 12;
            int cur_type;
            string cur_name;
            double cur_price;
            int cur_left_num;
            cout << "Please enter the goods' type." << endl;
            cout << "1.book" << endl
                 << "2.cloth" << endl
                 << "3.e_product" << endl;
            cur_type = check_num(1, 2);
            cout << "Please enter the goods' name or enter '0' back to  main page." << endl;
            cin >> cur_name;
            while (check_format(cur_name))
            {
                if (cur_name == "0")
                    return;
                cin >> cur_name;
                cout << "Please re-enter the goods' name or enter '0' back to  main page." << endl;
            }
            cout << "Please enter the goods' price." << endl;
            cur_price = check_num(0, max_goods_price);
            cout << "Please enter the goods' left num." << endl;
            cur_left_num = check_num(0, max_left_num);
            goods *tmp;
            switch (cur_type)
            {
            case book:
                tmp = new book_goods((long long)0, cur_name, cur_type, cur_left_num, cur_price, user_id, &goods_database);
                break;
            case cloth:
                tmp = new cloth_goods((long long)0, cur_name, cur_type, cur_left_num, cur_price, user_id, &goods_database);
                break;
            case e_product:
                tmp = new e_product_goods((long long)0, cur_name, cur_type, cur_left_num, cur_price, user_id, &goods_database);
                break;
            }
            tmp->add_message(request_add, 2);
            send(sd, request_add, 66, 0);
            delete[] request_add;
            break;
        }
        case 2:
        {
            char *request_change = new char[20]{0};
            request_change[0] = 1;
            request_change[1] = 13;
            cout << "Please enter the id of the item you want to change." << endl;
            long long id = check_num(1001, 1000 + goods_database.base_goods_info.size());
            while (goods_database.base_goods_info[id - 1001]->get_seller_id() != user_id)
            {
                if (id == 0)
                    return;
                cout << "It's not your item. Please re-enter or enter '0' to back to main page." << endl;
                cin >> id;
            }
            memcpy(request_change + 3, &id, 8);
            cout << "Please choose the message you want to change:" << endl
                 << "1.price" << endl
                 << "2.left_num" << endl;
            int operation = check_num(1, 2);
            long long mess;
            if (operation == 1)
            {
                request_change[2] = 0;
                cout << "Please enter the goods' price." << endl;
                mess = check_num(0, max_goods_price);
                memcpy(request_change + 11, &mess, 8);
            }
            else if (operation == 2)
            {
                request_change[2] = 1;
                cout << "Please enter the goods' left_num." << endl;
                mess = check_num(0, max_left_num);
                memcpy(request_change + 11, &mess, 8);
            }
            send(sd, request_change, 20, 0);
            break;
        }
        default:
            break;
        }
        char *recv_check = new char[3]{0};
        recv(sd, recv_check, 3, 0);
        if (recv_check[0] == 1 && (recv_check[1] == 12 || recv_check[1] == 13))
        {
            if (recv_check[2] == 0)
                cout << "Success!" << endl;
            else
                cout << "Failed!" << endl;
        }
        delete[] recv_check;
    }
}
void user::change_password(SOCKET sd)
{
    cout << "Please input your current password!" << endl;
    string p;
    cin >> p;
    while (p != password)
    {
        if (p == "0")
            return;
        cout << "Wrong password!" << endl;
        cout << "Please re-enter or input '0' to back to main page." << endl;
        cin >> p;
    }
    cout << "Please input your new password!" << endl;
    cin >> p;
    while (check_format(p))
    {
        if (p == "0")
            return;
        cout << "Please re-enter or input '0' to back to main page." << endl;
        cin >> p;
    }
    password = p;
    char *buf = new char[11]{0};
    buf[0] = 0;
    buf[1] = 11;
    memcpy(buf + 2, (password + '\0').c_str(), password.length() + 1);
    send(sd, buf, 11, 0);
    delete[] buf;
    char *check = new char[3]{0};
    recv(sd, check, 3, 0);
    if (check[0] == 1 && check[1] == 11 && check[2] == 0)
        cout << "Change password successfully!" << endl;
    else
    {
        cout << "Change password Failed!Please try again" << endl;
    }
    delete[] check;
}
void user::recharge(SOCKET sd)
{
    //发送获取当前用户余额的请求
    char *consult_balance = new char[2]{0};
    consult_balance[0] = 0;
    consult_balance[1] = 9;
    send(sd, consult_balance, 2, 0);
    delete[] consult_balance;
    //获取当前用户余额
    char *recv_balance = new char[10]{0};
    recv(sd, recv_balance, 10, 0);
    double cur_balance;
    memcpy(&cur_balance, recv_balance + 2, 8);
    delete[] recv_balance;

    cout << "Your current balance is: " << balance << endl;
    cout << "The max sum you could recharge is: " << INT_MAX - balance << endl;
    cout << "Please enter the amount you want to recharge: " << endl;
    int num = check_num(0, INT_MAX - balance);
    //发送充值请求
    char *recharge_request = new char[6]{0};
    recharge_request[0] = 0;
    recharge_request[1] = 8;
    memcpy(recharge_request + 2, &num, 4);
    send(sd, recharge_request, 6, 0);
    delete[] recharge_request;
    //获取充值结果
    char *recharge_answer = new char[3]{0};
    recv(sd, recharge_answer, 3, 0);
    if (recharge_answer[2] == 0)
    {
        balance += num;
        cout << "Recharge Successfully!Your current balance is: " << balance << endl;
    }
    else
    {
        cout << "Failed!Please try again!" << endl;
    }
    delete[] recharge_answer;
}
void user::add_shop_cart(goods *cur_good, all_item database, SOCKET sd)
{
    for (int i = 0; i < shopping_cart.size(); i++)
        if (shopping_cart[i].first->get_item_id() == cur_good->get_item_id())
        {
            cout << "This item is already in your shopping cart!Please delete it first!" << endl;
            return;
        }
    char *get_item_mess = new char[10]{0};
    get_item_mess[0] = 0;
    get_item_mess[1] = 2;
    long long tmp_item_id = cur_good->get_item_id();
    memcpy(get_item_mess + 2, &tmp_item_id, 8);
    send(sd, get_item_mess, 10, 0);
    delete[] get_item_mess;

    char *recv_item_mess = new char[70]{0};
    recv(sd, recv_item_mess, 70, 0);
    cur_good = database.get_item(recv_item_mess, 2);
    delete[] recv_item_mess;
    cout << "Please enter the num you want to add to your shopping cart." << endl;
    int num = check_num(0, cur_good->get_left_num());

    char *add_to_cart = new char[14]{0};
    add_to_cart[0] = 0;
    add_to_cart[1] = 7;
    tmp_item_id = cur_good->get_item_id();
    memcpy(add_to_cart + 2, &tmp_item_id, 8);
    memcpy(add_to_cart + 10, &num, 4);
    send(sd, add_to_cart, 14, 0);
    delete[] add_to_cart;
    char *check_add_to_cart = new char[3]{0};
    recv(sd, check_add_to_cart, 3, 0);
    if (check_add_to_cart[2] == 0)
    {
        shopping_cart.push_back(make_pair(cur_good, num));
        cout << "Successfully!" << endl;
    }
    else
    {
        cout << "Failed!Please try again!" << endl;
    }
    delete[] check_add_to_cart;
}
user *user::log_in(SOCKET tmp_sd, all_item goods_database)
{
    vector<pair<goods *, int>> tmp_debug_cart;
    buyer tmp_debug(tmp_sd, tmp_debug_cart);
    user *cur_user = &tmp_debug;
    int position;
    bool success_flag = 0;
    while (!success_flag)
    {
        cout << "Please enter your user id:" << endl;
        string id;
        cin >> id;
        bool format_flag = 0;
        while ((format_flag = cur_user->check_format(id)) == 1)
        {
            if (id == "0")
            {
                cur_user = &tmp_debug;
                return cur_user;
            }
            cout << "Please re-enter or input '0' to back to main page." << endl;
            cin >> id;
        }
        cout << "Please enter your user password:" << endl;
        string cur_password;
        cin >> cur_password;
        while ((format_flag = cur_user->check_format(cur_password)) == 1)
        {
            if (cur_password == "0")
            {
                cur_user = &tmp_debug;
                return cur_user;
            }
            cout << "Please re-enter or input '0' to back to main page." << endl;
            cin >> cur_password;
        }
        char *buf = new char[30]{0};
        buf[0] = 0;
        buf[1] = 1;
        memcpy(buf + 2, (id + '\0').c_str(), id.length() + 1);
        memcpy(buf + 11, (cur_password + '\0').c_str(), cur_password.length() + 1); //有结束符0,故id和密码长度最大为9
        send(tmp_sd, buf, 30, 0);
        recv(tmp_sd, buf, 30, 0);
        if (buf[0] == 1)
        {
            if (buf[1] == 0)
            {
                success_flag = 1;
                int cur_type;
                memcpy(&cur_type, buf + 2, 4);
                double cur_balance;
                memcpy(&cur_balance, buf + 6, 8);
                vector<pair<goods *, int>> cur_shop_cart = get_all_cart(tmp_sd, &goods_database);
                if (cur_type == merchant)
                    cur_user = new seller(tmp_sd, cur_shop_cart, id, cur_password, (user_type)cur_type, cur_balance);
                else if (cur_type == customer)
                    cur_user = new buyer(tmp_sd, cur_shop_cart, id, cur_password, (user_type)cur_type, cur_balance);
                else if (cur_type == administrator)
                    cur_user = new admin(tmp_sd, cur_shop_cart, id, cur_password, (user_type)cur_type, cur_balance);
                cout << "Log in successfully!" << endl;
                break;
            }
            else if (buf[1] == 1)
                cout << "This ID not exists!" << endl;
            else if (buf[1] == 2)
                cout << "Wrong password!" << endl;
        }
        delete[] buf;
    }
    return cur_user;
}
void user::manage_shop_cart(SOCKET sd, all_item goods_database)
{
    int ope_code = 1;
    while (ope_code)
    {
        shopping_cart = get_all_cart(sd, &goods_database);
        print_shop_cart();
        cout << "What do you want to do?" << endl;
        cout << "0.back to main page" << endl
             << "1.create order" << endl
             << "2.delete an item" << endl;
        ope_code = check_num(0, 2);
        //创建订单
        if (ope_code == 1)
        {
            vector<pair<goods *, int>> order;
            int continue_flag = 2;
            while (continue_flag == 2)
            {
                cout << "Please enter the item_id you want to buy.(Please confirm the number before)" << endl;
                int item_id = check_num(1001, 1000 + goods_database.base_goods_info.size());
                int position = find_id_in_shopcart(item_id);
                if (position == -1)
                    cout << "This good is not exist in your shopping cart." << endl;
                else
                {
                    if (shopping_cart[position].first->get_left_num() < shopping_cart[position].second)
                        cout << "This goods doesn't have enough now!Now left_num is: " << shopping_cart[position].second << " .Please del this item from your shopping cart and add it again!" << endl;
                    else
                        order.push_back(shopping_cart[position]);
                }
                cout << "Add more item?" << endl
                     << "0.No,Perform other operations." << endl
                     << "1.No,create order right now." << endl
                     << "2.Yes" << endl;
                continue_flag = check_num(0, 2);
            }
            if (!continue_flag)
                continue;
            if (!order.size())
            {
                cout << "There is nothing in your order!" << endl;
                continue;
            }
            //生成订单,并发送给服务端
            long long total_sum = 0;
            char *check_order_left_num = new char[500]{0};
            check_order_left_num[0] = 0;
            check_order_left_num[1] = 3;
            int offset = 2;
            for (int i = 0; i < order.size(); i++)
            {
                long long order_item_id = order[i].first->get_item_id();
                int order_buy_num = order[i].second;
                memcpy(check_order_left_num + offset, &order_item_id, 8);
                memcpy(check_order_left_num + offset + 8, &order_buy_num, 4);
                offset += 12;
            }
            send(sd, check_order_left_num, order.size() * 12 + 2, 0);
            delete[] check_order_left_num;
            //检查订单是否生成成功
            char *check_order_success = new char[3]{0};
            recv(sd, check_order_success, 3, 0);
            if (check_order_success[0] == 1 && check_order_success[1] == 3)
            {
                if (check_order_success[2] == 0)
                    for (int i = 0; i < order.size(); i++)
                    {
                        goods_database.base_goods_info[order[i].first->get_item_id() - 1001]->decrease_item(order[i].second);
                        total_sum += order[i].first->get_price() * order[i].second;
                    }
                else
                {
                    cout << "Create order failed!(Maybe goods' left_num has been changed,Please try again)" << endl;
                    continue;
                }
            }
            else
            {
                cout << "Error!" << endl;
                exit(0);
            }
            delete[] check_order_success;
            //记录建立订单的时间
            time_t order_create = time(NULL);
            cout << "Order has created!The total sum is:" << total_sum << endl;
            cout << "What do you want to do now?" << endl;
            cout << "1.pay for it" << endl
                 << "2.cancel" << endl;
            int pay_flag = check_num(1, 2);
            int cancel_flag = 0;
            //如果选择支付
            if (pay_flag == 1)
            {
                cout << "Your balance is: " << balance << " The total of amount of order is: " << total_sum << endl;
                while (pay_flag == 1)
                {
                    time_t now = time(NULL);
                    cout << now - order_create << endl;
                    if (now - order_create > 30)
                    {
                        cout << "Order time out!" << endl;
                        pay_flag = 2;
                    }
                    else if (balance < total_sum)
                    {
                        cout << "You don't have enough balance,please recharge first" << endl;
                        cout << "What do you want to do now?" << endl;
                        cout << "1.recharge" << endl
                             << "2.cancel order" << endl;
                        int recharge_flag = check_num(1, 2);
                        if (recharge_flag)
                            recharge(sd);
                        else
                            pay_flag = 2;
                    }
                    else if (balance >= total_sum)
                    {
                        cout << "paying......" << endl;
                        balance -= total_sum;
                        char *pay_for_order = new char[2]{0};
                        pay_for_order[0] = 0;
                        pay_for_order[1] = 4;
                        send(sd, pay_for_order, 2, 0);
                        delete[] pay_for_order;

                        char *check_pay = new char[3]{0};
                        recv(sd, check_pay, 3, 0);
                        if (check_pay[0] == 1 && check_pay[1] == 4)
                        {
                            if (check_pay[2] == 0)
                            {
                                cout << "Pay successfully!" << endl;
                                for (int i = 0; i < order.size(); i++)
                                {
                                    int cart_position = find_id_in_shopcart(order[i].first->get_item_id());
                                    if (cart_position < shopping_cart.size() - 1)
                                        shopping_cart.erase(shopping_cart.begin() + cart_position, shopping_cart.begin() + cart_position + 1);
                                    else
                                        shopping_cart.pop_back();
                                }
                                pay_flag = 3; //表示订单成功支付
                            }
                            else
                            {
                                cout << "Pay failed!(Order time out may cause that)Please try again" << endl;
                                pay_flag = 2;
                                continue;
                            }
                        }
                        delete[] check_pay;
                    }
                }
            }
            if (pay_flag == 2)
            {
                char *order_cancel = new char[2]{0};
                order_cancel[0] = 0;
                order_cancel[1] = 5;
                send(sd, order_cancel, 2, 0);
                delete[] order_cancel;
                for (int i = 0; i < order.size(); i++)
                    goods_database.base_goods_info[order[i].first->get_item_id() - 1001]->increase_item(order[i].second);
                cout << "Order has be canceled." << endl;
            }
            order.clear();
        }
        else if (ope_code == 2)
        {
            cout << "Please enter the item_id of the item you want to delete." << endl;
            int item_id = check_num(1001, 1000 + goods_database.base_goods_info.size());
            int position = find_id_in_shopcart(item_id);
            if (position == -1)
                cout << "This good is not exist in your shopping cart." << endl;
            else
            {
                char *del_shop_cart = new char[10]{0};
                del_shop_cart[0] = 0;
                del_shop_cart[1] = 6;
                long long del_item_id = shopping_cart[position].first->get_item_id();
                memcpy(del_shop_cart + 2, &del_item_id, 8);
                send(sd, del_shop_cart, 10, 0);
                delete[] del_shop_cart;
                shopping_cart.erase(shopping_cart.begin() + position, shopping_cart.begin() + position + 1);
                cout << "Delete successfully!" << endl;
            }
        }
    }
}