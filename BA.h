#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
const int M = 5;
const int ceil0 = M / 2 + 1;
enum State
{
    in_node,
    leaf_node
};
struct mydata
{
    string context;
};
struct node
{
    State state;
    int num;        //关键字的个数
    node* child[M + 10]; //指向儿子 in_node 有效
    int child_idx[M + 10];//关键字
    node* bro;
    mydata* data[M + 10]; //记录   leaf_node 有效
};


class BAtree
{
public:
    unordered_map<node*, node*> fa;
    unordered_map<node*, int> fapos; // 非关键字，而是关键字的索引
    node* root;
    node* start;
    BAtree()
    {
        root = nullptr;
    }
    void create_();
    void add_(int idx, mydata* data);
    void delete_(int idx);
    void change_(int idx, mydata* data);
    void balance_(node* now_node);
    void update_(node* now_node);
    mydata* find_(int idx, int* rp, node** ans);
    vector<mydata*> find_interval(int lmin, int rmax);
};
void test_output(node* tree, int n);







