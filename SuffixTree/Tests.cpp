#include <gtest/gtest.h>
#include <SuffixTree.cpp>
#include <Occurences.cpp>

#include <stdlib.h>
#include <algorithm>



#define MAX_LENGTH 100000



std::string initRandomStr(size_t max_length)
{
    std::string str;
    size_t str_length = 1 + rand() % max_length;
    for (size_t i = 0; i < str_length; i++)
    {
        char ch = char(int('a') + rand() % (int('z') - int('a')) );
        str.push_back(ch);
    }
    return str;
}



TEST(GoDownTest, SameNode)
{
    {
        SuffixTree tree = SuffixTree("aaaaa");
        SuffixTree::Node* root = tree.root_;
        SuffixTree::TriePointer trie_pointer = tree.goDown(root, root->edges['a'], 1);
        EXPECT_FALSE(trie_pointer.isNode());
        EXPECT_EQ(1, trie_pointer.lowering);
        EXPECT_EQ(root, trie_pointer.node);
        EXPECT_EQ(root->edges['a'], trie_pointer.edge);
        trie_pointer = tree.goDown(trie_pointer.node, trie_pointer.edge, 2);
        EXPECT_FALSE(trie_pointer.isNode());
        EXPECT_EQ(2, trie_pointer.lowering);
        EXPECT_EQ(root, trie_pointer.node);
        EXPECT_EQ(root->edges['a'], trie_pointer.edge);
    }
}

TEST(GoDownTest, NextNode)
{
    {
        SuffixTree tree = SuffixTree("aab");
        SuffixTree::Node* root = tree.root_;
        SuffixTree::TriePointer trie_pointer = tree.goDown(root, root->edges['a'], 1);
        EXPECT_TRUE(trie_pointer.isNode());
        EXPECT_EQ(0, trie_pointer.lowering);
        EXPECT_EQ(root->edges['a']->end, trie_pointer.node);
    }
}

TEST(GoDownTest, StrIndexes)
{
    std::string str = "abacaabbabab";
    SuffixTree tree = SuffixTree(str);
    SuffixTree::Node* root = tree.root_;
    SuffixTree::Node *cur_node = root, *next_node = root;
    for (size_t i = 0; i < str.length(); i++)
    {
        SuffixTree::TriePointer trie_pointer = tree.goDown(root, size_t(0), i);
        if (!trie_pointer.isNode())
            EXPECT_EQ(str[i], str[trie_pointer.strIndex()]);
        else
        {
            cur_node = next_node;
            EXPECT_EQ(cur_node, trie_pointer.node);
            if (!cur_node->edges[str[i]]->leadsToLeaf())
                next_node = cur_node->edges[str[i]]->end;
        }
    }
}



TEST(SuffixLinkTest, ToRoot)
{
    std::string str = "abacaabbabab";
    SuffixTree tree = SuffixTree(str);
    SuffixTree::Node *root = tree.root_;
    for (std::map<char, SuffixTree::Edge*>::iterator it = root->edges.begin(); it != root->edges.end(); it++)
    {
        if (!it->second->leadsToLeaf())
            EXPECT_EQ(root, it->second->end->suffix_link);
    }
}

TEST(SuffixLinkTest, All)
{
    std::string str = "abacaabbabab";
    SuffixTree tree = SuffixTree(str);
    SuffixTree::Node *root = tree.root_;
    for (size_t i = 0; i < str.length(); i++)
        for (size_t j = i + 1; j < str.length(); j++)
        {
            SuffixTree::TriePointer trie_pointer = tree.goDown(root, i, j);
            if (trie_pointer.isNode())
            {
                SuffixTree::Node* suf_link = tree.goDown(root, i + 1, j).node;
                EXPECT_EQ(suf_link, trie_pointer.node->suffix_link);
            }
        }
}



TEST(CreateEdgesTest, ZeroEdgesCreated)
{
    {
        std::string str = "a";
        char new_letter = 'a';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node, new_pointer.node);
        EXPECT_EQ(old_pointer.lowering + 1, new_pointer.lowering);
    }

    {
        std::string str = "aabaa";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node, new_pointer.node);
        EXPECT_EQ(old_pointer.lowering + 1, new_pointer.lowering);
    }
}

TEST(CreateEdgesTest, OneEdgeCreated)
{
    {
        std::string str = "baaca";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.node->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node->suffix_link, new_pointer.node);
        EXPECT_EQ(1, new_pointer.lowering);
    }

    {
        std::string str = "acaabaaa";
        char new_letter = 'c';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.node->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node->suffix_link, new_pointer.node);
        EXPECT_EQ(1, new_pointer.lowering);
    }
}

TEST(CreateEdgesTest, TwoEdgesCreated)
{
    {
        std::string str = "ababcab";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.node->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_TRUE(old_pointer.node->suffix_link->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node->suffix_link, new_pointer.node);
        EXPECT_EQ(0, new_pointer.lowering);
    }

    {
        std::string str = "abacabaaba";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.node->existsEdge(new_letter));
        EXPECT_FALSE(old_pointer.node->suffix_link->existsEdge(new_letter));
        tree.createEdges(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.node->existsEdge(new_letter));
        EXPECT_TRUE(old_pointer.node->suffix_link->existsEdge(new_letter));
        EXPECT_EQ(old_pointer.node->suffix_link->suffix_link, new_pointer.node);
        EXPECT_EQ(1, new_pointer.lowering);
    }
}



