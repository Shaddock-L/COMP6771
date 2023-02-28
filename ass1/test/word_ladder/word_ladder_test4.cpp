#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <string>
#include <vector>
/*
Normal test. Using the example in the README. Checking the length and contents of paths from
"airplane" to "tricycle" is NULL.

*/

TEST_CASE("airplane -> tricycle") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	// auto const english_lexicon =
	// word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/english.txt");
	auto const ladders = word_ladder::generate("airplane", "tricycle", english_lexicon);

	CHECK(std::size(ladders) == 0);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));
}