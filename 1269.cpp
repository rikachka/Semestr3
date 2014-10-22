#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <conio.h>
#include <algorithm>

using namespace std;

class Trie
{
private:
	struct Node
	{
		Node* parent;
		char parent_char;
		unordered_map<char, Node*> next_node;
		Node* link_node;
		bool leaf;
		int link_leaves;
		size_t length;

		Node(Node* parent, char parent_char, size_t length) 
			: parent(parent), parent_char(parent_char), link_node(NULL), leaf(false), link_leaves(-1), length(length)
		{}
	};

	Node* root;
	list<Node*> nodes;
	Node* cur_str_node;

public:
	Trie()
	{
		root = new Node(NULL, NULL, 0);
		root -> parent = root;
		root -> link_node = root;
		nodes.push_back(root);
		cur_str_node = root;
	}

	~Trie()
	{
		for (auto it = nodes.begin(); it != nodes.end(); it++)
		{
			delete *it;
		}
	}

	void addString(const string &str)
	{
		Node* cur_node = root;
		size_t length = str.length();
		size_t cur_length = 0;
		while (cur_length < length && cur_node -> next_node[str[cur_length]])
		{
			cur_node = cur_node -> next_node[str[cur_length]];
			cur_length++;
		}
		for (; cur_length < length; cur_length++)
		{
			char cur_char = str[cur_length];
			cur_node -> next_node[cur_char] = new Node(cur_node, cur_char, cur_length);
			cur_node = cur_node -> next_node[cur_char];
			nodes.push_back(cur_node);
		}
		cur_node -> leaf = true;
	}

	void buildAutomat() const
	{
		for (auto it = nodes.begin(); it != nodes.end(); it++)
		{
			Node* cur_node = *it;
			cur_node -> link_node = getSuffLink(cur_node);
		}

		for (auto it = nodes.begin(); it != nodes.end(); it++)
		{
			Node* cur_node = *it;
			countLinkLeaves(cur_node);
		}
	}

	int goByString(const string &str)
	{
		int position = -1;
		for (size_t i = 0; i < str.length(); i++)
		{
			char cur_char = str[i];
			cur_str_node = goByChar(cur_str_node, cur_char);
			if (cur_str_node -> link_leaves > 0)
			{
				Node* cur_node = cur_str_node;
				while (!cur_node -> leaf && cur_node != root)
					cur_node = cur_node -> link_node;
				int wrong_position = i - cur_str_node -> length + 1;
				if (position == -1)
					position = wrong_position;
				else
					position = min(position, wrong_position);
			}
		}
		return position;
	}
	
private:
	Node* goSuffLink(Node* cur_node) const 
	{
		if (cur_node -> link_node == NULL)
			cur_node -> link_node = getSuffLink(cur_node);
		return cur_node -> link_node;
	}

	Node* getSuffLink(Node* cur_node) const
	{
		if (cur_node -> link_node != NULL) 
			return cur_node -> link_node;
		if (cur_node -> parent == root)
			return root;
		char cur_char = cur_node -> parent_char;
		Node* link_parent_node = goSuffLink(cur_node -> parent);
		while ( link_parent_node -> next_node.find(cur_char) == link_parent_node -> next_node.end() ) // если нет перехода по такому символу
		{
			if (link_parent_node == root)
				return root;
			link_parent_node = goSuffLink(link_parent_node);
		}
		return link_parent_node -> next_node[cur_char];
	}

	size_t countLinkLeaves(Node* cur_node) const
	{
		if (cur_node -> link_leaves != -1)
			return cur_node -> link_leaves;
		if (cur_node == root)
		{
			if (cur_node -> leaf)
				return 1;
			else 
				return 0;
		}
		cur_node -> link_leaves = countLinkLeaves(cur_node -> link_node);
		if (cur_node -> leaf)
			cur_node -> link_leaves ++;
		return cur_node -> link_leaves;
	}

	Node* goByChar(Node* cur_node, char cur_char) const
	{
		if ( cur_node -> next_node.find(cur_char) != cur_node -> next_node.end() )
			return cur_node -> next_node[cur_char];
		if (cur_node == root)
			return root;		
		return goByChar(cur_node -> link_node, cur_char);
	}
};

int main()
{
	size_t number_of_words, number_of_strings;
	int go_index;
	string str;
	Trie trie;
	cin >> number_of_words;
	fflush(stdin);
	for (size_t i = 0; i < number_of_words; i++)
	{
		getline(cin, str);
		trie.addString(str);
	}
	trie.buildAutomat();
	cin >> number_of_strings;
	fflush(stdin);
	for (size_t i = 0; i < number_of_strings; i++)
	{
		getline(cin, str);
		go_index = trie.goByString(str);
		if (go_index != -1) 
		{
			cout << i << " " << go_index; 
			getch();
			return 0;
		}
	}
	cout << "Passed";
	getch();
	return 0;
}

