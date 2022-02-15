#ifndef USER_H
#define USER_H

#include <iostream>
#include <cstring>
#include <vector>
#include "json.hpp"
#include "commodity.h"
#include <ctime>
#include <winsock2.h>
using namespace std;
using json = nlohmann::json;
enum user_type
{
    merchant = 0,
    customer = 1,
    administrator = 2
};
class all_user;
class user
{
protected:
    string password;
    SOCKET sd;
    user_type type;
    double balance;
    int find_id_in_shopcart(int id); //返回在当前购物车内的位置
    string user_id;
    user(){};
    explicit user(SOCKET sd, vector<pair<goods *, int>> shop_cart, string id = "0", string p = "0", user_type t = customer, double b = 0);

public:
    const void print_shop_cart();             //打印购物车所有物品信息
    vector<pair<goods *, int>> shopping_cart; //用户购物车
    const virtual user_type getUserType() = 0;
    double get_balance(SOCKET sd);
    void recharge(SOCKET sd);
    const string get_user_id() { return user_id; };
    const string get_password() { return password; };
    const static double check_num(double min, double max);                        //检查数字是否在min到max之间
    void add_shop_cart(goods *tmp_good, all_item database, SOCKET sd);            //新增一条购物车物品
    vector<pair<goods *, int>> get_all_cart(SOCKET sd, all_item *goods_database); //向服务器发送请求，获取购物车信息，并更新
    void manage_shop_cart(SOCKET sd, all_item database);                          //管理购物车
    const int check_format(string s);                                             //返回值为0表示格式正确，为1表示格式错误，为2表示退出。
    user *log_in(SOCKET tmp_sd, all_item database);                               //登录
    void sign_in(SOCKET tmp_sd);                                                  //注册
    void change_password(SOCKET sd);                                              //修改密码
    void manage_shop_item(SOCKET sd, all_item goods_database);                    //添加商家商品信息
    void change_discount_rate(SOCKET sd, all_item goods_database);                //改变折扣率
};

class seller : public user
{
public:
    seller(SOCKET sd, vector<pair<goods *, int>> shop_cart, string id = "0", string p = "0", user_type t = merchant, double b = 0) : user(sd, shop_cart, id, p, t, b){};
    const user_type getUserType();
};
class buyer : public user
{
public:
    buyer(SOCKET sd, vector<pair<goods *, int>> shop_cart, string id = "0", string p = "0", user_type t = customer, double b = 0) : user(sd, shop_cart, id, p, t, b){};
    const user_type getUserType();
};
class admin : public user
{
public:
    admin(SOCKET sd, vector<pair<goods *, int>> shop_cart, string id = "0", string p = "0", user_type t = customer, double b = 0) : user(sd, shop_cart, id, p, t, b){};
    const user_type getUserType();
};

#endif //USER_H
