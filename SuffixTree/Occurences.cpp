#pragma once

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include "SuffixTree.cpp"



struct MyVisitor : Visitor
{
public:
    MyVisitor(std::string str, std::string substr)
        :str(str), substr(substr), flag_substr(SEARCH), substr_height(1), cur_height(0), substr_index(0), cur_length(0)
    {
        if (substr.length() == 0)
            flag_substr = FOUND;
    }

    void startNode()
    {
        if (cur_height != 0)
            cur_length += edges_length.top();
        cur_height++;
    }

    void finishNode()
    {
        if (flag_substr == FOUND && cur_height == substr_height)
            flag_substr = FINISHED;
        cur_height--;
        if (cur_height != 0)
            cur_length -= edges_length.top();
    }

    void startEdge(int str_begin, int str_end)
    {
        edges_length.push(str_end - str_begin);
        if (flag_substr == SEARCH)
        {
            if (substr_height < cur_height)
                return;
            bool flaq_substr_edge = false;
            for (int index = str_begin; index < str_end && substr_index < substr.length(); index++)
            {
                if (str[index] == substr[substr_index])
                {
                    flaq_substr_edge = true;
                    substr_index++;
                }
                else
                {
                    if (flaq_substr_edge)
                        flag_substr = FINISHED;
                    return;
                }
            }
            substr_height++;
            if (substr_index == substr.length())
                flag_substr = FOUND;
            if (str_end == str.length())
            {
                if (flag_substr == FOUND)
                    occurences.push_back(str_begin - cur_length);
                flag_substr = FINISHED;
            }
        }
        else if (flag_substr == FOUND)
        {
            if (str_end == str.length())
                occurences.push_back(str_begin - cur_length);
        }
    }

    void finishEdge()
    {
        edges_length.pop();
    }

    std::vector<size_t> returnOccurences()
    {
        return occurences;
    }

private:
    std::string str;
    std::string substr;
    enum {SEARCH, FOUND, FINISHED} flag_substr;
    size_t substr_height;
    size_t cur_height;
    size_t substr_index;
    std::stack<size_t> edges_length;
    size_t cur_length;
    std::vector<size_t> occurences;
};



std::vector<size_t> findAllOccurences(const SuffixTree tree, std::string substr)
{
    std::string str = tree.mainString();
    MyVisitor my_visitor = MyVisitor(str, substr);
    tree.accept(&my_visitor);

    std::vector<size_t> occurences = my_visitor.returnOccurences();
    return occurences;
}



void findAllOccurences()
{
    std::string str;
    std::cout << "Vvedite ishodnuyu stroku: ";
    std::cin >> str;
    str += '$';
    SuffixTree tree = SuffixTree();
    tree.addString(str);

    std::string substr;
    std::cout << "Vvedite pattern: ";
    std::cin >> substr;
    std::vector<size_t> occurences = findAllOccurences(tree, substr);
    for (size_t i = 0; i < occurences.size(); i++)
        std::cout << occurences[i] << " ";
    std::cout << std::endl;
}
