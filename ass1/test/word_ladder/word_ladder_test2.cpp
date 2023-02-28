#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <string>
#include <vector>
/*
Normal test. Using the example in the README. Checking the length and contents of paths from "work"
to "play".

*/

TEST_CASE("work -> play") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	// auto const english_lexicon =
	// word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/english.txt");
	auto const ladders = word_ladder::generate("work", "play", english_lexicon);

	CHECK(std::size(ladders) == 12);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "fork", "form", "foam", "flam", "flay", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "pork", "perk", "peak", "pean", "plan", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "pork", "perk", "peak", "peat", "plat", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "pork", "perk", "pert", "peat", "plat", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "pork", "porn", "pirn", "pian", "plan", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "pork", "port", "pert", "peat", "plat", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "word", "wood", "pood", "plod", "ploy", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "worm", "form", "foam", "flam", "flay", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "worn", "porn", "pirn", "pian", "plan", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "wort", "bort", "boat", "blat", "plat", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "wort", "port", "pert", "peat", "plat", "play"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"work", "wort", "wert", "pert", "peat", "plat", "play"})
	      == 1);
}
