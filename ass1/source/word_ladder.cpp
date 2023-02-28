#include "comp6771/word_ladder.hpp"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>
// Write your implementation here
// getting all possible characters combinations in the lexicon

// global variable

/*
This program is implemented by using double-side bfs and backtracking. I used single-side bfs first
while the speed was so slow. To optimize the running speed, I also shrink the size of the lexicon.
As we can assume that the length of start word and end word are same, only the words with the same
size of the start word are useful. I think that can reduce the time of running the
dicitionary.count().
*/

/*
This is a backtracking function. It is used for getting all the paths.
*/

void bkTracking(std::string parent,
                std::string end_word,
                std::unordered_map<std::string, std::vector<std::string>>& children,
                std::vector<std::string>& path,
                std::vector<std::vector<std::string>>& res) {
	if (parent == end_word) {
		res.emplace_back(path);
	}

	for (auto& child : children[parent]) {
		path.emplace_back(child);
		bkTracking(child, end_word, children, path, res);
		path.pop_back();
	}
}

namespace word_ladder {

	auto generate(std::string const& from,
	              std::string const& to,
	              std::unordered_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>>

	{
		auto result = std::vector<std::vector<std::string>>{};
		auto dict = std::unordered_set<std::string>{};
		// shrink the lexicon to smaller dictionary, for improving the running speed.
		for (auto const& wd : lexicon) {
			if (wd.size() == from.size()) {
				dict.insert(wd);
			}
		}
		/* unordered_set.count is much more faster than std::find(), to checking whether the string is
		   in the container as fast as possible, I use unordered_set as the container.
		*/
		auto head = std::unordered_set<std::string>{from};
		auto rear = std::unordered_set<std::string>{to};
		auto found = false; // initial state: not found
		auto children = std::unordered_map<std::string, std::vector<std::string>>{}; // record the
		                                                                             // string and its
		                                                                             // childrens
		auto backward = false; // initial state: forward
		dict.erase(from);
		dict.erase(to);
		while (!head.empty() and !found) {
			auto visited = std::unordered_set<std::string>{}; // store the visited strings
			for (auto& ini_word : head) {
				auto new_word = ini_word;
				for (std::string::size_type i = 0; i < new_word.size(); i++) {
					auto ini_c = new_word[i];
					// change the characters
					for (char change = 'a'; change <= 'z'; change++) {
						//  enter the new loop if the string is not changed
						if (new_word[i] == change) {
							continue;
						}
						new_word[i] = change;
						if (backward)
						// the new_word is the parent of the ini_word
						{
							// std::cout << "back" << "\n";
							//   2 sets meet in the middle
							if (rear.count(new_word)) {
								found = true;
								// std::cout << "got!" << "\n";
								children[new_word].emplace_back(ini_word); // a word and its children
							}
							if (dict.count(new_word)) {
								children[new_word].emplace_back(ini_word); // a word and its children
								visited.insert(new_word);
							}
						}
						else {
							// the ini_word is the parent of new_word
							if (rear.count(new_word)) {
								found = true;
								// std::cout << "got!" << "\n";
								children[ini_word].emplace_back(new_word); // a word and its children
							}
							if (dict.count(new_word)) {
								children[ini_word].emplace_back(new_word); // a word and its children
								visited.insert(new_word);
							}
						}
					}
					new_word[i] = ini_c; // reset the new_word;
				}
			}
			for (auto& used_word : visited) {
				dict.erase(used_word);
			}
			if (visited.size() < rear.size()) {
				head = visited;
			}
			else
			// Find the smaller one to improve the speed
			{
				head = rear;
				rear = visited;
				backward = !backward;
			}
		}

		auto all_paths = std::vector<std::string>{from};
		if (found) {
			bkTracking(from, to, children, all_paths, result);

			std::sort(result.begin(), result.end());
			return result;
		}
		// std::cout << "not found!" << "\n";
		std::sort(result.begin(), result.end());
		return result;
	}
} // namespace word_ladder

//-----------------------------------------------------------------------
