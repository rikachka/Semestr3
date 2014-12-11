#pragma once

#include "suffix_tree.h"
#include "find_occurrences.h"
#include <string>
#include <iostream>

class SuffixArrayBuilder
{
public:
    SuffixArrayBuilder(const SuffixTree& tree)
        :tree_(tree), string_length_(tree.sample().length())
    {}

    void InitVisitor()
    {
        current_depth_ = 0;
        edges_lenghths_.push(0);
    }

    void DiscoverNode(const SuffixTree::Link& in_link)
    {}

    void ReturnToNode(const SuffixTree::Link& return_link, const SuffixTree::Link& in_link)
    {}

    void ExamineEdge(const SuffixTree::Link& link)
    {
        int edge_length = std::min(link.sample_end_index, string_length_) - link.sample_start_index;
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
        if (tree_.IsLeaf(in_link.target_node_index) && current_depth_ > 1)
        {
            suffix_array_.push_back(string_length_ - current_depth_);
        }
        current_depth_ -= edges_lenghths_.top();
        edges_lenghths_.pop();
    }

    std::vector<int> getSuffixArray()
    {
        return suffix_array_;
    }

private:
    SuffixTree tree_;
    int string_length_;
    std::vector<int> suffix_array_;
    std::stack<int> edges_lenghths_;
    int current_depth_;
};

std::vector<int> buildSuffixArray(const std::string& string)
{
    SuffixTree tree(string);
    SuffixArrayBuilder visitor(tree);
    tree.DepthFirstSearchTraversal(&visitor);
    return visitor.getSuffixArray();
}

