#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("operator == ") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	auto g2 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
		g2.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
		g2.insert_edge(from, to, weight);
	}
	REQUIRE(g1 == g2);
}