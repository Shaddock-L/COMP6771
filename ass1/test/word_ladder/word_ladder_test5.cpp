#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <unordered_set>
#include <string>
#include <vector>
/*
Special test. The lexicon does not contain word with only 1 character.
In this test, I will create a little dictionary to test that scenario.
*/

TEST_CASE("airplane -> tricycle") {
	auto little_dic = std::unordered_set<std::string>{"a", "b", "c", "d", "e"};
	auto const ladders = word_ladder::generate("a", "e", little_dic);

	CHECK(std::size(ladders) == 1);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"a", "e"}) == 1);
}