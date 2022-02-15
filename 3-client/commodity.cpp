#include "commodity.h"
goods::goods(long long id, string n, int t, int l_num, double p, string s_i, all_item* database)
{
    item_id = id;
    name = n;
    left_num = l_num;
    price = (double)p;
    seller_id = s_i;
    type = (commodity_type)t;
    database_in = database;
}
const double book_goods::get_price()
{
    return price * database_in->book_discount;
}
const double cloth_goods::get_price()
{
    return price * database_in->cloth_discount;
}
const double e_product_goods::get_price()
{
    return price * database_in->e_product_discount;
}
void goods::decrease_item(int x)
{
    left_num -= x;
}
void goods::increase_item(int x)
{
    left_num += x;
}
void goods::add_message(char *buf, int offset)
{
    memcpy(buf + offset, &item_id, 8);
    memcpy(buf + offset + 8, (name + '\0').c_str(), name.length() + 1);
    memcpy(buf + offset + 39, &type, 4);
    memcpy(buf + offset + 43, &left_num, 4);
    memcpy(buf + offset + 47, &price, 8);
    memcpy(buf + offset + 55, (seller_id + '\0').c_str(), seller_id.length() + 1);
} //单条信息长度为64
const void goods::print_item()
{
    cout << left << setw(8) << "item_id";
    cout << left << setw(5) << "type";
    cout << left << setw(30) << "name";
    cout << left << setw(10) << "left_num";
    cout << left << setw(10) << "cur_price";
    cout << left << setw(10) << "merchant";
    cout << endl;
    cout << left << setw(8) << item_id;
    cout << left << setw(5) << type;
    cout << left << setw(30) << name;
    cout << left << setw(10) << left_num;
    cout << left << setw(10) << get_price();
    cout << left << setw(10) << seller_id;
    cout << endl;
}
goods* all_item::get_item(char* buf, int offset)
{
    long long tmp_item_id;
    string tmp_name;
    int tmp_type;
    int tmp_left_num;
    double tmp_price;
    string tmp_seller_id;
    memcpy(&tmp_item_id, buf + offset, 8);
    tmp_name = buf + offset + 8;
    memcpy(&tmp_type, buf + offset + 39, 4);
    memcpy(&tmp_left_num, buf + offset + 43, 4);
    memcpy(&tmp_price, buf + offset + 47, 8);
    tmp_seller_id = buf + offset + 55;
    goods* tmp_good;
    switch (tmp_type)
    {
    case book:
        tmp_good = new book_goods(tmp_item_id,tmp_name,tmp_type,tmp_left_num,tmp_price,tmp_seller_id, this);
        break;
    case cloth:
        tmp_good = new cloth_goods(tmp_item_id,tmp_name,tmp_type,tmp_left_num,tmp_price,tmp_seller_id, this);
        break;
    case e_product:
        tmp_good = new e_product_goods(tmp_item_id,tmp_name,tmp_type,tmp_left_num,tmp_price,tmp_seller_id, this);
        break;
    default:
        break;
    }
    int i = 0;
    while(i < base_goods_info.size() && base_goods_info[i]->get_item_id() != tmp_item_id)
        i++;
    if(i >= base_goods_info.size())
        base_goods_info.push_back(tmp_good);
    else if(base_goods_info[i]->get_item_id() > tmp_item_id)
        base_goods_info.insert(base_goods_info.begin()+ i - 1, 1, tmp_good);
    else if(base_goods_info[i]->get_item_id() ==  tmp_item_id)
        base_goods_info[i] = tmp_good;
    return tmp_good;
}
all_item::all_item(SOCKET sd)
{
    book_discount = 1.0;
    cloth_discount = 1.0;
    e_product_discount = 1.0;
}
void all_item::get_all_item(SOCKET sd)
{
    char* request = new char[2]{0};
    request[0] = 0;
    request[1] = 100;
    send(sd,request,2, 0);
    delete[] request;
    char* answer = new char[2000];
    int total_len = recv(sd, answer, 2000,0);
    cout << total_len << endl;
    if(answer[0] == 1 && answer[1] == 100)
    {
        int offset = 2;
        while(offset <= total_len - 64)
        {
            get_item(answer,offset);
            offset += 64;
        }
    }
    delete[] answer;
}
goods* all_item::find_item(long long id)
{
    for(int i = 0; i < base_goods_info.size(); i++)
        if(base_goods_info[i]->get_item_id() == id)
            return base_goods_info[i];
    goods* tmp = NULL;
    return tmp;
}
const void all_item::print_all(vector<goods *> to_print)
{
    cout << left << setw(8) << "item_id";
    cout << left << setw(5) << "type";
    cout << left << setw(30) << "name";
    cout << left << setw(10) << "left_num";
    cout << left << setw(10) << "cur_price";
    cout << left << setw(10) << "merchant";
    cout << endl;
    for (int i = 0; i < to_print.size(); i++)
    {
        cout << left << setw(8) << to_print[i]->get_item_id();
        cout << left << setw(5) << to_print[i]->get_type();
        cout << left << setw(30) << to_print[i]->get_name();
        cout << left << setw(10) << to_print[i]->get_left_num();
        cout << left << setw(10) << to_print[i]->get_price();
        cout << left << setw(10) << to_print[i]->get_seller_id();
        cout << endl;
    }
}
const void all_item::search()
{
    vector<goods *> result;
    int flag1;
    cout << "Please select the query method:" << endl;
    cout << "1.commmodity name." << endl;
    cout << "2.seller id." << endl;
    cout << "3.commodity type." << endl;
    cin >> flag1;
    while (flag1 < 0 || flag1 > 3)
    {
        cout << "Please select one above or enter '0' to back to main package." << endl;
        cin >> flag1;
    }
    if (flag1 == 0)
        return;
    else if (flag1 == 1)
    {
        string name;
        cout << "Please enter the query information." << endl;
        cin >> name;
        for (int i = 0; i < base_goods_info.size(); i++)
        {
            if (base_goods_info[i]->get_name() == name)
                result.push_back(base_goods_info[i]);
        }
    }
    else if (flag1 == 2)
    {
        string name;
        cout << "Please enter the query information." << endl;
        cin >> name;
        for (int i = 0; i < base_goods_info.size(); i++)
        {
            if (base_goods_info[i]->get_seller_id() == name)
                result.push_back(base_goods_info[i]);
        }
    }
    else if (flag1 == 3)
    {
        int search_type;
        cout << "Please select the commutity type." << endl;
        cout << "1.book" << endl;
        cout << "2.cloth" << endl;
        cout << "3.e_product" << endl;
        cin >> search_type;
        for (int i = 0; i < base_goods_info.size(); i++)
        {
            if (base_goods_info[i]->get_type() == search_type)
                result.push_back(base_goods_info[i]);
        }
    }
    if (result.size() == 0)
        cout << "Not found!";
    else
        print_all(result);
    return;
}
void all_item::change_discount(int kind, double new_discount)
{
    switch (kind)
    {
    case 1:
        book_discount = new_discount;
        break;
    case 2:
        cloth_discount = new_discount;
        break;
    case 3:
        e_product_discount = new_discount;
        break;
    default:
        break;
    }
}