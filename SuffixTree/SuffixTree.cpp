#pragma once

//#include "SuffixTree.h"

#include <string>
#include <map>
#include <limits>


/**
 * To run tests, this define should be used as private methods are also tested
 */
// #define private public

const int INF = std::numeric_limits<int>::max();



/**
 * To go around the Suffix Tree, you should construct your own class, inherited from Visitor,
 * and realize functions, which will be carried out during DFS
 */
struct Visitor
{
    virtual void startNode()
    {}

    virtual void finishNode()
    {}

    /**
     * Parametres are indexes in the string: begin and end, corresponding to the edge.
     */
    virtual void startEdge(int, int)
    {}

    virtual void finishEdge()
    {}
};



class SuffixTree
{
public:
    /**
     * Constructs an empty Suffix Tree
     */
    SuffixTree()
        : str_(""), dummy_(new Node(NULL)),
          root_(new Node(NULL)),
          active_pointer_(TriePointer(root_))
    {
        root_->suffix_link = dummy_;
    }

    /**
     * Constructs a Suffix Tree, initilized with str
     */
    SuffixTree(std::string str)
        : str_(""),
          dummy_(new Node(NULL)),
          root_(new Node(NULL)),
          active_pointer_(TriePointer(root_))
    {
        root_->suffix_link = dummy_;
        addString(str);
    }    

    /**
     * Ukkonen's Algorithm is online, so it is possible to lengthen the string and
     * go on building the tree
     */
    void addString(std::string str)
    {
        size_t index = str_.length();
        str_ += str;
        for (; index < str_.length(); ++index)
        {
            addLetter(str_[index], index);
        }
    }

    /**
     * Starts DFS and calls visitor's methods at the time of every event:
     * coming into the node, coming out the node
     * coming into the edge, coming out the edge
     */
    void accept(Visitor* visitor) const
    {
        Dfs(visitor, root_);
    }

    /**
     * Returns the string, on which the SuffixTree is constructed.
     * It should be needed during DFS by visitor
     */
    std::string mainString() const
    {
        return str_;
    }



private:
    struct Node;

    struct Edge
    {
        Edge(size_t str_begin)
            :end(NULL), str_begin(str_begin)
        {}

        Edge(Node* end, size_t str_begin = 0, size_t str_end = 1)
            :end(end), str_begin(str_begin), str_end(str_end)
        {}

        size_t length()
        {
            if (!end)
                return INF;
            else
                return str_end - str_begin;
        }

        bool leadsToLeaf()
        {
            return !end;
        }

        size_t str_begin;
        size_t str_end;
        Node* end;
    };

    struct Node
    {
        Node(Node* parent)
            : parent(parent)
        {}

        bool existsEdge(char ch) const
        {
            return edges.count(ch);
        }

        Node* parent;
        std::map<char, Edge*> edges;
        Node* suffix_link;
    };

    struct TriePointer
    {
        TriePointer(Node* node)
            :node(node), edge(NULL), lowering(0)
        {}

        TriePointer(Node* node, Edge* edge, int lowering)
            :node(node), edge(edge), lowering(lowering)
        {}

        int strIndex() const
        {
            if (edge)
                return edge->str_begin + lowering;
            else
                return -1;
        }

        bool isNode() const
        {
            return !lowering;
        }

        Node* node;
        Edge* edge;
        size_t lowering; // спуск вниз от вершины
    };

    TriePointer goDown(Node* node, Edge* edge, size_t lowering)
    {
        if (edge->length() == lowering)
            return TriePointer(edge->end);
        else
            return TriePointer(node, edge, lowering);
    }

    TriePointer goDown(Node* node, size_t str_begin, size_t str_end)
    {
        Edge* edge = node->edges[str_[str_begin]];
        if (edge->length() >= str_end - str_begin)
            return goDown(node, edge, str_end - str_begin);
        else
            return goDown(edge->end, str_begin + edge->length(), str_end);
    }

    TriePointer findSuffixLink(Node* node, size_t str_between_parent_and_node_begin,
                               size_t str_between_parent_and_node_end)
    {
        return goDown(node->parent->suffix_link, str_between_parent_and_node_begin,
                      str_between_parent_and_node_end);
    }

    void createEdges(char ch, size_t str_index)
    {
        while (!active_pointer_.node->existsEdge(ch))
        {
            active_pointer_.node->edges.insert(
                        std::make_pair<char, Edge*>(ch, new Edge(str_index)));
            active_pointer_ = active_pointer_.node->suffix_link;
        }
        active_pointer_ = goDown(active_pointer_.node, active_pointer_.node->edges[ch],
                                 active_pointer_.lowering + 1);
    }

    void divideEdgeIntoTwoParts(Edge* active_edge, Node* middle_node, size_t gap_str_index)
    {
        if (active_edge->leadsToLeaf())
        {
            Edge* child_edge = new Edge(gap_str_index);
            middle_node->edges.insert(std::make_pair<char, Edge*>(str_[gap_str_index], child_edge));
        }
        else
        {
            Edge* child_edge = new Edge(active_edge->end, gap_str_index, active_edge->str_end);
            middle_node->edges.insert(std::make_pair<char, Edge*>(str_[gap_str_index], child_edge));
            child_edge->end->parent = middle_node;
        }

        Edge* parent_edge = active_edge;
        *parent_edge = Edge(middle_node, active_edge->str_begin, gap_str_index);
    }

    Node* createNode(char ch, size_t str_index)
    {
        Node* new_node = new Node(active_pointer_.node);
        divideEdgeIntoTwoParts(active_pointer_.edge, new_node, active_pointer_.strIndex());
        new_node->edges.insert(std::make_pair<char, Edge*>(ch, new Edge(str_index)));
        return new_node;
    }

    Node* createNodes(char ch, size_t str_index)
    {
        if (active_pointer_.isNode())
        {
            return active_pointer_.node;
        }

        Node* new_node = createNode(ch, str_index);

        Edge* active_edge = active_pointer_.edge;
        active_pointer_ = findSuffixLink(new_node, active_edge->str_begin, active_edge->str_end);
        new_node->suffix_link = createNodes(ch, str_index);
        return new_node;
    }

    void addLetterToAlphabet(char ch)
    {
        if (!dummy_->existsEdge(ch))
        {
            dummy_->edges.insert(std::make_pair<char, Edge*>(ch, new Edge(root_)));
        }
    }

    void addLetter(char ch, size_t str_index)
    {
        addLetterToAlphabet(ch);

        if (!active_pointer_.isNode() && str_[active_pointer_.strIndex()] == ch)
        {
            active_pointer_ = goDown(active_pointer_.node, active_pointer_.edge,
                                     active_pointer_.lowering + 1);
        }
        else
        {
            createNodes(ch, str_index);

            createEdges(ch, str_index);
        }
    }

    void Dfs(Visitor* visitor, Node* node) const
    {
        visitor->startNode();
        for (std::map<char, Edge*>::iterator it = node->edges.begin();
                                                        it != node->edges.end(); ++it)
        {
            Edge* edge = it->second;
            int str_end;
            if (edge->leadsToLeaf())
                str_end = str_.length();
            else
                str_end = edge->str_end;
            visitor->startEdge(edge->str_begin, str_end);
            if (!edge->leadsToLeaf())
                Dfs(visitor, edge->end);
            visitor->finishEdge();
        }
        visitor->finishNode();
    }



    std::string str_;
    Node* root_;
    Node* dummy_;
    TriePointer active_pointer_;
};


