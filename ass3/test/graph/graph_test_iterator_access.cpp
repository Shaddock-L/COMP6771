#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("begin") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	auto head = g1.begin();
	auto wanted = g1.find('A', 'B', 4);
	REQUIRE(head == wanted);
	g1.erase_edge('A', 'B', 4);
	auto n_head = g1.begin();
	auto wanted_2 = g1.find('A', 'C', 10);
	REQUIRE(n_head == wanted_2);

	auto g2 = graph{};
	REQUIRE(g2.begin() == g2.end());
}

TEST_CASE("end") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	auto tail = g1.end();
	auto wanted = g1.find('C', 'A', 2);
	REQUIRE((--tail) == wanted);
}
