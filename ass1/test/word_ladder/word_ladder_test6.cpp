#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <string>
#include <vector>
/*
Normal test. Checking the length and contents of paths
from "hot" to "dig".
*/

TEST_CASE("airplane -> tricycle") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	// auto const english_lexicon =
	// word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/english.txt");
	auto const ladders = word_ladder::generate("hot", "dig", english_lexicon);

	CHECK(std::size(ladders) == 4);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));
	CHECK(
	   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"hot", "dot", "dit", "dig"})
	   == 1);
	CHECK(
	   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"hot", "dot", "dog", "dig"})
	   == 1);
	CHECK(
	   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"hot", "hit", "dit", "dig"})
	   == 1);
	CHECK(
	   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"hot", "hog", "dog", "dig"})
	   == 1);
}