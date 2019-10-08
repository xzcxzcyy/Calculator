//
// Created by Banson on 2019/10/5.
//

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <iterator>

class Node;

typedef Node *pt;

class Node
{
private:
    class Pair
    {
    public:
        int num;
        int level;
        char cmd;

        bool operator<(const Pair &b)
        {
            return (level < b.level);
        }

        bool is_num()
        {
            return (level == 0);
        }

        explicit Pair(int num)
        {
            this->num = num;
            level = cmd = 0;
        }

        Pair(char cmd, int level)
        {
            this->cmd = cmd;
            this->level = level;
            num = 0;
        }
    };

    typedef std::vector<Pair>::iterator it;

    static const int step = 2;

    bool is_num;
    char cmd;
    int data;   // represent either number(num) or level(cmd)
    pt left;
    pt right;

    static int get_code(char ch);
    bool onCreate(const std::vector<Pair> &arr, it begin, it end);

public:
    int calculate();
    bool create(const std::string &str);
    void destroy();
    ~Node();

};

int Node::calculate()
{
    if (is_num)
        return data;
    int num1 = left->calculate();
    int num2 = right->calculate();
    switch (cmd)
    {
        case '+':
            return num1 + num2;
            break;
        case '-':
            return num1 - num2;
            break;
        case '*':
            return num1 * num2;
            break;
        case '/':
            return num1 / num2;
            break;
        default:
            std::cout << "Error occurred when proceeding function calculate()" << std::endl;
            abort();
            break;
    }
}

bool Node::create(const std::string &str)
{
    int length = str.length();
    int addition = 0;
    int num_buff;
    int i = 0;
    std::vector<Pair> arr;
    while (i < length)
    {
        if (str[i] == '(')
            addition += step;
        if (str[i] == ')')
            addition -= step;
        if (addition < 0)
            return false;
        if (std::isdigit(str[i]))
        {
            num_buff = 0;
            int j;
            for (j = i; j < length && isdigit(str[j]); ++j)
            {
                num_buff = num_buff * 10 + (str[j] - '0');
            }
            i = j;
            arr.emplace_back(num_buff);
            continue;
        }
        if (str[i] == '-')
        {
            if (!(arr.end() - 1)->is_num())
            {
                arr.emplace_back(0);
            }
            arr.emplace_back('-', get_code('-') + addition);
        }
        if (str[i] == '+' | str[i] == '*' | str[i] == '/')
        {
            arr.emplace_back(str[i], get_code(str[i]) + addition);
        }
        ++i;
    }
    it begin, end;
    begin = arr.begin();
    end = arr.end();
    return onCreate(arr, begin, end);
}

int Node::get_code(char ch) //TODO: complete this function
{
    switch (ch)
    {
        case '+':
        case '-':
            return 1;
            break;
        case '*':
        case '/':
            return 2;
            break;
        default:
            std::cerr << "Runtime Exception in Node::get_code()" << std::endl;
            abort();
            break;
    }
}

void Node::destroy()
{
    if (is_num)
    {
        return;
    } else
    {
        left->destroy();
        right->destroy();
        return;
    }
}

Node::~Node()
{
    destroy();
}

bool Node::onCreate(const std::vector<Pair> &arr, it begin, it end)
{
    if (begin + 1 == end)
    {
        if (!begin->is_num())
            return false;
        is_num = true;
        data = begin->num;
        left = right = nullptr;
        return true;
    }
    auto p = end;
    auto parent = p - 1;
    while (--p >= begin)
    {
        if (!p->is_num())
        {
            if (parent->is_num())
            {
                parent = p;
            } else
            {
                if (p->level < parent->level)
                    parent = p;
            }

        }
    }
    is_num = false;
    cmd = parent->cmd;
    data = parent->level;
    left = new Node;
    right = new Node;

    return (left->onCreate(arr, begin, parent) && right->onCreate(arr, parent + 1, end));
}


#endif //CALCULATOR_H
