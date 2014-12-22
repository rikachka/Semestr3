#include "stdafx.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <conio.h>
#include <iomanip>


class SuffArray
{
public:
	SuffArray(std::string& str)
		:str_(str), str_length_(str.size())
	{}

	std::vector<int> construct()
	{
		sortLetters();
		getLettersEquivalenceClass();
		for (size_t sorted_length = 1; sorted_length < str_length_; sorted_length = sorted_length*2)
		{
			renewSuffArray(sorted_length);
			renewClassEquivalence(sorted_length);
		}
		return suff_array_;
	}

private:
	std::string str_;
	size_t str_length_;
	std::vector<int> suff_array_;
	std::vector<int> equivalence_class_;

	void sortLetters()
	{
		std::vector<std::pair<char, int> > letters_str;
		for (size_t i = 0; i < str_length_; ++i)
			letters_str.push_back(std::make_pair<char, int>(str_[i], i));
		sort(letters_str.begin(), letters_str.end());
		for (size_t i = 0; i < str_length_; ++i)
			suff_array_.push_back(letters_str[i].second);
	}

	void getLettersEquivalenceClass()
	{
		if (str_length_ == 0)
			return;
		equivalence_class_.resize(str_length_);
		equivalence_class_[suff_array_[0]] = 0;
		for (size_t i = 1; i < str_length_; ++i)
		{
			if (str_[suff_array_[i]] == str_[suff_array_[i - 1]])
				equivalence_class_[suff_array_[i]] = equivalence_class_[suff_array_[i - 1]];
			else
				equivalence_class_[suff_array_[i]] = equivalence_class_[suff_array_[i - 1]] + 1;
		}
	}

	void renewSuffArrayBySecondPart(size_t sorted_length)
	{
		std::vector<int> new_suff_array;
		new_suff_array.resize(str_length_);
		for (size_t i = 0; i < str_length_; ++i)
		{
			size_t position = suff_array_[i];
			size_t new_position = (position + str_length_ - sorted_length) % str_length_;
			new_suff_array[i] = new_position;
		}
		suff_array_.swap(new_suff_array);
	}

	void renewSuffArrayByFirstPart(size_t sorted_length)
	{
		std::vector<size_t> equivalent_elements_number;
		equivalent_elements_number.resize(str_length_, 0);
		for (size_t i = 0; i < str_length_; ++i)
		{
			equivalent_elements_number[ equivalence_class_[i] ]++;
		}
		std::vector<size_t> previous_elements_number;
		previous_elements_number.resize(str_length_, 0);
		for (size_t i = 1; i < str_length_; ++i)
		{
			previous_elements_number[i] += previous_elements_number[i - 1] + equivalent_elements_number[i - 1];
		}

		std::vector<int> new_suff_array;
		new_suff_array.resize(str_length_);
		for (size_t i = 0; i < str_length_; ++i)
		{
			size_t str_index = suff_array_[i];
			size_t eq_class = equivalence_class_[str_index];
			size_t new_position = previous_elements_number[eq_class];
			new_suff_array[new_position] = str_index;
			previous_elements_number[eq_class]++;
		}
		suff_array_.swap(new_suff_array);
	}

	void renewSuffArray(size_t sorted_length)
	{
		renewSuffArrayBySecondPart(sorted_length);
		renewSuffArrayByFirstPart(sorted_length);		
	}
	
