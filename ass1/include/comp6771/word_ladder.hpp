// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef COMP6771_WORD_LADDER_HPP
#define COMP6771_WORD_LADDER_HPP

#include <unordered_set>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>


namespace word_ladder {
	[[nodiscard]] auto read_lexicon(std::string const& path) -> std::unordered_set<std::string>;

	// Given a start word and destination word, returns all the shortest possible paths from the
	// start word to the destination, where each word in an individual path is a valid word per the
	// provided lexicon. Pre: ranges::size(from) == ranges::size(to) Pre: valid_words.contains(from)
	// and valid_words.contains(to)
	[[nodiscard]] auto generate(std::string const& from,
	                            std::string const& to,
	                            std::unordered_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>>;
} // namespace word_ladder


void backtracking(std::string parent,
                  std::string end_word,
                  std::unordered_map<std::string,std::vector<std::string>> &children,
                  std::vector<std::string> &path,
                  std::vector<std::vector<std::string>> &res);

#endif // COMP6771_WORD_LADDER_HPP