TEST(CreateNodesTest, ZeroNodesCreated)
{
    {
        std::string str = "a";
        char new_letter = 'a';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.isNode());
        tree.createNodes(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_EQ(old_pointer.node, new_pointer.node);
    }

    {
        std::string str = "baaca";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_TRUE(old_pointer.isNode());
        tree.createNodes(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        EXPECT_EQ(old_pointer.node, new_pointer.node);
    }
}

TEST(CreateNodesTest, OneNodeCreated)
{
    {
        std::string str = "baa";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.isNode());
        tree.createNodes(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        old_pointer = tree.goDown(old_pointer.node, old_pointer.edge, old_pointer.lowering);
        EXPECT_TRUE(old_pointer.isNode());
        EXPECT_EQ(old_pointer.node->suffix_link, new_pointer.node);
    }

    {
        std::string str = "abba";
        char new_letter = 'a';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.isNode());
        tree.createNodes(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        old_pointer = tree.goDown(old_pointer.node, old_pointer.edge, old_pointer.lowering);
        EXPECT_TRUE(old_pointer.isNode());
        EXPECT_EQ(old_pointer.node->suffix_link, new_pointer.node);
    }
}

TEST(CreateNodesTest, TwoNodesCreated)
{
    {
        std::string str = "abaacbaa";
        char new_letter = 'b';
        SuffixTree tree = SuffixTree(str);
        SuffixTree::TriePointer old_pointer = tree.active_pointer_;
        EXPECT_FALSE(old_pointer.isNode());
        tree.createNodes(new_letter, str.length() + 1);
        SuffixTree::TriePointer new_pointer = tree.active_pointer_;
        old_pointer = tree.goDown(old_pointer.node, old_pointer.edge, old_pointer.lowering);
        EXPECT_TRUE(old_pointer.isNode());
        EXPECT_EQ(old_pointer.node->suffix_link->suffix_link, new_pointer.node);
    }
}



TEST(AddStringTest, QuickQonstruction)
{
    srand(time(NULL));
    std::string str;
    size_t length = rand() % MAX_LENGTH;
    for (size_t i = 0; i < length; i++)
    {
        char ch = char(int('a') + rand() % (int('z') - int('a')) );
        str.push_back(ch);
    }
    SuffixTree tree = SuffixTree(str);
}

TEST(AddStringTest, ContainsAllSuffixes)
{
    srand(time(NULL));
    std::string str = initRandomStr(MAX_LENGTH);
    str.push_back('$');
    SuffixTree tree = SuffixTree(str);
    for (size_t i = 0; i < str.length(); i++)
    {
        SuffixTree::TriePointer trie_pointer = tree.goDown(tree.root_, i, str.length());
        EXPECT_TRUE(trie_pointer.edge->leadsToLeaf());
    }
}



TEST(FindAllOccurencesTest, SameLetters)
{
    std::string str;
    for(size_t i = 0; i < 10; i++)
        str += 'a';
    str += '$';
    SuffixTree tree = SuffixTree(str);
    std::vector<size_t> result = findAllOccurences(tree, "a");
    std::sort(result.begin(), result.end());
    ASSERT_EQ(str.length() - 1, result.size());
    for (size_t i = 0; i < str.length() - 1; i++)
        EXPECT_EQ(i, result[i]);
}

TEST(FindAllOccurencesTest, EmptySubstr)
{
    std::string str = initRandomStr(10);
    str += '$';
    SuffixTree tree = SuffixTree(str);
    std::vector<size_t> result = findAllOccurences(tree, "");
    EXPECT_EQ(str.length(), result.size());
}

TEST(FindAllOccurencesTest, NoSuchSubstings)
{
    {
        std::string str = "abacabaacbaba";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "bcb");
        std::sort(result.begin(), result.end());
        EXPECT_EQ(0, result.size());
    }

    {
        std::string str = "abacabaacbaba";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "aab");
        std::sort(result.begin(), result.end());
        EXPECT_EQ(0, result.size());
    }

    {
        std::string str = "abacabaacbaba";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "f");
        std::sort(result.begin(), result.end());
        EXPECT_EQ(0, result.size());
    }
}

TEST(FindAllOccurencesTest, IntersectedSubstrings)
{
    {
        std::string str = "aababaaaab";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "aba");
        std::sort(result.begin(), result.end());
        ASSERT_EQ(2, result.size());
        EXPECT_EQ(1, result[0]);
        EXPECT_EQ(3, result[1]);
    }
}

TEST(FindAllOccurencesTest, RandomTest)
{
    std::string str = initRandomStr(MAX_LENGTH);
    str.push_back('$');

    std::string substr = initRandomStr(5);

    SuffixTree tree = SuffixTree(str);
    std::vector<size_t> result = findAllOccurences(tree, substr);
    for (size_t i = 0; i < result.size(); i++)
        for (size_t j = 0; j < substr.length(); j++)
            EXPECT_EQ(substr[j], str[result[i] + j]);
}

TEST(FindAllOccurencesTest, SmallTests)
{
    {
        std::string str = "qwerdwerwer$";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "wer");
        std::sort(result.begin(), result.end());
        ASSERT_EQ(3, result.size());
        EXPECT_EQ(1, result[0]);
        EXPECT_EQ(5, result[1]);
        EXPECT_EQ(8, result[2]);
    }

    {
        std::string str = "abacabaabbbababac$";
        SuffixTree tree = SuffixTree(str);
        std::vector<size_t> result = findAllOccurences(tree, "ab");
        std::sort(result.begin(), result.end());
        ASSERT_EQ(5, result.size());
        EXPECT_EQ(0, result[0]);
        EXPECT_EQ(4, result[1]);
        EXPECT_EQ(7, result[2]);
        EXPECT_EQ(11, result[3]);
        EXPECT_EQ(13, result[4]);
    }
}


