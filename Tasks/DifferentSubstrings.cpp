#pragma once

#include "suffix_tree.h"
#include "find_occurrences.h"
#include <string>
#include <iostream>

class MaxLengthsOfDifferentSubstringsFinder
{
public:
    MaxLengthsOfDifferentSubstringsFinder(int string_length)
        :string_length_(string_length)
    {}

    void InitVisitor()
    {
        number_of_different_substrings_ = 0;
    }

    void DiscoverNode(const SuffixTree::Link& in_link)
    {}

    void ReturnToNode(const SuffixTree::Link& return_link, const SuffixTree::Link& in_link)
    {}

    void ExamineEdge(const SuffixTree::Link& link)
    {
        number_of_different_substrings_ +=
                std::min(link.sample_end_index, string_length_) - link.sample_start_index;
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
    {}

    int getNumberOfDifferentSubstrings()
    {
        return number_of_different_substrings_;
    }

private:
    int number_of_different_substrings_;
    int string_length_;
};

int findMaxLengthsOfDifferentSubstrings(const std::string& string)
{
    SuffixTree tree;
    tree.AppendSample(string);
    MaxLengthsOfDifferentSubstringsFinder visitor(string.length());
    tree.DepthFirstSearchTraversal(&visitor);
    return visitor.getNumberOfDifferentSubstrings();
}

