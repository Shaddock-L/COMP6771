#include "comp6771/word_ladder.hpp"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
// Please note: it's not good practice to test your code via a main function that does
//  printing. Instead, you should be using your test folder. This file should only really
//  be used for more "primitive" debugging as we know that working solely with test
//  frameworks might be overwhelming for some.

auto main() -> int {
	// auto const english_lexicon = word_ladder::read_lexicon("./test/word_ladder/english.txt");
	/*------Change the path here-----  */
	auto const english_lexicon = word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/"
	                                                       "english.txt");
	// get the start word and destination word.
	/*std::string (start_word);
	std::string (end_word);
	std::cout <<"Enter the start word: ";
	std::cin >> start_word;
	std::cout<< "Enter the end word: ";
	std::cin >> end_word;
	*/
	//
	// std::unordered_map<std::string, std::vector<std::string>> (possible_word);
	// std::queue<std::string> (wd_queue);

	// wd_queue.push(start_word);
	//
	// Possible_words_in_lexicon(start_word, english_lexicon, wd_queue, possible_word);

	auto const ladders = word_ladder::generate("sky", "fly", english_lexicon);
	std::cout << "ladder size is : " << ladders.size() << "\n";
	for (auto ar : ladders)
	{
		for (auto node : ar)
		{
			std::cout << node << " ";
		}
		std::cout << "\n";
	}
	// debug here
}
