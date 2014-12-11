#include <gtest/gtest.h>
#include <DifferentSubstrings.cpp>
#include <SuffixArray.cpp>
#include <CommonSubstrings.cpp>

TEST(TasksTest, DifferentSubstrings)
{
    EXPECT_EQ(0, findMaxLengthsOfDifferentSubstrings(""));
    EXPECT_EQ(1, findMaxLengthsOfDifferentSubstrings("a"));
    EXPECT_EQ(2, findMaxLengthsOfDifferentSubstrings("aa"));
    EXPECT_EQ(5, findMaxLengthsOfDifferentSubstrings("aaaaa"));
    EXPECT_EQ(6, findMaxLengthsOfDifferentSubstrings("abc"));
    EXPECT_EQ(14, findMaxLengthsOfDifferentSubstrings("hello"));
    EXPECT_EQ(11, findMaxLengthsOfDifferentSubstrings("ababab"));
    EXPECT_EQ(15, findMaxLengthsOfDifferentSubstrings("banana"));
    EXPECT_EQ(21, findMaxLengthsOfDifferentSubstrings("abacaba"));
    EXPECT_EQ(8, findMaxLengthsOfDifferentSubstrings("abba"));
    EXPECT_EQ(54, findMaxLengthsOfDifferentSubstrings("qwerdwerwer"));
}

TEST(TasksTest, SuffixArray)
{
    std::vector< std::vector<int> > answers =
        {   {},
            {0},
            {1, 0},
            {4, 3, 2, 1, 0},
            {0, 1, 2},
            {1, 0, 2, 3, 4},
            {4, 2, 0, 5, 3, 1},
            {5, 3, 1, 0, 4, 2},
            {6, 4, 0, 2, 5, 1, 3},
            {3, 0, 2, 1},
            {4, 9, 2, 6, 0, 10, 3, 7, 8, 1, 5}  };
    EXPECT_EQ(answers[0], buildSuffixArray(""));
    EXPECT_EQ(answers[1], buildSuffixArray("a"));
    EXPECT_EQ(answers[2], buildSuffixArray("aa"));
    EXPECT_EQ(answers[3], buildSuffixArray("aaaaa"));
    EXPECT_EQ(answers[4], buildSuffixArray("abc"));
    EXPECT_EQ(answers[5], buildSuffixArray("hello"));
    EXPECT_EQ(answers[6], buildSuffixArray("ababab"));
    EXPECT_EQ(answers[7], buildSuffixArray("banana"));
    EXPECT_EQ(answers[8], buildSuffixArray("abacaba"));
    EXPECT_EQ(answers[9], buildSuffixArray("abba"));
    EXPECT_EQ(answers[10], buildSuffixArray("qwerdwerwer"));
}

TEST(TasksTest, CommonSubstrings)
{
    std::vector< std::vector<int> > answers =
        {   {1},
            {0},
            {0},
            {1},
            {3},
            {1},
            {1},
            {1, 1, 1},
            {0, 0},
            {3, 1},
            {2, 1},
            {3, 2, 1},
            {3, 3},
            {3, 3, 0}   };
    EXPECT_EQ(answers[0], findMaxLengthsOfCommonSubstrings({"a", "ab"}));
    EXPECT_EQ(answers[1], findMaxLengthsOfCommonSubstrings({"", "abscsd"}));
    EXPECT_EQ(answers[2], findMaxLengthsOfCommonSubstrings({"fsdasa", "lkkjiojn"}));
    EXPECT_EQ(answers[3], findMaxLengthsOfCommonSubstrings({"a", "a"}));
    EXPECT_EQ(answers[4], findMaxLengthsOfCommonSubstrings({"aaa", "aaa"}));
    EXPECT_EQ(answers[5], findMaxLengthsOfCommonSubstrings({"abcd", "dcba"}));
    EXPECT_EQ(answers[6], findMaxLengthsOfCommonSubstrings({"azbzc", "aybyc"}));
    EXPECT_EQ(answers[7], findMaxLengthsOfCommonSubstrings({"a", "a", "a", "a"}));
    EXPECT_EQ(answers[8], findMaxLengthsOfCommonSubstrings({"a", "dgdfb", "poikll"}));
    EXPECT_EQ(answers[9], findMaxLengthsOfCommonSubstrings({"abcd", "ab", "bcd"}));
    EXPECT_EQ(answers[10], findMaxLengthsOfCommonSubstrings({"abc", "bca", "cab"}));
    EXPECT_EQ(answers[11], findMaxLengthsOfCommonSubstrings({"a", "aa", "aaa", "aaaa"}));
    EXPECT_EQ(answers[12], findMaxLengthsOfCommonSubstrings({"kabaeeqab", "abassd", "weffabbaba"}));
    EXPECT_EQ(answers[13], findMaxLengthsOfCommonSubstrings({"qwq", "vvvqwq", "ssdfqwqvv", "ssvv"}));
}