	void renewClassEquivalence(size_t sorted_length)
	{
		if (str_length_ == 0)
			return;
		std::vector<int> new_eq_class;
		new_eq_class.resize(str_length_);
		new_eq_class[suff_array_[0]] = 0;
		for (size_t i = 1; i < str_length_; ++i)
		{
			size_t curr = suff_array_[i];
			size_t prev = suff_array_[i - 1];
			if (equivalence_class_[curr] > equivalence_class_[prev])
				new_eq_class[curr] = new_eq_class[prev] + 1;
			else if (equivalence_class_[curr] < equivalence_class_[prev])
				new_eq_class[curr] = new_eq_class[prev];
			else if (equivalence_class_[curr] == equivalence_class_[prev])
			{
				if (equivalence_class_[(curr + sorted_length) % str_length_] > equivalence_class_[(prev + sorted_length) % str_length_]) 
					new_eq_class[curr] = new_eq_class[prev] + 1;
				else
					new_eq_class[curr] = new_eq_class[prev];
			}
		}
		equivalence_class_.swap(new_eq_class);
	}
};

std::vector<int> getCycleSuffArray(std::string& str)
{
	SuffArray suffArray(str);
	return suffArray.construct();
}

std::vector<int> getNonCycleSuffArray(std::string& str)
{
	std::string new_str = str + '\0';
	SuffArray suffArray(new_str);
	std::vector<int> suff_array_with_null_element = suffArray.construct(); // первый элемент - нулевой
	std::vector<int> suff_array_without_null_element(suff_array_with_null_element.begin() + 1, suff_array_with_null_element.end());
	return suff_array_without_null_element;
}

std::vector<int> constructLcpArray(std::string& str, std::vector<int> suff_array)
{
	size_t str_length = str.length();
	str = str + '$';

	std::vector<int> inverse_suff_array;
	inverse_suff_array.resize(str_length);
	for (size_t i = 0; i < str_length; i++)
		inverse_suff_array[suff_array[i]] = i;
	
	std::vector<int> lcp_array;
	lcp_array.resize(str_length - 1);
	int lcp = 0;
	for (size_t index = 0; index < str_length; index++)
	{
		lcp = std::max(0, lcp - 1);
		int index_projection = inverse_suff_array[index];
		if (index_projection >= str_length - 1) 
			continue;
		size_t next_index = suff_array[index_projection + 1];
		while (str[index + lcp] == str[next_index + lcp])
			lcp++;
		lcp_array[index_projection] = lcp;
	}
	
	return lcp_array;
}

std::vector<int> constructCycleLcpArray(std::string& str, std::vector<int> suff_array)
{
	size_t str_length = str.length();

	std::vector<int> inverse_suff_array;
	inverse_suff_array.resize(str_length);
	for (size_t i = 0; i < str_length; i++)
		inverse_suff_array[suff_array[i]] = i;
	
	std::vector<int> lcp_array;
	lcp_array.resize(str_length - 1);
	int lcp = 0;
	for (size_t index = 0; index < str_length; index++)
	{
		lcp = std::max(0, lcp - 1);
		int index_projection = inverse_suff_array[index];
		if (index_projection >= str_length - 1) 
			continue;
		size_t next_index = suff_array[index_projection + 1];
		while (lcp < str_length && str[(index + lcp) % str_length] == str[(next_index + lcp) % str_length])
			lcp++;
		lcp_array[index_projection] = lcp;
		lcp = std::min( std::min(lcp, int(str_length - index)), int(str_length - next_index) );
	}
	
	return lcp_array;
}

int main()
{
	int str_length;
	std::string str;
	std::cin >> str_length >> str;
	if (str_length == 1)
	{
		std::cout << 0;
		return 0;
	}
	std::string str_twice = str + str;
	std::vector<int> suffArray = getNonCycleSuffArray(str_twice);

	std::vector<int> lcpArray = constructLcpArray(str_twice, suffArray);

	double average_lcp = 0;
	int last_lcp = 0;
	for (size_t i = 0; i < lcpArray.size(); ++i)
	{
		if (suffArray[i + 1] < str.length())
		{
			average_lcp += std::min(last_lcp, lcpArray[i]);
			last_lcp = str.length();
		}
		else
		{
			last_lcp = std::min(last_lcp, lcpArray[i]);
		}
	}
	average_lcp = average_lcp / (str.length() - 1);
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << average_lcp;
	getch();
	return 0;
}

