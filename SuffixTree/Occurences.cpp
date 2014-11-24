#pragma once

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include "SuffixTree.cpp"



struct MyVisitor : Visitor
{
public:
    /**
     * Constructs visitor.
     * If length of the string is null, it can starts anywhere, so it is already found.
     */
    MyVisitor(std::string str, std::string substr)
        : str(str), substr(substr), flag_substr(SEARCH), substr_height(1), substr_index(0),
          cur_height(0), cur_length(0)
    {
        if (substr.length() == 0)
            flag_substr = FOUND;
    }

    /**
     * Supports information between the state in the tree and the position in the string:
     *
     * cur_height - The number of nodes between current node and the root
     * cur_length - The length of the current substring
     */
    void startNode()
    {
        if (cur_height != 0)
            cur_length += edges_length.top();
        cur_height++;
    }

    /**
     * Supports information between the state in the tree and the position in the string
     *
     * If processing the node is finished, and this was the first node, including the whole
     * substring, then there are no other substrings including our one.
     */
    void finishNode()
    {
        if (cur_height <= substr_height)
            flag_substr = FINISHED;
        cur_height--;
        if (cur_height != 0)
            cur_length -= edges_length.top();
    }

    /**
     * Finds out whether the edge is the part of our substring:
     *
     * If the whole edge is in the substring, then go the next edge and implement 'substr_index'
     * showing how many first symbols of the substring are already found.
     *
     * If the end of the substring is in the egde, then we have found the first node (the end of the egde), concluding
     * the substring.
     *
     * If only parts of the edge and the rest of the substring are the same, then there is no our substring in the tree
     * as the previous node is in the edge, and the only way (defined by the first symbol in the edge) is to go by this edge.
     *
     * If the beginnings of the edge and the rest of the edge are different then we should just go by another node.
     *
     * If the edge is deeper than the current height of the substring than it is a branch, continuing the edge which is
     * not part of the substring, so it is not necessary to work with it.
     *
     *
     * If the substring is already found we look for leaves and add the index of the substring, concerning to this branch:
     * for every entry of the substring there is a suffix starting at the same index
     */
    void startEdge(int str_begin, int str_end)
    {
        edges_length.push(str_end - str_begin);
        if (flag_substr == SEARCH)
        {
            if (substr_height < cur_height)
                return;
            bool flaq_substr_intersects_edge = false;
            for (int index = str_begin; index < str_end && substr_index < substr.length(); ++index)
            {
                if (str[index] == substr[substr_index])
                {
                    flaq_substr_intersects_edge = true;
                    substr_index++;
                }
                else
                {
                    if (flaq_substr_intersects_edge)
                        flag_substr = FINISHED;
                    return;
                }
            }
            goThroughEdge(str_begin, str_end);
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
    void goThroughEdge(size_t str_begin, size_t str_end)
    {
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


/**
 * Returns the vector, included indexes of the entries of the substring into the tree.
 * The tree should be built by the string with the '$' at the end.
 */
std::vector<size_t> findAllOccurences(const SuffixTree tree, std::string substr)
{
    std::string str = tree.mainString();
    MyVisitor my_visitor = MyVisitor(str, substr);
    tree.accept(&my_visitor);

    std::vector<size_t> occurences = my_visitor.returnOccurences();
    return occurences;
}



