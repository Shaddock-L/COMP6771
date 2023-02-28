#include "comp6771/word_ladder.hpp"

#include "catch2/catch.hpp"
#include <string>
#include <vector>
/*
Normal test. Using the example in the README. Checking the length and contents of paths from "awake"
to "sleep".

*/

TEST_CASE("awake -> sleep") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	// auto const english_lexicon =
	// word_ladder::read_lexicon("/home/lly/ass1/test/word_ladder/english.txt");
	auto const ladders = word_ladder::generate("awake", "sleep", english_lexicon);

	CHECK(std::size(ladders) == 2);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"awake",
	                                          "aware",
	                                          "sware",
	                                          "share",
	                                          "sharn",
	                                          "shawn",
	                                          "shewn",
	                                          "sheen",
	                                          "sheep",
	                                          "sleep"})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{"awake",
	                                          "aware",
	                                          "sware",
	                                          "share",
	                                          "shire",
	                                          "shirr",
	                                          "shier",
	                                          "sheer",
	                                          "sheep",
	                                          "sleep"})
	      == 1);
}
