
/*
#include<unordered_map>
#include<vector>
#include<float.h>
#include<algorithm>
*/
//10升, 6升, 5升,4升
#include <iostream>
#include <map>
#include <limits>
int capacity[] = { 10, 6, 5, 4 };
class state
{
public:
    state(int _b10, int _b6, int _b5, int _b4)
        :count(0), value(_b10 * 1000 + _b6 * 100 + _b5 * 10 + _b4)
    {
    }
    bool operator<(state rhs)const
    {
        return value < rhs.value;
    }
    int getvalue()const
    {
        return value;
    }
    short count;
protected:
    short value;
};
std::map<int,state> open, close;
void int2array(int content[], state s)
{
    int svalue = s.getvalue();
    content[0] = svalue / 1000;
    content[1] = svalue % 1000 / 100;
    content[2] = svalue % 100 / 10;
    content[3] = svalue % 10;
}
int array2int(int content[])
{
    return content[0] * 1000 + content[1] * 100
        + content[2] * 10 + content[3];
}
//一步转移状态
void derive(std::pair<int, state> sp)
{
    int c[4], t[4];
    int2array(c, sp.second);
    for (int i = 0; i < 4; ++i) // the bottle to pour from
    {
        if (c[i] == 0)
            continue; // no action on empty bottle
        for (int j = 0; j < 4; ++j) // the bottle to pour to
        {
            if (j == i || c[j] == capacity[j]) // don't pour to self or full bottle
                continue;
            //now we can derive a new state
            //state s=*this;
            //++s.count;
            int balance = capacity[j] - c[j];
            t[0] = c[0];
            t[1] = c[1];
            t[2] = c[2];
            t[3] = c[3];
            if (c[i] > balance) // source has more than target can hold
                t[i] -= balance, t[j] = capacity[j];
            else // target can hold everything currently in source
                t[j] += t[i], t[i] = 0;
            state si(t[0], t[1], t[2], t[3]);
            si.count = sp.second.count + 1;
            auto p = open.find(array2int(t));
            auto q = close.find(array2int(t));
            if (p != open.end()) // might encounter a better state
            {
                if (p->second.count > si.count)
                    p->second.count = si.count;
            }
            else if (q == close.end())
            {
                open.insert(std::pair<int, state>(array2int(t),si));
            }
        }
    }
}
int main() {
    int n = 4;
    int input;
    int min = INT_MAX;
    int initvalue = 1;
    //输入初始状态
    open.insert(std::pair<int, state>(10000, state(10, 0, 0, 0)));
    //输入的四个数变为一个数
    for (int i = 0; i < n; i++)
    {
        std::cin >> input;
        if (i == 0)
            initvalue = input;
        else
            initvalue = initvalue * 10 + input;
    }
    //穷搜整个空间,取出open状态加入close，新状态加入open
    while (!open.empty())
    {
        auto p = open.begin();
        auto s = *p;
        close.insert(s);
        open.erase(p);
        derive(s);
    }
    //printf 
    for (auto i = close.begin(); i != close.end(); ++i)
    {
        std::cout << i->first << " " << i->second.count << std::endl;
    }
    auto i = close.find(initvalue);
    if(i != close.end())
        if (min > i->second.count)
            min = i->second.count;
    if (min == INT_MAX)
        std::cout << -1 << std::endl;
    std::cout << min << std::endl;
    return 0;
}

// tasks.json 用来做编译， launch.json用来做执行编译好的文件
//C++语言库的位置：Library/Developer/CommandLineTools/usr/include/c++/v1/