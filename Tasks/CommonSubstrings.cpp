#pragma once

#include "suffix_tree.h"
#include "find_occurrences.h"
#include <string>
#include <iostream>
#include <unordered_set>

class MaxLengthsOfCommonSubstringsFinder
{
public:
    MaxLengthsOfCommonSubstringsFinder(const SuffixTree& tree, const std::vector<int>& strings_indexes)
        : tree_(tree), whole_string_(tree.sample()), whole_string_length_(tree.sample().length()), strings_indexes_(strings_indexes)
    {
        nodes_.resize(tree.Size(), Node(0));
    }

    void InitVisitor()
    {
        current_depth_ = 0;
        edges_lenghths_.push(0);
    }

    void DiscoverNode(const SuffixTree::Link& in_link)
    {
        nodes_[in_link.target_node_index] = Node(current_depth_);
    }

    void ReturnToNode(const SuffixTree::Link& return_link, const SuffixTree::Link& in_link)
    {
        std::unordered_set<int>* child_reachable_strings = &nodes_[return_link.target_node_index].reachable_strings;
        for (auto it = child_reachable_strings->begin(); it != child_reachable_strings->end(); ++it)
        {
                nodes_[in_link.target_node_index].reachable_strings.insert(*it);
        }
        int outgoing_string_index = strings_indexes_[return_link.sample_start_index];
        nodes_[in_link.target_node_index].reachable_strings.insert(outgoing_string_index);
    }

    void ExamineEdge(const SuffixTree::Link& link)
    {
        int edge_length = std::min(link.sample_end_index, whole_string_length_) - link.sample_start_index;
        edges_lenghths_ .push(edge_length);
        current_depth_ += edge_length;
    }

    SuffixTree::DFSChooseNextNeighbourResult ChooseNextNeighbour(
        int active_node,
        const LinkMapConstIterator& link_map_begin_it,
        const LinkMapConstIterator& link_map_next_letter_it,
        const LinkMapConstIterator& link_map_end_it,
        int suffix_link)
    {
        return SuffixTree::DFSChooseNextNeighbourResult(false, link_map_next_letter_it);
    }

    void FinishNode(const SuffixTree::Link& in_link)
    {
        current_depth_ -= edges_lenghths_.top();
        edges_lenghths_.pop();
    }

    std::vector<int> findMaxLengthsOfCommonSubstrings()
    {
        std::vector<int> result;

        result.resize(strings_indexes_[strings_indexes_.size() - 1] + 2, 0);
        for (int i = 0; i < nodes_.size(); ++i)
        {
            Node node = nodes_[i];
            if (result[node.reachable_strings.size()] < node.depth)
                result[node.reachable_strings.size()] = node.depth;
        }

        int current_max = 0;
        for (int i = result.size() - 1; i >= 0; --i)
        {
            if (current_max > result[i])
                result[i] = current_max;
            else
                current_max = result[i];
        }

        return result;
    }

private:
    struct Node
    {
        Node(int node_depth)
        {
            depth = node_depth;
        }

        int depth;
        std::unordered_set<int> reachable_strings;
    };

    SuffixTree tree_;
    int whole_string_length_;
    std::string whole_string_;
    std::vector<Node> nodes_;
    std::stack<int> edges_lenghths_;
    int current_depth_;
    std::vector<int> strings_indexes_;
};



std::vector<char> findNewLetters(const std::vector<std::string>& strings)
{
    std::unordered_set<char> used_letters;
    std::vector<char> new_letters;
    for (int i = 0; i < strings.size(); ++i)
        for (int j = 0; j < strings[i].length(); ++j)
            used_letters.insert(strings[i][j]);
    char new_char = '!';               // один из первых неслужебных символов
    for (int i = 0; i < strings.size(); ++i)
    {
        while (used_letters.count(new_char))
        {
            new_char++;
        }
        new_letters.push_back(new_char);
        new_char++;
    }
    return new_letters;
}



std::vector<int> findMaxLengthsOfCommonSubstrings(const std::vector<std::string>& strings)
{
    std::vector<char> new_letters = findNewLetters(strings);
    std::string whole_string;
    std::vector<int> strings_indexes;
    int whole_string_size;
    for (int i = 0; i < strings.size(); ++i)
        whole_string_size += strings[i].size() + 1;
    whole_string.reserve(whole_string_size);
    for (int i = 0; i < strings.size(); ++i)
    {
        whole_string += strings[i] + new_letters[i];
        for (int j = 0; j <= strings[i].length(); ++j)
            strings_indexes.push_back(i);
    }

    SuffixTree tree;
    tree.AppendSample(whole_string);

    MaxLengthsOfCommonSubstringsFinder visitor(tree, strings_indexes);
    tree.DepthFirstSearchTraversal(&visitor);
    std::vector<int> visitors_answer = visitor.findMaxLengthsOfCommonSubstrings();
    std::vector<int> answer_without_first_two_elements;
    for (int i = 2; i < visitors_answer.size(); ++i)
        answer_without_first_two_elements.push_back(visitors_answer[i]);
    return answer_without_first_two_elements;
}

