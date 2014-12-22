//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

struct LongInteger
{
	static const int BASE = 10;

	std::vector<int> figures;

	LongInteger(int number) 
	{
		if (number == 0) 
		{
			figures.push_back(0);
		} 
		else 
		{
			while (number > 0) 
			{
				figures.push_back(number % BASE);
				number /= BASE;
			}
		}
	}

	LongInteger operator+= (LongInteger& added)
	{
		figures.resize(std::max(figures.size(), added.figures.size()), 0);
		int remainder = 0;
		for (int i = 0; i < added.figures.size(); ++i)
		{
			int sum = figures[i] + added.figures[i] + remainder;
			figures[i] = sum % BASE;
			remainder = sum / BASE;
		}
		int index = added.figures.size();
		while (remainder != 0)
		{
			if (index >= figures.size())
			{
				figures.push_back(1);
				break;
			}
			int sum = figures[index] + remainder;
			figures[index] = sum % BASE;
			remainder = sum / BASE;
			index++;
		}
		return *this;
	}
};



class AhoCorasickTrie
{
private:
	struct Node
	{
		int parent;
		char parent_char;
		std::map<char, int> next;
		std::map<char, int> go;
		bool terminal;
		int suffix_link;

		Node(int parent, char parent_char)
			:parent(parent), parent_char(parent_char), terminal(false), suffix_link(-1)
		{}

		Node()
			:terminal(false)
		{}
	};



public:
	AhoCorasickTrie()
	{
		nodes.push_back(Node());
	}

	void addString(std::string string)
	{
		int current_node = 0;
		for (int i = 0; i < string.size(); ++i)
		{
			char symbol = string[i];
			if (nodes[current_node].next.count(symbol))
			{
				current_node = nodes[current_node].next[symbol];
			}
			else
			{
				int new_node = nodes.size();
				nodes.push_back(Node(current_node, symbol));
				nodes[current_node].next.insert(std::make_pair(symbol, new_node));
				current_node = new_node;
			}
		}
		nodes[current_node].terminal = true;
	}

	LongInteger countGoodWords(int sentence_length, std::string& alphabet_string)
	{
		std::vector< std::vector<LongInteger> > words_number(sentence_length + 1, std::vector<LongInteger>(nodes.size(), LongInteger(0)));
		words_number[0][0] = LongInteger(1);
		for (int word_length = 0; word_length < sentence_length; ++word_length)
		{
			for (int cur_node_index = 0; cur_node_index < nodes.size(); ++cur_node_index)
			{
				if (nodes[cur_node_index].terminal || 
					(words_number[word_length][cur_node_index].figures.size() == 1 && words_number[word_length][cur_node_index].figures[0] == 0))
				{
					continue;
				}
				for (int i = 0; i < alphabet_string.size(); ++i)
				{
					int next_node_index = goSuffLink(cur_node_index, alphabet_string[i]);
					words_number[word_length + 1][next_node_index] += words_number[word_length][cur_node_index];
				}
			}
		}
		LongInteger result = LongInteger(0);
		for (int node_index = 0; node_index < nodes.size(); ++node_index)
			if (!nodes[node_index].terminal)
				result += words_number[sentence_length][node_index];
		return result;
	}



private:
	int getSuffLink(int node_index) {
		if (nodes[node_index].suffix_link == -1) {
			if (node_index == 0 || nodes[node_index].parent == 0) {
				nodes[node_index].suffix_link = 0;
			} else {
				nodes[node_index].suffix_link = goSuffLink(getSuffLink(nodes[node_index].parent), nodes[node_index].parent_char);
			}
		}
		return nodes[node_index].suffix_link;
	}

	int goSuffLink(int node_index, char symbol) 
	{
		if (!nodes[node_index].go.count(symbol))
		{
			if (!nodes[node_index].next.count(symbol)) 
			{
				if (node_index == 0) 
					nodes[node_index].go.insert(std::pair<char, int>(symbol, 0));
				else 
					nodes[node_index].go.insert(std::pair<char, int>(symbol, goSuffLink(getSuffLink(node_index), symbol)));
			}
			else
			{
				nodes[node_index].go.insert(std::pair<char, int>(symbol, nodes[node_index].next[symbol]));
			}
		}
		return nodes[node_index].go[symbol];
	}



	std::vector<Node> nodes;
};



int main()
{
	int alphabet_size, sentence_length, bad_words_number;
	std::string alphabet_str;
	std::cin >> alphabet_size >> sentence_length >> bad_words_number;
	std::cin >> alphabet_str;

	AhoCorasickTrie trie;

	std::vector<std::string> bad_words;
	for (int i = 0; i < bad_words_number; ++i) {
		std::string bad_word;
		std::cin >> bad_word;
		bool success = true;
		for (size_t i = 0; i < bad_words.size(); ++i) {
			success = (bad_word.find(bad_words[i]) == std::string::npos);
			if (!success) {
				break;
			}
			success = (bad_words[i].find(bad_word) == std::string::npos);
			if (!success) {
				std::swap(bad_words[i], bad_word);
				break;
			}
		}
		if (success) {
			bad_words.push_back(bad_word);
		}
	}
	for (size_t i = 0; i < bad_words.size(); ++i) {
		trie.addString(bad_words[i]);
	}

	LongInteger result = trie.countGoodWords(sentence_length, alphabet_str);
	for (int i = result.figures.size() - 1; i >= 0; --i)
		std::cout << result.figures[i];

	return 0;
}

