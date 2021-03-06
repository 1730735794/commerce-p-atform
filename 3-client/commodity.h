#ifndef COMMODITY_H
#define COMMODITY_H
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <winsock2.h>
const int max_goods_price = 100000000;
const int max_left_num = 100000;
using namespace std;
using json = nlohmann::json;
const int commodity_type_num = 3;
const int offset = 1001;
class all_item;
enum commodity_type
{
    book = 0,
    cloth = 1,
    e_product = 2
};
class goods
{
protected:
    long long item_id;
    string name;
    commodity_type type;
    int left_num;
    double price;
    string seller_id;
    all_item *database_in;

public:
    goods(long long item_id, string name, int type, int left_num, double price, string seller_id, all_item *database_in);
    const virtual double get_price() = 0;
    const commodity_type get_type() { return type; };
    const string get_name() { return name; };
    const string get_seller_id() { return seller_id; };
    const int get_item_id() { return item_id; };
    const int get_left_num() { return left_num; };
    void add_message(char *buf, int offset);
    const void print_item();
    void decrease_item(int x);
    void increase_item(int x);
};
class book_goods : public goods
{
public:
    book_goods(long long item_id, string name, int type, int left_num, double price, string seller_id, all_item *database_in) : goods(item_id, name, type, left_num, price, seller_id, database_in){};
    const double get_price();
};
class cloth_goods : public goods
{
public:
    cloth_goods(long long item_id, string name, int type, int left_num, double price, string seller_id, all_item *database_in) : goods(item_id, name, type, left_num, price, seller_id, database_in){};
    const double get_price();
};
class e_product_goods : public goods
{
public:
    e_product_goods(long long item_id, string name, int type, int left_num, double price, string seller_id, all_item *database_in) : goods(item_id, name, type, left_num, price, seller_id, database_in){};
    const double get_price();
};
class all_item
{
public:
    double book_discount;
    double cloth_discount;
    double e_product_discount;
    vector<goods *> base_goods_info;
    all_item(SOCKET sd);
    void add_new(goods *new_good);
    const void print_all(vector<goods *> tmp); //????????????????????????
    const void search();                       //??????
    goods *get_item(char *buf, int offset);    //???buf+offset???????????????????????????
    const void reserve_goods();                //??????????????????????????????
    goods *find_item(long long id);            //????????????????????????id???????????????????????????
    void change_discount(int kind, double new_discount);
    void get_all_item(SOCKET sd); //???????????????????????????????????????????????????
};
#endif //COMMODITY_H