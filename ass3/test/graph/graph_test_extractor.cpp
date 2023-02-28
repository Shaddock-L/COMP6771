#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <sstream>
TEST_CASE("output") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};
	auto const nv = std::vector<int>{1, 2, 3, 4, 5, 6};
	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	g1.insert_node(64);
	auto out = std::ostringstream{};
	out << g1;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
	REQUIRE(out.str() == expected_output);
}