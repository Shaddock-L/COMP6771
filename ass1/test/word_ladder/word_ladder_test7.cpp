#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <string>
#include <vector>
/*
Normal test. Checking the length and contents of paths
from "word" to "poly".
*/

TEST_CASE("word -> poly") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	// auto const english_lexicon =
	// word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/english.txt");
	auto const ladders = word_ladder::generate("word", "poly", english_lexicon);

	CHECK(std::size(ladders) == 6);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "cord", "cold", "coly", "poly"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "cord", "cory", "coly", "poly"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "wold", "cold", "coly", "poly"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "wold", "hold", "holy", "poly"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "wold", "mold", "moly", "poly"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"word", "wore", "pore", "pole", "poly"})
	      == 1);
}